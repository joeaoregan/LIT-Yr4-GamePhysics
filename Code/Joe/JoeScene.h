// Added Ch 8

#include "../BulletOpenGLApplication.h"
#include "btBulletDynamicsCommon.h"
//#include "Audio.h"

#define SDL_MAIN_HANDLED																			// JOR Handling SDL main definition ourselves
#define BLOCKS_IN_ROW 11																			// Number of blocks in a row in castle walls
#define EXPLOSION_STRENGTH 50.0f																	// Strength of explosion

class JoeScene : public BulletOpenGLApplication {
public:
	JoeScene();

	virtual void UpdateScene(float dt) override;													// scene updating. Overriden from parent class
	virtual void InitializePhysics() override;
	virtual void ShutdownPhysics() override;

	void CreateObjects();

	virtual void Keyboard(unsigned char key, int x, int y) override;
	virtual void KeyboardUp(unsigned char key, int x, int y) override;
	virtual void Mouse(int button, int state, int x, int y) override;
	//virtual void UpdateScene(float dt);

	virtual void CollisionEvent(btRigidBody* pBody0, btRigidBody * pBody1) override;

	// New projectiles
	void ShootBall(const btVector3 &direction);														// JOR
	void ShootArrow(const btVector3 &direction);													// JOR
	void ShootArrowCompound(const btVector3 &direction);											// Compound shape arrow projectile

	//void displayText(float x, float y, int r, int g, int b, const char *string);					// JOR

protected:
	// Exposions
	btCollisionObject* m_pExplosion;
	bool m_bCanExplode;
};