//#include <GL/freeglut.h>
#include "BasicDemo.h"

// Ch 6.3
BasicDemo::BasicDemo() : BulletOpenGLApplication(),
	m_bApplyForce(false),
	m_pExplosion(0),
	m_bCanExplode(true) {
}

void BasicDemo::InitializePhysics() {	
	m_pCollisionConfiguration = new btDefaultCollisionConfiguration();														// create the collision configuration	
	m_pDispatcher = new btCollisionDispatcher(m_pCollisionConfiguration);													// create the dispatcher	
	m_pBroadphase = new btDbvtBroadphase();																					// create the broadphase	
	m_pSolver = new btSequentialImpulseConstraintSolver();																	// create the constraint solver	
	m_pWorld = new btDiscreteDynamicsWorld(m_pDispatcher, m_pBroadphase, m_pSolver, m_pCollisionConfiguration);				// create the world

	CreateObjects();																										// create our scene's physics objects
}

void BasicDemo::ShutdownPhysics() {
	delete m_pWorld;
	delete m_pSolver;
	delete m_pBroadphase;
	delete m_pDispatcher;
	delete m_pCollisionConfiguration;
}

void BasicDemo::CreateObjects() {
	/*
	// Removed Ch4.1
	btBoxShape* pBoxShape = new btBoxShape(btVector3(1.0f, 1.0f, 1.0f));															// Create a box shape of size (1,1,1)
	
	btTransform transform;																											// Give our box an initial position of (0,0,0)
	transform.setIdentity();
	transform.setOrigin(btVector3(0.0f, 0.0f, 0.0f));
	
	m_pMotionState = new OpenGLMotionState(transform);																				// Create a motion state
	// create the rigid body construction info object, giving it a mass of 1, the motion state, and the shape
	btRigidBody::btRigidBodyConstructionInfo rbInfo(1.0f, m_pMotionState, pBoxShape);
	btRigidBody* pRigidBody = new btRigidBody(rbInfo);
	
	m_pWorld->addRigidBody(pRigidBody);																								// Inform our world that we just created a new rigid body for it to manage
	*/
	
	CreateGameObject(new btBoxShape(btVector3(1, 50, 50)), 0, btVector3(0.2f, 0.6f, 0.6f), btVector3(0.0f, 0.0f, 0.0f));			// Create a ground plane	
	//CreateGameObject(new btBoxShape(btVector3(1, 1, 1)), 1.0, btVector3(1.0f, 0.2f, 0.2f), btVector3(0.0f, 10.0f, 0.0f));			// 6.2 REMOVED - Create our original red box	
	
	m_pBox = CreateGameObject(new btBoxShape(btVector3(1, 1, 1)), 1.0, btVector3(1.0f, 0.2f, 0.2f), btVector3(0.0f, 10.0f, 0.0f));	// 6.2 - create our red box, but store the pointer for future usage

	CreateGameObject(new btBoxShape(btVector3(1, 1, 1)), 1.0, btVector3(0.0f, 0.2f, 0.8f), btVector3(1.25f, 20.0f, 0.0f));			// Create a second box
		
	m_pTrigger = new btCollisionObject();																							// 6.2 - create a trigger volume	
	m_pTrigger->setCollisionShape(new btBoxShape(btVector3(1,0.25,1)));																// create a box for the trigger's shape
	// 6.2 - set the trigger's position
	btTransform triggerTrans;
	triggerTrans.setIdentity();
	triggerTrans.setOrigin(btVector3(0,1.5,0));
	m_pTrigger->setWorldTransform(triggerTrans);	
	m_pTrigger->setCollisionFlags(btCollisionObject::CF_NO_CONTACT_RESPONSE);														// 6.2 - flag the trigger to ignore contact responses	
	m_pWorld->addCollisionObject(m_pTrigger);																						// 6.2 - add the trigger to our world
																															
	CreateGameObject(new btSphereShape(1.0f), 1.0, btVector3(0.7f, 0.7f, 0.0f), btVector3(-5.0, 10.0f, 0.0f));						// Ch 7.1 - create a yellow sphere
	CreateGameObject(new btCylinderShape(btVector3(1, 1, 1)), 1.0, btVector3(0.0f, 0.7f, 0.0f), btVector3(-2, 10.0f, 0.0f));		// Ch 7.1 - create a green cylinder

	// Ch 7.2 - create a vertex cloud defining a square-based pyramid - P92
	btVector3 points[5] = {btVector3(-0.5,1,1),
		btVector3(-0.5,1,-1),
		btVector3(-0.5,-1,1),
		btVector3(-0.5,-1,-1),
		btVector3(1,0,0)};
	
	btConvexHullShape* pShape = new btConvexHullShape(&points[0].getX(),5);															// Ch 7.2 - create our convex hull	
	pShape->initializePolyhedralFeatures();																							// Ch 7.2 - initialize the object as a polyhedron	
	CreateGameObject(pShape, 1.0, btVector3(1,1,1), btVector3(5, 15, 0));															// Ch 7.2 - create the game object using our convex hull shape

	// Ch 7.3 - Create two shapes for the rod and the load
	btCollisionShape* pRod = new btBoxShape(btVector3(1.5f, 0.2f, 0.2f));
	btCollisionShape* pLoad = new btSphereShape(0.5f);
	// create a transform we'll use to set each object's position
	btTransform trans;
	trans.setIdentity();
	btCompoundShape* pCompound = new btCompoundShape();																				// Ch 7.3 - Create our compound shape
	// add the rod
	pCompound->addChildShape(trans, pRod);
	trans.setOrigin(btVector3(-1.75f, 0.0f, 0.0f));
	// add the top load
	pCompound->addChildShape(trans, pLoad);
	trans.setIdentity();
	// add the bottom load
	trans.setOrigin(btVector3(1.75f, 0.0f, 0.0f));
	pCompound->addChildShape(trans, pLoad);	
	CreateGameObject(pCompound, 2.0f, btVector3(0.8,0.4,0.1), btVector3(-4, 10.0f, 0.0f));											// Ch 7.3 - Create a game object using the compound shape
}

// Ch 6.2
void BasicDemo::CollisionEvent(btRigidBody* pBody0, btRigidBody* pBody1) {
	// did the box collide with the trigger?
	if (pBody0 == m_pBox->GetRigidBody() && pBody1 == m_pTrigger ||
		pBody1 == m_pBox->GetRigidBody() && pBody0 == m_pTrigger) {			
			CreateGameObject(new btBoxShape(btVector3(2,2,2)), 2.0, btVector3(0.3, 0.7, 0.3), btVector3(5, 10, 0));	// if yes, create a big green box nearby
	}

	// Ch 6.3 - Impulse testing
	if (pBody0 == m_pExplosion || pBody1 == m_pExplosion) {
		// get the pointer of the other object
		btRigidBody* pOther;
		pBody0 == m_pExplosion ? pOther = (btRigidBody*)pBody1 : pOther = (btRigidBody*)pBody0;
		
		pOther->setActivationState(ACTIVE_TAG);																		// wake the object up
		// calculate the vector between the object and the center of the explosion
		btVector3 dir = pOther->getWorldTransform().getOrigin() - m_pExplosion->getWorldTransform().getOrigin();		
		float dist = dir.length();																					// get the distance		
		float strength = EXPLOSION_STRENGTH;																		// calculate the impulse strength		
		if (dist != 0.0) strength /= dist;																			// follow an inverse-distance rule		
		dir.normalize();																							// normalize the direction vector		
		pOther->applyCentralImpulse(dir * strength);																// apply the impulse
	}
}

// Ch 6.3
void BasicDemo::Keyboard(unsigned char key, int x, int y) {
	// call the base implementation first
	BulletOpenGLApplication::Keyboard(key, x, y);
	switch(key) {
		// Force testing
		case 'g': {		
			m_bApplyForce = true;																					// if 'g' is held down, apply a force		
			m_pBox->GetRigidBody()->setActivationState(DISABLE_DEACTIVATION);										// prevent the box from deactivating
			break;
		}
		// Impulse testing
		case 'e': {		
			if (m_pExplosion || !m_bCanExplode) break;																// don't create a new explosion if one already exists or we haven't released the key, yet		
			m_bCanExplode = false;																					// don't let us create another explosion until the key is released																				
			// create a collision object for our explosion
			m_pExplosion = new btCollisionObject();
			m_pExplosion->setCollisionShape(new btSphereShape(3.0f));
			// get the position that we clicked
			RayResult result;
			Raycast(m_cameraPosition, GetPickingRay(x, y), result, true);
			// create a transform from the hit point
			btTransform explodeTrans;
			explodeTrans.setIdentity();
			explodeTrans.setOrigin(result.hitPoint);
			m_pExplosion->setWorldTransform(explodeTrans);
		
			m_pExplosion->setCollisionFlags(btCollisionObject::CF_NO_CONTACT_RESPONSE);								// set the collision flag		
			m_pWorld->addCollisionObject(m_pExplosion);																// add the explosion trigger to our world
			break;
		}
	}
}

// Ch 6.3
void BasicDemo::KeyboardUp(unsigned char key, int x, int y) {
	// call the base implementation first
	BulletOpenGLApplication::KeyboardUp(key, x, y);
	switch(key) {
	// Force testing
	case 'g': {			
			m_bApplyForce = false;																					// if 'g' is let go, stop applying the force			
			m_pBox->GetRigidBody()->forceActivationState(ACTIVE_TAG);												// allow the object to deactivate again
			break;
		}
	case 'e': m_bCanExplode = true; break;																			// Impulse testing
	}
}

// Ch 6.3
void BasicDemo::UpdateScene(float dt) {	
	BulletOpenGLApplication::UpdateScene(dt);																		// call the base implementation first

	// Force testing
	if (m_bApplyForce) {
		if (!m_pBox) return;		
		m_pBox->GetRigidBody()->applyCentralForce(btVector3(0, 20, 0));												// apply a central upwards force that exceeds gravity
	}

	// Impulse testing
	if (m_pExplosion) {
		// destroy the explosion object after one iteration
		m_pWorld->removeCollisionObject(m_pExplosion);
		delete m_pExplosion;
		m_pExplosion = 0;
	}
}