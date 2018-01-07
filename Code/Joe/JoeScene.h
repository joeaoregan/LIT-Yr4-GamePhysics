// Added Ch 8

#include "../BulletOpenGLApplication.h"
//#include "Audio.h"

#define SDL_MAIN_HANDLED	// JOR Handling SDL main definition ourselves

#include "btBulletDynamicsCommon.h"

//#include "Audio.h"


class JoeScene : public BulletOpenGLApplication {
public:
	JoeScene();

	virtual void InitializePhysics() override;
	virtual void ShutdownPhysics() override;
	void CreateObjects();
};