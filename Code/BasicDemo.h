#include "BulletOpenGLApplication.h"
#include "btBulletDynamicsCommon.h"

#define EXPLOSION_STRENGTH 50.0f													// Ch 6.3

class BasicDemo : public BulletOpenGLApplication {
public:
	BasicDemo();																	// Ch 6.3

	virtual void InitializePhysics() override;
	virtual void ShutdownPhysics() override;

	void CreateObjects();

	virtual void Keyboard(unsigned char key, int x, int y) override;				// Ch 6.3
	virtual void KeyboardUp(unsigned char key, int x, int y) override;				// Ch 6.3
	virtual void UpdateScene(float dt);

	virtual void CollisionEvent(btRigidBody* pBody0, btRigidBody* pBody1) override;	// Ch 6.2

protected:	
	GameObject* m_pBox;																// Ch 6.2 - our box to lift	
	btCollisionObject* m_pTrigger;													// Ch 6.2 - a simple trigger volume

	bool m_bApplyForce;																// Ch 6.3 - keeps track of whether we're holding down the 'g' key

	// ch 6.3 - explosion variables
	btCollisionObject* m_pExplosion;									
	bool m_bCanExplode;
};