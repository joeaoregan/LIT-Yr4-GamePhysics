#ifndef _BULLETOPENGLAPP_H_
#define _BULLETOPENGLAPP_H_

#include <Windows.h>
#include <GL/GL.h>
#include <GL/freeglut.h>
#include "BulletDynamics/Dynamics/btDynamicsWorld.h"
#include "BulletCollision/CollisionShapes/btConvexPolyhedron.h"										// Ch 7.2 - includes for convex hulls
#include "BulletCollision/CollisionShapes/btShapeHull.h"											// Ch 7.2 - includes for convex hulls
#include "OpenGLMotionState.h"																		// include our custom Motion State object
#include "DebugDrawer.h"																			// Ch 4.2 - Our custom debug renderer
#include "GameObject.h"
#include <vector>
#include <set>																						// Ch 5.2
#include <iterator>																					// Ch 5.2
#include <algorithm>																				// Ch 5.2

typedef std::vector<GameObject*> GameObjects;														// a convenient typedef to reference an STL vector of GameObjects

// convenient typedefs for collision events
typedef std::pair<const btRigidBody*, const btRigidBody*> CollisionPair;							// Ch 6.1
typedef std::set<CollisionPair> CollisionPairs;														// Ch 6.1

// struct to store our raycasting results
struct RayResult {
	btRigidBody* pBody;
	btVector3 hitPoint;
};

class BulletOpenGLApplication {
public:
	BulletOpenGLApplication();
	~BulletOpenGLApplication();
	void Initialize();
	// FreeGLUT callbacks //
	virtual void Keyboard(unsigned char key, int x, int y);
	virtual void KeyboardUp(unsigned char key, int x, int y);
	virtual void Special(int key, int x, int y);
	virtual void SpecialUp(int key, int x, int y);
	virtual void Reshape(int w, int h);
	virtual void Idle();
	virtual void Mouse(int button, int state, int x, int y);
	virtual void PassiveMotion(int x, int y);
	virtual void Motion(int x, int y);
	virtual void Display();

	virtual void RenderScene();																		// rendering. Can be overridden by derived classes
	virtual void UpdateScene(float dt);																// scene updating. Can be overridden by derived classes

	// physics functions. Can be overriden by derived classes (like BasicDemo)
	virtual void InitializePhysics() {};
	virtual void ShutdownPhysics() {};

	void UpdateCamera();																			// camera functions
	void RotateCamera(float &angle, float value);
	void ZoomCamera(float distance);

	// drawing functions
	//void DrawBox(const btVector3 &halfSize, const btVector3 &color = btVector3(1.0f, 1.0f, 1.0f));	
	//void DrawBox(btScalar* transform, const btVector3 &halfSize, const btVector3 &color = btVector3(1.0f, 1.0f, 1.0f));
	void DrawBox(const btVector3 &halfSize);	
	void DrawSphere(const btScalar &radius);														// Ch 7.1
	void DrawCylinder(const btScalar &radius, const btScalar &halfHeight);							// Ch 7.1
	void DrawConvexHull(const btCollisionShape* shape);												// Ch 7.2
	void DrawShape(btScalar* transform, const btCollisionShape* pShape, const btVector3 &color);

	// object functions
	GameObject* CreateGameObject(btCollisionShape* pShape,
		const float &mass,
		const btVector3 &color = btVector3(1.0f, 1.0f, 1.0f),
		const btVector3 &initialPosition = btVector3(0.0f, 0.0f, 0.0f),
		const btQuaternion &initialRotation = btQuaternion(0, 0, 1, 1));

	void ShootBox(const btVector3 &direction);
	void DestroyGameObject(btRigidBody* pBody);
	GameObject* FindGameObject(btRigidBody* pBody);													// Ch 6.1

	// Ch 5.1 picking functions
	btVector3 GetPickingRay(int x, int y);
	//bool Raycast(const btVector3 &startPosition, const btVector3 &direction, RayResult &output);								// Removed Ch 6.3
	bool Raycast(const btVector3 &startPosition, const btVector3 &direction, RayResult &output, bool includeStatic = false);	// Ch 6.3
	
	// Ch 5.2 constraint functions
	void CreatePickingConstraint(int x, int y);
	void RemovePickingConstraint();

	// Ch 6.1 collision event functions
	void CheckForCollisionEvents();
	virtual void CollisionEvent(btRigidBody* pBody0, btRigidBody * pBody1);
	virtual void SeparationEvent(btRigidBody * pBody0, btRigidBody * pBody1);

protected:
	// camera control
	btVector3 m_cameraPosition;																		// the camera's current position
	btVector3 m_cameraTarget;																		// the camera's lookAt target
	float m_nearPlane;																				// minimum distance the camera will render
	float m_farPlane;																				// farthest distance the camera will render
	btVector3 m_upVector;																			// keeps the camera rotated correctly
	float m_cameraDistance;																			// distance from the camera to its target
	float m_cameraPitch; 																			// pitch of the camera 
	float m_cameraYaw; 																				// yaw of the camera

	int m_screenWidth, m_screenHeight;																// Screen dimensions

	// core Bullet components
	btBroadphaseInterface* m_pBroadphase;
	btCollisionConfiguration* m_pCollisionConfiguration;
	btCollisionDispatcher* m_pDispatcher;
	btConstraintSolver* m_pSolver;
	btDynamicsWorld* m_pWorld;

	//OpenGLMotionState* m_pMotionState;															// our custom motion state
	btClock m_clock;																				// a simple clock for counting time

	GameObjects m_objects;																			// an array of our game objects

	DebugDrawer* m_pDebugDrawer;																	// 4.2 - debug renderer
	
	// Ch 5.2 constraint variables
	btRigidBody* m_pPickedBody;																		// Ch 5.2 - the body we picked up
	btTypedConstraint*  m_pPickConstraint;															// Ch 5.2 - the constraint the body is attached to
	btScalar m_oldPickingDist;																		// Ch 5.2 - the distance from the camera to the hit point (so we can move the object up, down, left and right from our view)

	CollisionPairs m_pairsLastUpdate;																// Ch 6.1 - collision event variables
};
#endif