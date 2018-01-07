// Added Ch4.1

#include "GameObject.h"

/*
GameObject::GameObject(btCollisionShape* pShape, float mass, const btVector3 &color, const btVector3 &initialPosition, const btQuaternion &initialRotation) {
	m_pShape = pShape;																			// store the shape for later usage	
	m_color = color;																			// store the color

																								// create the initial transform
	btTransform transform;
	transform.setIdentity();
	transform.setOrigin(initialPosition);
	transform.setRotation(initialRotation);

	m_pMotionState = new OpenGLMotionState(transform);											// create the motion state from the initial transform

	btVector3 localInertia(0, 0, 0);															// calculate the local inertia

	if (mass != 0.0f) pShape->calculateLocalInertia(mass, localInertia);						// objects of infinite mass can't move or rotate

	btRigidBody::btRigidBodyConstructionInfo cInfo(mass, m_pMotionState, pShape, localInertia);	// create the rigid body construction info using the mass, motion state and shape

	m_pBody = new btRigidBody(cInfo);															// create the rigid body

	m_fx = false;																				// Don't play sound effect
}
*/
// JOR
GameObject::GameObject(btCollisionShape* pShape, float mass, const btVector3 &color, const btVector3 &initialPosition, const btQuaternion &initialRotation, bool playAudio) {
	m_pShape = pShape;																			// store the shape for later usage	
	m_color = color;																			// store the color

																								// create the initial transform
	btTransform transform;
	transform.setIdentity();
	transform.setOrigin(initialPosition);
	transform.setRotation(initialRotation);

	m_pMotionState = new OpenGLMotionState(transform);											// create the motion state from the initial transform

	btVector3 localInertia(0, 0, 0);															// calculate the local inertia

	if (mass != 0.0f) pShape->calculateLocalInertia(mass, localInertia);						// objects of infinite mass can't move or rotate

	btRigidBody::btRigidBodyConstructionInfo cInfo(mass, m_pMotionState, pShape, localInertia);	// create the rigid body construction info using the mass, motion state and shape

	m_pBody = new btRigidBody(cInfo);															// create the rigid body

	playAudio = false;
}

GameObject::~GameObject() {
	delete m_pBody;
	delete m_pMotionState;
	delete m_pShape;
}
