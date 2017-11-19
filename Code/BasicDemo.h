#include "BulletOpenGLApplication.h"
#include "btBulletDynamicsCommon.h"

class BasicDemo : public BulletOpenGLApplication {
public:
	virtual void InitializePhysics() override;
	virtual void ShutdownPhysics() override;

	void CreateObjects();

	virtual void CollisionEvent(btRigidBody* pBody0, btRigidBody* pBody1) override;	// Ch 6.2

protected:	
	GameObject* m_pBox;																// Ch 6.2 - our box to lift	
	btCollisionObject* m_pTrigger;													// Ch 6.2 - a simple trigger volume
};