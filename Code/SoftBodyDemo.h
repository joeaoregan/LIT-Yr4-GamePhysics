// NOTE
// Three extra steps were taken to configure this project:
//   1) The BulletSoftBody project was included
//	 2) The above project was set as a dependency
//   3) BulletSoftBody_vs2010_debug.lib was added to the list of required .lib files (Linker->Input->Additional Dependencies).
// All of the above are required for the simulation of soft bodies.

#include "BulletOpenGLApplication.h"

// includes required for soft body simulation
#include "BulletSoftBody/btSoftRigidDynamicsWorld.h"
#include "BulletSoftBody/btSoftBodyRigidBodyCollisionConfiguration.h"
#include "BulletSoftBody/btSoftBodyHelpers.h"

class SoftBodyDemo : public BulletOpenGLApplication {
public:
	SoftBodyDemo();

	virtual void InitializePhysics() override;
	virtual void ShutdownPhysics() override;
	virtual void RenderScene() override;
	void CreateObjects();

protected:	
	btSoftRigidDynamicsWorld*  m_pSoftBodyWorld;	// a pointer to our world, typecast into its soft body type	
	btSoftBodyWorldInfo  m_softBodyWorldInfo;		// the soft body world info. Needed for proper contact generation
};
