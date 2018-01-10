//#include <GL/freeglut.h>
#include "JoeSceneV2.h"
#include "Audio.h"
#include "Text.h"

// bmp image
//GLuint image = loadBMP_custom("./test.bmp");

// Ch 6.3
JoeSceneV2::JoeSceneV2() : BulletOpenGLApplication(),
	m_bApplyForce(false),
	m_pExplosion(0),
	m_bCanExplode(true),
	m_canFireBall(true),
	m_score(0) {
}

// Initialise physics elements
void JoeSceneV2::InitializePhysics() {
	m_pCollisionConfiguration = new btDefaultCollisionConfiguration();														// Create the collision configuration	
	m_pDispatcher = new btCollisionDispatcher(m_pCollisionConfiguration);													// Create the dispatcher	
	m_pBroadphase = new btDbvtBroadphase();																					// Create the broadphase	
	m_pSolver = new btSequentialImpulseConstraintSolver();																	// Create the constraint solver	
	m_pWorld = new btDiscreteDynamicsWorld(m_pDispatcher, m_pBroadphase, m_pSolver, m_pCollisionConfiguration);				// Create the world

	CreateObjects();																										// Create our scene's physics objects

	InitSDLAudio();																											// Initialise SDL audio when physics done
}

void JoeSceneV2::ShutdownPhysics() {
	delete m_pWorld;
	delete m_pSolver;
	delete m_pBroadphase;
	delete m_pDispatcher;
	delete m_pCollisionConfiguration;
}

// Initialise SDL music and sound effects
void JoeSceneV2::InitSDLAudio() {
	SDL_Init(SDL_INIT_AUDIO);
	Audio::Instance()->loadMediaAudio();																					// Load music & effects, and start playing random track 
}

void JoeSceneV2::CreateObjects() {
		
	CreateGameObject(new btBoxShape(btVector3(1, 50, 50)), 0, btVector3(0.2f, 0.6f, 0.6f), btVector3(0.0f, 0.0f, 0.0f));			// Create a ground plane	
	
	//m_pBox = CreateGameObject(new btBoxShape(btVector3(1, 1, 1)), 1.0, btVector3(1.0f, 0.2f, 0.2f), btVector3(0.0f, 10.0f, 0.0f));// 6.2 - create our red box, but store the pointer for future usage

	/*
	m_pTrigger = new btCollisionObject();																							// 6.2 - create a trigger volume	
	m_pTrigger->setCollisionShape(new btBoxShape(btVector3(1,0.25,1)));																// create a box for the trigger's shape
	// 6.2 - set the trigger's position
	btTransform triggerTrans;
	triggerTrans.setIdentity();
	triggerTrans.setOrigin(btVector3(0,1.5,0));
	m_pTrigger->setWorldTransform(triggerTrans);	
	m_pTrigger->setCollisionFlags(btCollisionObject::CF_NO_CONTACT_RESPONSE);														// 6.2 - flag the trigger to ignore contact responses	
	m_pWorld->addCollisionObject(m_pTrigger);																						// 6.2 - add the trigger to our world
	*/																													
	/*
	// Compound shape: Create two shapes for the rod and the load
	//CreateGameObject(new btCylinderShape(btVector3(1, 1, 1)), 1.0, btVector3(0.0f, 0.7f, 0.0f), btVector3(-2, 10.0f, 0.0f));		// Ch 7.1 - create a green cylinder
	btCollisionShape* pRod = new btBoxShape(btVector3(1.5f, 0.2f, 0.2f));
	btCollisionShape* pLoad = new btSphereShape(0.5f);
	// create a transform we'll use to set each object's position
	btTransform trans;
	trans.setIdentity();
	btCompoundShape* pCompound = new btCompoundShape();																				// Ch 7.3 - Create our compound shape
	// add the rod
	pCompound->addChildShape(trans, pRod);
	trans.setOrigin(btVector3(-1.75f, 0.0f, 0.0f));
	// add the top load
	pCompound->addChildShape(trans, pLoad);
	trans.setIdentity();
	// add the bottom load
	trans.setOrigin(btVector3(1.75f, 0.0f, 0.0f));
	pCompound->addChildShape(trans, pLoad);	
	CreateGameObject(pCompound, 2.0f, btVector3(0.8,0.4,0.1), btVector3(-4, 10.0f, 0.0f));											// Ch 7.3 - Create a game object using the compound shape
	*/
	float xPos = 0, yPos =0;
	/*
	btVector3 positions[4];
	positions[0]  (xPos, yPos, 4);																	// create a btVector3 for the position
	positions[1](xPos, yPos, 28);																	// create a btVector3 for the position
	positions[2](-7, yPos, xPos + 11);																// create a btVector3 for the position
	positions[3](17, yPos, xPos + 11);																// create a btVector3 for the position
	*/


	// create a vertex cloud defining a square-based pyramid (used as top of towers)
	btVector3 points[5] = { btVector3(-0.5,3,3),
		btVector3(-0.5,3,-3),
		btVector3(-0.5,-3,3),
		btVector3(-0.5,-3,-3),
		btVector3(1,0,0) };
	// create our convex hull
	btConvexHullShape* pShape = new btConvexHullShape(&points[0].getX(), 5);
	// initialize the object as a polyhedron
	pShape->initializePolyhedralFeatures();
	// create the game object using our convex hull shape
	CreateGameObject(pShape, 1.0, btVector3(1, 1, 1), btVector3(-8.33, 11, 2.25));
	CreateGameObject(pShape, 1.0, btVector3(1, 1, 1), btVector3(-8.33, 11, 29.25));
	CreateGameObject(pShape, 1.0, btVector3(1, 1, 1), btVector3(18.75, 11, 2.25));
	CreateGameObject(pShape, 1.0, btVector3(1, 1, 1), btVector3(18.75, 11, 29.25));

	// Use stacks of cylinders as towers
	for (int i = 0; i < 11; i++) {
		// create a cylinder
		CreateGameObject(new btCylinderShape(btVector3(2.5, .5, 1)),
			1.0f,																					// Mass
			//btVector3(1.0f, 0.2f, i % 2),															// Alternate colors
			//btVector3(1, 1, 1),																	// Colour
			btVector3(1 - (.1 * (i % 2)), 1 - (.1 * (i % 2)), 1 - (.1 * (i % 2))),
			btVector3(-8.33, i, 2.25),																// Position
			COLGROUP_BOX,																			// Group
			COLGROUP_BOX | COLGROUP_STATIC | COLGROUP_BULLET,										// Mask
			//btQuaternion(btVector3(.5, .5, .5), .5);															
			btQuaternion(0, 1, 0, .25));															// Rotation

		CreateGameObject(new btCylinderShape(btVector3(2.5, .5, 1)),
			1.0f,
			btVector3(1, 1, i%2),
			btVector3(-8.33, i, 29.25),
			COLGROUP_BOX,
			COLGROUP_BOX | COLGROUP_STATIC | COLGROUP_BULLET,
			btQuaternion(0, 1, 0, .25));
		
		CreateGameObject(new btCylinderShape(btVector3(2.5, .5, 1)),
			1.0f,
			//btVector3(i % 2, (i % 2) * .01, i % 2),
			btVector3(1 - (.1 * (i %2)), 1 - (.1 * (i % 2)), 1 - (.1 * (i % 2))),
			btVector3(18.75, i, 2.25),
			COLGROUP_BOX,
			COLGROUP_BOX | COLGROUP_STATIC | COLGROUP_BULLET,
			btQuaternion(0, 1, 0, .25));
			
		CreateGameObject(new btCylinderShape(btVector3(2.5, .5, 1)),
			1.0f,
			btVector3(1, 1, i % 2),
			btVector3(18.75, i, 29.25),
			COLGROUP_BOX,
			COLGROUP_BOX | COLGROUP_STATIC | COLGROUP_BULLET,
			btQuaternion(0, 1, 0, .25));
	}

	// create boxes to make up the four walls
	//float yPos = 0;
	for (int i = 0; i < 55; i++) {
		//float xPos = -5 + 2 * (float)(i % BLOCKS_IN_ROW);														// Calculate x position for the object (5 columns)
		xPos = -5 + 2 * (float)(i % BLOCKS_IN_ROW);																// Calculate x position for the object (5 columns)
		if (i % BLOCKS_IN_ROW == 0) yPos += 2;																	// Calculate y position for the object (5 rows)

		btVector3 position0(xPos, yPos, 4);																		// create a btVector3 for the position
		btVector3 position1(xPos, yPos, 28);																	// create a btVector3 for the position
		btVector3 position2(-7, yPos, xPos + 11);																// create a btVector3 for the position
		btVector3 position3(17, yPos, xPos + 11);																// create a btVector3 for the position	
		
		// create a box
		CreateGameObject(new btBoxShape(btVector3(1, 1, 1)),
			1.0f,
			//btVector3(1.0f, 0.2f, i % 2),																		// alternate colors
			btVector3(0.8f + ((i % 2) * 0.2f), 0.8f + ((i % 2) * 0.2f), 1.0f),
			position0,
			COLGROUP_BOX,
			COLGROUP_BOX | COLGROUP_STATIC | COLGROUP_BULLET);													// boxes can collide with the ground plane and other boxes

		// create a box
		CreateGameObject(new btBoxShape(btVector3(1, 1, 1)),
			1.0f,
			//btVector3(1.0f, 0.2f, i % 2),																		// alternate colors
			btVector3(0.8f + ((i % 2) * 0.2f), 0.8f + ((i % 2) * 0.2f), 1.0f),
			position1,
			COLGROUP_BOX,
			COLGROUP_BOX | COLGROUP_STATIC | COLGROUP_BULLET);													// boxes can collide with the ground plane and other boxes

		// create a box
		CreateGameObject(new btBoxShape(btVector3(1, 1, 1)),
			1.0f,
			btVector3(0.8f + ((i % 2) * 0.2f), 0.8f + ((i % 2) * 0.2f), 1.0f),									// alternate colors
			position2,
			COLGROUP_BOX,
			COLGROUP_BOX | COLGROUP_STATIC | COLGROUP_BULLET);													// boxes can collide with the ground plane and other boxes

		// create a box
		CreateGameObject(new btBoxShape(btVector3(1, 1, 1)),
			1.0f,
			btVector3(0.8f + ((i % 2) * 0.2f), 0.8f + ((i % 2) * 0.2f), 1.0f),									// alternate colors
			position3,
			COLGROUP_BOX,
			COLGROUP_BOX | COLGROUP_STATIC | COLGROUP_BULLET);													// boxes can collide with the ground plane and other boxes

		/*																										// create a sphere in the same location
		CreateGameObject(new btSphereShape(1.2),
			1.0f,
			btVector3(i % 2, 0.0f, 1.0f),																		// alternate colors
			//btVector3(0.9f, 0.9f + ((i%2) * 0.025f), 1.0f),													// alternate colors
			position,
			COLGROUP_SPHERE,
			COLGROUP_SPHERE | COLGROUP_STATIC);																	// spheres can collide with the ground plane and other spheres
		*/


		//GL_TextControl* blah = new GL_TextControl;
		//glRastPos3f(btScalar(20), btScalar(20), btScalar(0));
		//GLDebugDrawString(20, 20, "test");
		//btVector3 rgb(1, 1, 1);
		//GLDebugDrawStringInternal(20, 20, "test", rgb);
		/*
		displayText(10, 10, 1, 1, 1, "test");
		displayText(10, -10, 1, 1, 1, "test");
		displayText(-10, -10, 1, 1, 1, "test");
		displayText(-10, 10, 1, 1, 1, "test");
		*/
	}
}

// Ch 6.2
void JoeSceneV2::CollisionEvent(btRigidBody* pBody0, btRigidBody* pBody1) {
	// Find the two colliding objects
	GameObject* pObj0 = FindGameObject((btRigidBody*)pBody0);
	GameObject* pObj1 = FindGameObject((btRigidBody*)pBody1);
	
	// Impulse testing (needs to be before collisions so it is detected)
	if (pBody0 == m_pExplosion || pBody1 == m_pExplosion) {
		std::cout << "Explosion Detected" << std::endl;

		// get the pointer of the other object
		btRigidBody* pOther;
		pBody0 == m_pExplosion ? pOther = (btRigidBody*)pBody1 : pOther = (btRigidBody*)pBody0;

		pOther->setActivationState(ACTIVE_TAG);																		// wake the object up
																													
		btVector3 dir = pOther->getWorldTransform().getOrigin() - m_pExplosion->getWorldTransform().getOrigin();	// calculate the vector between the object and the center of the explosion
		float dist = dir.length();																					// get the distance		
		float strength = EXPLOSION_STRENGTH;																		// calculate the impulse strength		
		if (dist != 0.0) strength /= dist;																			// follow an inverse-distance rule		
		dir.normalize();																							// normalize the direction vector		
		pOther->applyCentralImpulse(dir * strength);																// apply the impulse
	}

	if (!pObj0 || !pObj1) return;																					// Needs to be after explostion dectection exit if we didn't find anything
	
	if (pObj0->GetType() == PROJECTILE) {
		Audio::Instance()->playFX("gunfireFX");
		DestroyGameObject(pObj0->GetRigidBody());
		incrementScore(10);
	}
	if (pObj1->GetType() == PROJECTILE) {
		Audio::Instance()->playFX("gunfireFX");
		DestroyGameObject(pObj1->GetRigidBody());
		incrementScore(10);
	}
	
	// Create explosion for Ball projectile collision (not working)
	if (pObj0->GetType() == EXPLODING || pObj1->GetType() == EXPLODING) {
		std::cout << "Exploding Object Collision" << std::endl;
		btRigidBody* pExplosionBody;
		//GameObject* pExplosionObj;

		pObj1->GetType() == EXPLODING ? pExplosionBody = (btRigidBody*)pBody1 : pExplosionBody = (btRigidBody*)pBody0;
		//pObj0->GetType() == EXPLODING ? pExplosionObj = pObj0 : pExplosionObj = pObj1;

		Audio::Instance()->playFX("explosionFX");
		//CreateExplosion(pExplosionBody->GetTransform());//
		//CreateExplosion2(pExplosionBody->getWorldTransform());
		//pExplosionBody->getWorldTransform().getOrigin()

		if (m_pExplosion || !m_bCanExplode) {
			std::cout << "Can't create explosion " << std::endl;
			//return;
		}

		CreateExplosion(pExplosionBody->getWorldTransform().getOrigin());
		//CreateExplosion(btVector3(2.5, .5, 1));

		/*
		std::cout << "Create Explosion X: " <<
			pExplosionBody->getWorldTransform().getOrigin().getX() <<
			" Y: " << pExplosionBody->getWorldTransform().getOrigin().getY() <<
			" Z: " << pExplosionBody->getWorldTransform().getOrigin().getZ() << std::endl;
		*/

		//m_bCanExplode = true; 
		m_canFireBall = true;

		DestroyGameObject(pExplosionBody);

		incrementScore(50);
	}
/*
		//DestroyGameObject(pExplosionObj->GetRigidBody());
		//DestroyGameObject(pExplosionBody);

		//std::cout << "Exploding Object Destroyed" << std::endl;

		if (m_pExplosion || !m_bCanExplode) return;																// don't create a new explosion if one already exists or we haven't released the key, yet		

		std::cout << "Can create explosion" << std::endl;

		m_bCanExplode = false;																					// don't let us create another explosion until the key is released																				

		// create a collision object for our explosion
		m_pExplosion = new btCollisionObject();
		m_pExplosion->setCollisionShape(new btSphereShape(3.0f));

		// get the position that we clicked
		//RayResult result;
		//Raycast(m_cameraPosition, GetPickingRay(x, y), result, true);

		// create a transform from the hit point
		btTransform explodeTrans;
		explodeTrans.setIdentity();
		//explodeTrans.setOrigin(result.hitPoint);
		//explodeTrans.setOrigin();
		//btScalar transform[16];																					// create an array of 16 floats (representing a 4x4 matrix)
		//pExplosionBody->GetTransform(transform)
		//explodeTrans.setOrigin(pExplosionBody->GetTransform());		
		explodeTrans.setOrigin(pExplosionBody->getCenterOfMassPosition());

		std::cout << "test x: " << pExplosionBody->getCenterOfMassPosition().getX() << std::endl;
		std::cout << "test y: " << pExplosionBody->getCenterOfMassPosition().getY() << std::endl;
		std::cout << "test z: " << pExplosionBody->getCenterOfMassPosition().getZ() << std::endl;


		m_pExplosion->setWorldTransform(explodeTrans);

		m_pExplosion->setCollisionFlags(btCollisionObject::CF_NO_CONTACT_RESPONSE);								// set the collision flag		
		m_pWorld->addCollisionObject(m_pExplosion);																// add the explosion trigger to our world

		DestroyGameObject(pExplosionBody);
		std::cout << "Exploding Object Destroyed" << std::endl;
		std::cout << "Explosion complete" << std::endl;
	}
	*/
	/*
	// did the box collide with the trigger?
	if (pBody0 == m_pBox->GetRigidBody() && pBody1 == m_pTrigger ||
		pBody1 == m_pBox->GetRigidBody() && pBody0 == m_pTrigger) {			
			CreateGameObject(new btBoxShape(btVector3(2,2,2)), 2.0, btVector3(0.3, 0.7, 0.3), btVector3(5, 10, 0));	// if yes, create a big green box nearby
	}
	*/
}

// Ch 6.3
void JoeSceneV2::Keyboard(unsigned char key, int x, int y) {
	// call the base implementation first
	BulletOpenGLApplication::Keyboard(key, x, y);																	// Call parent function to handle z,x,w,b,d key presses

	switch(key) {
		// Force testing
		case 'g': {		
			m_bApplyForce = true;																					// if 'g' is held down, apply a force		
			m_pBox->GetRigidBody()->setActivationState(DISABLE_DEACTIVATION);										// prevent the box from deactivating
			break;
		}
		// Impulse testing
		case 'e': {		
			if (m_pExplosion || !m_bCanExplode) break;																// don't create a new explosion if one already exists or we haven't released the key, yet		
			m_bCanExplode = false;																					// don't let us create another explosion until the key is released																				

			// create a collision object for our explosion
			//m_pExplosion = new btCollisionObject();
			//m_pExplosion->setCollisionShape(new btSphereShape(3.0f));

			// get the position that we clicked
			RayResult result;
			Raycast(m_cameraPosition, GetPickingRay(x, y), result, true);

			CreateExplosion(result.hitPoint);																		// JOR

			// create a transform from the hit point
			//btTransform explodeTrans;
			//explodeTrans.setIdentity();
			//explodeTrans.setOrigin(result.hitPoint);
			//m_pExplosion->setWorldTransform(explodeTrans);		
			//m_pExplosion->setCollisionFlags(btCollisionObject::CF_NO_CONTACT_RESPONSE);							// set the collision flag		
			//m_pWorld->addCollisionObject(m_pExplosion);															// add the explosion trigger to our world
			break;
		}
		// Audio 
		case 'p': Audio::Instance()->playPauseMusic(); break;														// JOR Pause or play the music
		case '.': Audio::Instance()->musicForward(); break;															// JOR Skip music track backwards
		case ',': Audio::Instance()->musicBack(); break;															// JOR Skip music track forwards
	}
}
/*
void JoeSceneV2::CreateExplosion2(btTransform &transform) {
	m_pExplosion = new btCollisionObject();																			// create a collision object for our explosion
	m_pExplosion->setCollisionShape(new btSphereShape(3.0f));

	// get the position that we clicked
	//RayResult result;
	//Raycast(m_cameraPosition, GetPickingRay(x, y), result, true);

	// create a transform from the hit point
	btTransform explodeTrans;
	//explodeTrans.setIdentity();
	//explodeTrans.setOrigin(result.hitPoint);
	//explodeTrans.setOrigin(origin);
	m_pExplosion->setWorldTransform(transform);

	m_pExplosion->setCollisionFlags(btCollisionObject::CF_NO_CONTACT_RESPONSE);								// set the collision flag		
	m_pWorld->addCollisionObject(m_pExplosion);																// add the explosion trigger to our world
}
*/
void JoeSceneV2::CreateExplosion(const btVector3 &origin) {
	std::cout << "Create Explosion()\n X: " << origin.getX() << " Y: " << origin.getY() << " Z: " << origin.getZ() << std::endl;
	
	m_pExplosion = new btCollisionObject();																			// create a collision object for our explosion
	m_pExplosion->setCollisionShape(new btSphereShape(3.0f));

	if (m_pExplosion) std::cout << "Explosion Created" << std::endl;

	// create a transform from the hit point
	btTransform explodeTrans;
	explodeTrans.setIdentity();
	explodeTrans.setOrigin(origin);
	m_pExplosion->setWorldTransform(explodeTrans);

	m_pExplosion->setCollisionFlags(btCollisionObject::CF_NO_CONTACT_RESPONSE);										// set the collision flag		
	m_pWorld->addCollisionObject(m_pExplosion);																		// add the explosion trigger to our world
}

// Ch 6.3
void JoeSceneV2::KeyboardUp(unsigned char key, int x, int y) {
	// call the base implementation first
	BulletOpenGLApplication::KeyboardUp(key, x, y);
	switch(key) {
	// Force testing
	case 'g': {			
			m_bApplyForce = false;																					// if 'g' is let go, stop applying the force			
			m_pBox->GetRigidBody()->forceActivationState(ACTIVE_TAG);												// allow the object to deactivate again
			break;
		}
	case 'e': m_bCanExplode = true; break;																			// Impulse testing
	}
}

// Override Mouse() function. Added middle and right button projectile firing
void JoeSceneV2::Mouse(int button, int state, int x, int y) {
	BulletOpenGLApplication::Mouse(button, state, x, y);										// Call parent function

	switch (button) {
		// Middle button fires ball
		case 1: 
			if (state == 0) {
				std::cout << "Middle Button Pressed" << std::endl;
				if (m_canFireBall) {
					Audio::Instance()->playFX("swoosh2FX");										// JOR Play swoosh sound 2 from map using ID 
					// Audio::Instance()->Fire2();												// JOR Play swoosh sound 2
					//if (m_pExplosion || !m_bCanExplode) break;
					ShootBall(GetPickingRay(x, y));												// JOR Middle button pressed, shoot a sphere
					m_canFireBall = false;
				} 
				break;
			} else {
				std::cout << "Middle Button Not Pressed" << std::endl;
				//m_bCanExplode = true;														// Create another explosion when the key is released	
				break;
			}
		// Right button fires arrow
		case 2: 
			if (state == 0) {
				std::cout << "Right Button Pressed" << std::endl;
				Audio::Instance()->playFX("swoosh1FX");											// JOR Play swoosh sound 1 from map using ID 
				//Audio::Instance()->Fire1();													// JOR Play swoosh sound 1
				//ShootArrow(GetPickingRay(x, y)); break;										// JOR Right mouse button pressed, shoot arrow
				ShootArrowCompound(GetPickingRay(x, y)); break;									// JOR Right mouse button pressed, shoot arrow
			} else {
				std::cout << "Right Button Not Pressed" << std::endl; break;
			}
	}
}

void JoeSceneV2::RenderScene() {
	BulletOpenGLApplication::RenderScene();														// Call the base implementation first
	//int score = 0;
	char scoreText[20];
	//snprintf(scoreText, sizeof(scoreText), "Score: %d", score);
	snprintf(scoreText, sizeof(scoreText), "Score: %d", m_score);

	char* test = "Joe O'Regan K00203642";

	//Text::Instance()->PrintText(20, 20, "Score: ");
	Text::Instance()->PrintText(20, 20, scoreText);
	Text::Instance()->PrintText(m_screenWidth-240, 20, "Joe O'Regan K00203642");
}

// Overrides BulletOpenGLApplication UpdateScene()
void JoeSceneV2::UpdateScene(float dt) {
	BulletOpenGLApplication::UpdateScene(dt);													// Call the base implementation first


	Text::Instance()->DisplayText(10, 10, 1, 1, 1, "Hit The Castle");							// Access text function using singleton
	
	// Force testing
	if (m_bApplyForce) {
		if (!m_pBox) return;		
		m_pBox->GetRigidBody()->applyCentralForce(btVector3(0, 20, 0));							// apply a central upwards force that exceeds gravity
	}

	/*
	if (m_pExplosion && !m_bCanExplode)
	{
		std::cout << "Active Explosion" << std::endl;
	}
	*/

	// Impulse testing
	if (m_pExplosion && !m_bCanExplode) {
		std::cout << "Active Explosion Removed From World" << std::endl;
		// destroy the explosion object after one iteration
		m_pWorld->removeCollisionObject(m_pExplosion);
		delete m_pExplosion;
		m_pExplosion = 0;
		m_bCanExplode = true;
	}

	//if (!m_bCanExplode) std::cout << "test2" << std::endl;


	//Text::Instance()->PrintText(20, 20, "Test Text");
}

/* PROJECTILES */

// Ball projectile fires from camera position, when middle mouse button pressed
void JoeSceneV2::ShootBall(const btVector3 &direction) {

	// create game object (shape, mass, colour, initial position, group, mask, rotation)

	//GameObject* pObject = CreateGameObject(new btBoxShape(btVector3(.5, .5, .5)), 1, btVector3(0.4f, 0.f, 0.4f), m_cameraPosition);	// create a new box object
	GameObject* pObject = CreateGameObject(new btSphereShape(0.5),
		1.0f,																															// Mass
		btVector3(1.0f, 1.0f, 0.0f),																									// Colour
		m_cameraPosition, 																												// Start at Cameras position
		COLGROUP_BULLET,																												// Collision groups are box and bullet
		COLGROUP_BOX);

	pObject->SetPlayAudio(false);
	//pObject->SetName("bullet");
	pObject->SetType(EXPLODING);																										// JOR Set the type as bullet

	btVector3 velocity = direction;																										// calculate the velocity
	velocity.normalize();
	velocity *= 25.0f;

	pObject->GetRigidBody()->setLinearVelocity(velocity);																				// set the linear velocity of the box

	/*																																		// Create explosion for Ball projectile
	//if (pObj0->GetType() == EXPLODING || pObj1->GetType() == EXPLODING) {
		std::cout << "Exploding Object Collision" << std::endl;
		btRigidBody* pExplosionBody;
		//GameObject* pExplosionObj;

		//pObj1->GetType() == EXPLODING ? pExplosionBody = (btRigidBody*)pBody1 : pExplosionBody = (btRigidBody*)pBody0;
		//pObj0->GetType() == EXPLODING ? pExplosionObj = pObj0 : pExplosionObj = pObj1;

		Audio::Instance()->playFX("gunfireFX");
		//DestroyGameObject(pExplosionObj->GetRigidBody());
		//DestroyGameObject(pExplosionBody);

		//std::cout << "Exploding Object Destroyed" << std::endl;

		if (m_pExplosion || !m_bCanExplode) return;																// don't create a new explosion if one already exists or we haven't released the key, yet		

		std::cout << "Can create explosion" << std::endl;

		m_bCanExplode = false;																					// don't let us create another explosion until the key is released																				

		// create a collision object for our explosion
		m_pExplosion = new btCollisionObject();
		m_pExplosion->setCollisionShape(new btSphereShape(3.0f));

		// get the position that we clicked
		//RayResult result;
		//Raycast(m_cameraPosition, GetPickingRay(x, y), result, true);

		
		btTransform explodeTrans;																				// create a transform from the hit point
		explodeTrans.setIdentity();
		explodeTrans.setOrigin(pExplosionBody->getCenterOfMassPosition());
		//explodeTrans.setOrigin(result.hitPoint);
		//explodeTrans.setOrigin();
		//btScalar transform[16];																				// create an array of 16 floats (representing a 4x4 matrix)
		//pExplosionBody->GetTransform(transform)
		//explodeTrans.setOrigin(pExplosionBody->GetTransform());		

		std::cout << "test x: " << pExplosionBody->getCenterOfMassPosition().getX() << std::endl;
		std::cout << "test y: " << pExplosionBody->getCenterOfMassPosition().getY() << std::endl;
		std::cout << "test z: " << pExplosionBody->getCenterOfMassPosition().getZ() << std::endl;


		m_pExplosion->setWorldTransform(explodeTrans);

		m_pExplosion->setCollisionFlags(btCollisionObject::CF_NO_CONTACT_RESPONSE);								// set the collision flag		
		m_pWorld->addCollisionObject(m_pExplosion);																// add the explosion trigger to our world

		DestroyGameObject(pExplosionBody);
		std::cout << "Exploding Object Destroyed" << std::endl;
		std::cout << "Explosion complete" << std::endl;
	//}	
	*/
}

#define RADIANS_PER_DEGREE 0.01745329f	

// Compound shape projectile
// Arrow projectile fires from camera position, when right mouse button pressed
void JoeSceneV2::ShootArrowCompound(const btVector3 &direction) {
	// Arrowhead
	btVector3 points[5] = { btVector3(-0.25,0.25,0.25),
		btVector3(-0.25,0.25,-0.25),
		btVector3(-0.25,-0.25,0.25),
		btVector3(-0.25,-0.25,-0.25),
		btVector3(.75,0,0) };
	// create our convex hull
	btConvexHullShape* pShape = new btConvexHullShape(&points[0].getX(), 5);
	// initialize the object as a polyhedron
	pShape->initializePolyhedralFeatures();

	btCollisionShape* head = pShape;
	btCollisionShape* shaft = new btCylinderShape(btVector3(.1, 2, 1));
	//btCollisionShape* flight1 = new btBoxShape(btVector3(0.1, 0.05, 0.15));
	btCollisionShape* flight1 = new btBoxShape(btVector3(0.05, 0.15, 0.1));
	btCollisionShape* flight2 = new btBoxShape(btVector3(0.05, 0.15, 0.1));
	btCollisionShape* flight3 = new btBoxShape(btVector3(0.1, 0.15, 0.05));
	btCollisionShape* flight4 = new btBoxShape(btVector3(0.1, 0.15, 0.05));

	// create a transform we'll use to set each object's position
	btTransform trans;
	trans.setIdentity();
	btCompoundShape* pCompound = new btCompoundShape();																				// Ch 7.3 - Create our compound shape
	trans.setRotation(btQuaternion(0, 1, 0, -1));
	pCompound->addChildShape(trans, head);
	trans.setRotation(btQuaternion(1, 0, 0, -1));
	trans.setOrigin(-btVector3(0, 0, 2));
	pCompound->addChildShape(trans, shaft);
	trans.setOrigin(-btVector3(0, -0.2, 3.75));																						// add the rod
	pCompound->addChildShape(trans, flight1);
	trans.setOrigin(-btVector3(0, 0.2, 3.75));
	pCompound->addChildShape(trans, flight2);
	trans.setOrigin(-btVector3(-0.2, 0, 3.75));																						// add the rod
	pCompound->addChildShape(trans, flight3);
	trans.setOrigin(-btVector3(0.2, 0, 3.75));
	pCompound->addChildShape(trans, flight4);

	trans.setIdentity();

	// create game object (shape, mass, colour, initial position, group, mask, rotation)
	//	GameObject* arrow = CreateGameObject(pCompound, 2.0f, btVector3(0.8, 0.4, 0.1), m_cameraPosition);								// Create a game object using the compound shape
	GameObject* arrow = CreateGameObject(pCompound, 2.0f,
		btVector3(0.8, 0.4, 0.1),
		m_cameraPosition,
		COLGROUP_BULLET,
		COLGROUP_BOX | COLGROUP_STATIC,
		btQuaternion(0, 0, 0, 1));
	//btQuaternion(0, 1, 0, m_cameraYaw * RADIANS_PER_DEGREE));
	//btQuaternion(0, 0, 0, 1) - btQuaternion(0, 1, 0, m_cameraYaw * RADIANS_PER_DEGREE));

	//btQuaternion(0, 0, 1, 1 * (m_cameraYaw / 360)));
	//btQuaternion(1, 1, 0, 0));
	//btQuaternion(1 * m_cameraPitch / 360, 1 * m_cameraYaw / 360, 0, 1));
	//btQuaternion(0, 1, 0, 1) + btQuaternion(0, m_cameraYaw / 360, 0, 1));
	//btQuaternion(0, 1 * m_cameraYaw / 360, 0, -1));
	//btQuaternion(m_cameraPitch, m_cameraYaw, 0,1));
	//btQuaternion(1 + m_cameraPitch, 0, 0, 1));
	//btQuaternion(1, 0, 0, m_cameraPitch) + btQuaternion(0, 0, 1, m_cameraYaw));
	//btQuaternion(1,0,0, -1) + btQuaternion(m_cameraPitch, m_cameraYaw,0,1));

	std::cout << "Pitch : " << m_cameraPitch << " Yaw: " << m_cameraYaw << std::endl;

	btVector3 velocity = direction;																									// calculate the velocity
	velocity.normalize();
	velocity *= 25.0f;

	arrow->SetType(PROJECTILE);																											// Lets collision audio play
	arrow->GetRigidBody()->setLinearVelocity(velocity);																				// set the linear velocity of the arrow shaft
}

// Bunch of shapes fired together and hoping for the best
void JoeSceneV2::ShootArrow(const btVector3 &direction) {

	GameObject* pBox1 = CreateGameObject(new btBoxShape(btVector3(0.1, 0.05, 0.15)), 1, btVector3(0.4f, 0.f, 0.4f), m_cameraPosition - btVector3(0, -0.2, 3.75));	// create a new box object
	GameObject* pBox2 = CreateGameObject(new btBoxShape(btVector3(0.1, 0.05, 0.15)), 1, btVector3(0.4f, 0.f, 0.4f), m_cameraPosition - btVector3(0, 0.2, 3.75));	// create a new box object
	GameObject* pBox3 = CreateGameObject(new btBoxShape(btVector3(0.05, 0.1, 0.15)), 1, btVector3(0.4f, 0.f, 0.4f), m_cameraPosition - btVector3(-0.2, 0, 3.75));	// create a new box object
	GameObject* pBox4 = CreateGameObject(new btBoxShape(btVector3(0.05, 0.1, 0.15)), 1, btVector3(0.4f, 0.f, 0.4f), m_cameraPosition - btVector3(0.2, 0, 3.75));	// create a new box object
																																									// Arrowhead
	btVector3 points[5] = { btVector3(-0.25,0.25,0.25),
		btVector3(-0.25,0.25,-0.25),
		btVector3(-0.25,-0.25,0.25),
		btVector3(-0.25,-0.25,-0.25),
		btVector3(.75,0,0) };
	// create our convex hull
	btConvexHullShape* pShape = new btConvexHullShape(&points[0].getX(), 5);
	// initialize the object as a polyhedron
	pShape->initializePolyhedralFeatures();

	// create the game object using our convex hull shape
	GameObject* pObject = CreateGameObject(pShape, 1.0, btVector3(1, 1, 1), m_cameraPosition, COLGROUP_BULLET, COLGROUP_BOX, btQuaternion(0, 1, 0, -1));


	// Shaft = cylinder
	GameObject* pObject2 = CreateGameObject(new btCylinderShape(btVector3(.1, 2, 1)),
		1.0f,
		btVector3(1, 1, 1),
		m_cameraPosition - btVector3(0, 0, 2),
		COLGROUP_BULLET,
		COLGROUP_BOX | COLGROUP_STATIC,
		btQuaternion(1, 0, 0, -1));
	//btQuaternion(m_cameraPitch, m_cameraYaw, 0, 1));
	//btQuaternion(1 + m_cameraPitch, 0, 0, ));
	//btQuaternion(1,0,0, -1) + btQuaternion(m_cameraPitch, m_cameraYaw,0,1));

	pObject->SetPlayAudio(false);
	//pObject->SetName("bullet");
	pObject->SetType(PROJECTILE);																											// JOR Set the type as bullet

	btVector3 velocity = direction;																										// calculate the velocity
	velocity.normalize();
	velocity *= 25.0f;

	pBox1->GetRigidBody()->setLinearVelocity(velocity);																					// set the linear velocity of the arrowhead
	pBox2->GetRigidBody()->setLinearVelocity(velocity);																					// set the linear velocity of the arrowhead
	pBox3->GetRigidBody()->setLinearVelocity(velocity);																					// set the linear velocity of the arrowhead
	pBox4->GetRigidBody()->setLinearVelocity(velocity);																					// set the linear velocity of the arrowhead
	pObject->GetRigidBody()->setLinearVelocity(velocity);																				// set the linear velocity of the arrowhead
	pObject2->GetRigidBody()->setLinearVelocity(velocity);																				// set the linear velocity of the arrow shaft
}
