#include "../BulletOpenGLApplication.h"
#include "btBulletDynamicsCommon.h"
#include <iostream>

#define SDL_MAIN_HANDLED															// JOR Handling SDL main definition ourselves
#define BLOCKS_IN_ROW 11															// Number of blocks in a row in castle walls
#define EXPLOSION_STRENGTH 50.0f													// Strength of the explosion
#define RADIANS_PER_DEGREE 0.01745329f												// Number of radians per degree

class JoeSceneV2 : public BulletOpenGLApplication {
public:
	JoeSceneV2();																	// Constructor

	virtual void InitializePhysics() override;										// Initialise physics. Override BulletOpenGLApplication function
	virtual void ShutdownPhysics() override;										// Clear physics elements from scene. Override BulletOpenGLApplication function

	void InitSDLAudio();															// Initialise music and sound effects

	void CreateObjects();

	virtual void Keyboard(unsigned char key, int x, int y) override;				// Override Key function of BulletOpenGLApplication
	virtual void KeyboardUp(unsigned char key, int x, int y) override;				// Override Key up function of BulletOpenGLApplication 
	virtual void Mouse(int button, int state, int x, int y) override;				// Override the Mouse() function of BulletOpenGLApplication
	virtual void UpdateScene(float dt);												// Scene updating. Overriden from parent class

	virtual void CollisionEvent(btRigidBody* pBody0, btRigidBody* pBody1) override;	// Handle collisions. Override BulletOpenGLApplication function

	// New projectiles
	void ShootBall(const btVector3 &direction);										// Fire sphere objects from camera position
	void ShootArrow(const btVector3 &direction);									// Collection of shapes making up arrow
	void ShootArrowCompound(const btVector3 &direction);							// Compound shape arrow projectile
	void CreateExplosion(const btVector3 &origin);									// Create explosion at position
	//void CreateExplosion2(btTransform &transform);

protected:	
	GameObject* m_pBox;																// Right click - box to lift
	//btCollisionObject* m_pTrigger;												// Simple trigger volume

	bool m_bApplyForce;																// Is the 'g' key held down

	// Explosion variables
	btCollisionObject* m_pExplosion;	
	bool m_canFireBall;
	bool m_bCanExplode;
};