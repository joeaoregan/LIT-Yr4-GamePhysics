// Added Ch 8

#include "BulletOpenGLApplication.h"

#include "btBulletDynamicsCommon.h"

class CollisionFilteringDemo : public BulletOpenGLApplication {
public:
	CollisionFilteringDemo();

	virtual void InitializePhysics() override;
	virtual void ShutdownPhysics() override;
	void CreateObjects();
};