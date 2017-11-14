//#include <GL/freeglut.h>
#include "BasicDemo.h"

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
	btBoxShape* pBoxShape = new btBoxShape(btVector3(1.0f, 1.0f, 1.0f));													// Create a box shape of size (1,1,1)
	
	btTransform transform;																									// Give our box an initial position of (0,0,0)
	transform.setIdentity();
	transform.setOrigin(btVector3(0.0f, 0.0f, 0.0f));
	
	m_pMotionState = new OpenGLMotionState(transform);																		// Create a motion state
	// create the rigid body construction info object, giving it a mass of 1, the motion state, and the shape
	btRigidBody::btRigidBodyConstructionInfo rbInfo(1.0f, m_pMotionState, pBoxShape);
	btRigidBody* pRigidBody = new btRigidBody(rbInfo);
	
	m_pWorld->addRigidBody(pRigidBody);																						// Inform our world that we just created a new rigid body for it to manage
	*/
	
	CreateGameObject(new btBoxShape(btVector3(1, 50, 50)), 0, btVector3(0.2f, 0.6f, 0.6f), btVector3(0.0f, 0.0f, 0.0f));	// Create a ground plane
	
	CreateGameObject(new btBoxShape(btVector3(1, 1, 1)), 1.0, btVector3(1.0f, 0.2f, 0.2f), btVector3(0.0f, 10.0f, 0.0f));	// Create our original red box
	
	CreateGameObject(new btBoxShape(btVector3(1, 1, 1)), 1.0, btVector3(0.0f, 0.2f, 0.8f), btVector3(1.25f, 20.0f, 0.0f));	// Create a second box
}