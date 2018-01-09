// Added Chapter 4.1

#ifndef _GAMEOBJECT_H_
#define _GAMEOBJECT_H_

#include "btBulletDynamicsCommon.h"

#include "OpenGLMotionState.h"
#include <string>

enum ObjectTypes {
	NORMAL = 0,
	PROJECTILE
};

class GameObject {
public:
	//GameObject(btCollisionShape* pShape, float mass, const btVector3 &color, const btVector3 &initialPosition = btVector3(0, 0, 0), const btQuaternion &initialRotation = btQuaternion(0, 0, 1, 1));
	GameObject(btCollisionShape* pShape, float mass, const btVector3 &color, const btVector3 &initialPosition = btVector3(0, 0, 0), const btQuaternion &initialRotation = btQuaternion(0, 0, 1, 1), bool playAudio = false);
	~GameObject();

	// accessors
	btCollisionShape* GetShape() { return m_pShape; }

	btRigidBody* GetRigidBody() { return m_pBody; }

	btMotionState* GetMotionState() { return m_pMotionState; }

	void GetTransform(btScalar* transform) { 
		if (m_pMotionState) m_pMotionState->GetWorldTransform(transform); 
	}
	
	btVector3 GetColor() { return m_color; }						// Get the object colour

	void SetColor(const btVector3 &color) { m_color = color; }		// Ch 6.1

	bool GetPlayAudio() { return m_fx; }							// JOR Get if the object should play a sound effect
	void SetPlayAudio(bool set) { m_fx = set; }						// JOR Set if the object should play a sound effect

	std::string GetName() { return m_name; }						// JOR Get name
	void SetName(std::string set) { m_name = set; }					// JOR Set name

	bool GetAlive() { return m_Alive; }								// JOR Get if the object should play a sound effect
	void SetAlive(bool set) { m_Alive = set; }						// JOR Set if the object should play a sound effect

	int GetType() { return m_type; }
	void SetType(int set) { m_type = set; }

protected:
	btCollisionShape*  m_pShape;
	btRigidBody*    m_pBody;										// Rigid body
	OpenGLMotionState*  m_pMotionState;
	btVector3      m_color;											// Object colour

	bool m_fx;														// JOR play an effet or not
	bool m_Alive;
	std::string m_name;
	int m_type;														// Type of game object
};
#endif
