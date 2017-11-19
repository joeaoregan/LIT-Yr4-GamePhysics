// Added Ch 8

#include "CollisionFilteringDemo.h"

CollisionFilteringDemo::CollisionFilteringDemo()
	:
BulletOpenGLApplication()
{
}

void CollisionFilteringDemo::InitializePhysics() {	
	m_pCollisionConfiguration = new btDefaultCollisionConfiguration();											// create the collision configuration	
	m_pDispatcher = new btCollisionDispatcher(m_pCollisionConfiguration);										// create the dispatcher	
	m_pBroadphase = new btDbvtBroadphase();																		// create the broadphase	
	m_pSolver = new btSequentialImpulseConstraintSolver();														// create the constraint solver	
	m_pWorld = new btDiscreteDynamicsWorld(m_pDispatcher, m_pBroadphase, m_pSolver, m_pCollisionConfiguration);	// create the world
	CreateObjects();
}

void CollisionFilteringDemo::ShutdownPhysics() {
	delete m_pWorld;
	delete m_pSolver;
	delete m_pBroadphase;
	delete m_pDispatcher;
	delete m_pCollisionConfiguration;
}

void CollisionFilteringDemo::CreateObjects() {
	// create a ground plane
	CreateGameObject(new btBoxShape(btVector3(1,50,50)), 
		0, btVector3(0.2f, 0.6f, 0.6f), btVector3(0.0f, 0.0f, 0.0f),
		COLGROUP_STATIC,		
		COLGROUP_BOX | COLGROUP_SPHERE);																		/*the ground plane can collide with boxes and spheres*/

	// create 25 boxes and spheres in a 5x5 stack formation 
	float yPos = 0;
	for (int i = 0; i < 25; i++) {
		// calculate the x and y for the object
		float xPos = -5 + 2*(float)(i % 5);
		if (i % 5 == 0) yPos += 2;
		
		btVector3 position(xPos, yPos, 4);																		// create a btVector3 for the position

		// create a box
		CreateGameObject(new btBoxShape(btVector3(1,1,1)),
			1.0f,
			btVector3(1.0f, 0.2f, i%2),																			/*alternate colors*/
			position,
			COLGROUP_BOX,			
			COLGROUP_BOX | COLGROUP_STATIC);																	/*boxes can collide with the ground plane and other boxes*/

		// create a sphere in the same location
		CreateGameObject(new btSphereShape(1.2),
			1.0f,			
			btVector3(i%2, 1.0f, 0.2f),																			/*alternate colors*/
			position,
			COLGROUP_SPHERE,			
			COLGROUP_SPHERE | COLGROUP_STATIC);																	/*spheres can collide with the ground plane and other spheres*/
	}
}
