// Added Ch 8

#include "../BulletOpenGLApplication.h"
#include "btBulletDynamicsCommon.h"
//#include "Audio.h"

#define SDL_MAIN_HANDLED																// JOR Handling SDL main definition ourselves
#define BLOCKS_IN_ROW 11																// Number of blocks in a row in castle walls
#define EXPLOSION_STRENGTH 50.0f														// Strength of explosion
#define RADIANS_PER_DEGREE 0.01745329f	

class JoeScene : public BulletOpenGLApplication {
public:
	JoeScene();

	virtual void InitializePhysics() override;											// Initialise physics. Override BulletOpenGLApplication function
	virtual void ShutdownPhysics() override;											// Clear physics elements from scene. Override BulletOpenGLApplication function

	void CreateObjects();
	void CreateLetters();

	virtual void Keyboard(unsigned char key, int x, int y) override;					// Override Key function of BulletOpenGLApplication
	virtual void KeyboardUp(unsigned char key, int x, int y) override;					// Override Key up function of BulletOpenGLApplication 
	virtual void Mouse(int button, int state, int x, int y) override;					// Override the Mouse() function of BulletOpenGLApplication

	virtual void RenderScene() override;												// Override the RenderScene() functione of BulletOpenGLApplication
	virtual void UpdateScene(float dt) override;										// scene updating. Overriden from parent class

	virtual void CollisionEvent(btRigidBody* pBody0, btRigidBody * pBody1) override;	// Handle collisions. Override BulletOpenGLApplication function

	// New projectiles
	void ShootBall(const btVector3 &direction);											// JOR Fire sphere objects from camera position
	void ShootArrow(const btVector3 &direction);										// JOR Collection of shapes making up arrow
	void ShootArrowCompound(const btVector3 &direction);								// Compound shape arrow projectile
	void CreateExplosion(const btVector3 &origin);										// Create explosion at position

	//void displayText(float x, float y, int r, int g, int b, const char *string);		// JOR

	void incrementScore(int amount) { m_score += amount; }								// Add value to score

	void DoProjectileCollisionStuff(int type);

protected:
	// Exposions
	btCollisionObject* m_pExplosion;													// Explosion object
	bool m_canFireBall;																	// Can only fire one ball at a time
	bool m_bCanExplode;																	// If an explosion can happen or not

	int m_score;																		// Score for hitting an object
};