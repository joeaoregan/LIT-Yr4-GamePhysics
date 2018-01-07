#include "SoftBodyDemo.h"

SoftBodyDemo::SoftBodyDemo()
	:
BulletOpenGLApplication()
{
}

void SoftBodyDemo::InitializePhysics() {	
	m_pCollisionConfiguration = new btSoftBodyRigidBodyCollisionConfiguration();								// create the collision configuration
	
	m_pDispatcher = new btCollisionDispatcher(m_pCollisionConfiguration);										// create the dispatcher

	m_pBroadphase = new btDbvtBroadphase();																		// create the broadphas
			
	m_pSolver = new btSequentialImpulseConstraintSolver();														// create the constraint solver
		
	m_pWorld = new btSoftRigidDynamicsWorld(m_pDispatcher, m_pBroadphase, m_pSolver, m_pCollisionConfiguration);// create the softbody world
	
	m_pSoftBodyWorld = (btSoftRigidDynamicsWorld*)m_pWorld;														// typecast the world and store it for future usage

	// initialize the world info for soft bodies
	m_softBodyWorldInfo.m_dispatcher = m_pDispatcher;
	m_softBodyWorldInfo.m_broadphase = m_pBroadphase;
	m_softBodyWorldInfo.m_sparsesdf.Initialize();

	CreateObjects();
}

void SoftBodyDemo::ShutdownPhysics() {
	delete m_pWorld;
	delete m_pSolver;
	delete m_pBroadphase;
	delete m_pDispatcher;
	delete m_pCollisionConfiguration;
}

void SoftBodyDemo::CreateObjects() {	
	CreateGameObject(new btBoxShape(btVector3(1,50,50)), 0, btVector3(0.2f, 0.6f, 0.6f), btVector3(0.0f, 0.0f, 0.0f));		// create a ground plane

	btSoftBody*  pSoftBody = btSoftBodyHelpers::CreateEllipsoid(m_softBodyWorldInfo,btVector3(0,0,0),btVector3(3,3,3),128);	// create a soft 'ball' with 128 sides and a radius of 3
		
	pSoftBody->translate(btVector3(0,15,0));																				// set the body's position

	pSoftBody->m_cfg.kVC = 0.5;																								// set the 'volume conservation coefficient'
		
	pSoftBody->m_materials[0]->m_kLST = 0.5;																				// set the 'linear stiffness'
		
	pSoftBody->setTotalMass(5);																								// set the total mass of the soft body
		
	pSoftBody->setPose(true,false);																							// tell the soft body to initialize and attempt to maintain the current pose
		
	m_pSoftBodyWorld->addSoftBody(pSoftBody);																				// add the soft body to our world
}

void SoftBodyDemo::RenderScene() {	
	BulletOpenGLApplication::RenderScene();																					// call the base rendering code first
	// check the list of our world's soft bodies
	for (int i=0; i< m_pSoftBodyWorld->getSoftBodyArray().size(); i++) {		
		btSoftBody*  pBody = (btSoftBody*)m_pSoftBodyWorld->getSoftBodyArray()[i];											// get the body
		// is it possible to render?
		if (m_pSoftBodyWorld->getDebugDrawer() && !(m_pSoftBodyWorld->getDebugDrawer()->getDebugMode() & (btIDebugDraw::DBG_DrawWireframe))) {			
			btSoftBodyHelpers::Draw(pBody, m_pSoftBodyWorld->getDebugDrawer(), m_pSoftBodyWorld->getDrawFlags());			// draw it
		}
	}
}