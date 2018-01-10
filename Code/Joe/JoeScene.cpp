//#include <SDL.h>
#include "Audio.h"
//#include <SDL_image.h>
//#include "SDL_ttf.h"
#include "JoeScene.h"
#include "Text.h"
#include <iostream>

void InitSDLAudio();
//bool InitSDL();

//JoeScene::JoeScene() : BulletOpenGLApplication() {}
JoeScene::JoeScene() : BulletOpenGLApplication(),
	m_pExplosion(0),		// set m_pExplosion collision object to null
	m_bCanExplode(true),	// Set can explode initially to true
	m_score(0) {}			// Set m_score initial value to 0

void JoeScene::UpdateScene(float dt) {	
	BulletOpenGLApplication::UpdateScene(dt);												// Call the base implementation first

	//GL_TextControl* blah = new GL_TextControl;
	//glRastPos3f(btScalar(20), btScalar(20), btScalar(0));
	//GLDebugDrawString(20, 20, "test");
	//btVector3 rgb(1, 1, 1);
	//GLDebugDrawStringInternal(20, 20, "test", rgb);

	//std::cout << "Test Display" << std::endl;	// Doesn't repeat v.often
	//Displays in scene space, not window
	//displayText(10, 10, 1, 1, 1, "Hit The Castle");	// Works
	//displayText(0, 11, 0, 0, 0, "Hit The Castle");

	Text::Instance()->DisplayText(10, 10, 1, 1, 1, "Hit The Castle");						// Access text function using singleton

	/*
	DisplayString(10, 10, "test2");
	DisplayString(10, -10, "test2");
	DisplayString(-10, 10, "test2");
	DisplayString(-10, -10, "test2");
	*/
	//displayProfileString(0, 0, "test");
	//displayText(10, -10, 1, 1, 1, "test");
	//displayText(-10, -10, 1, 1, 1, "test");
	//displayText(-10, 10, 1, 1, 1, "test");
	/*
	if (m_pWorld) {																			// check if the world object exists		
		m_pWorld->stepSimulation(dt);														// step the simulation through time. This is called every update and the amount of elasped time was determined back in ::Idle() by our clock object.
	}

	CheckForCollisionEvents();																// Ch 6. 1 - Check for any new collisions/serparations
	*/
	
	// Impulse testing
	if (m_pExplosion && !m_bCanExplode) {
		std::cout << "Active Explosion Removed From World" << std::endl;
	//if (m_pExplosion) {
		//std::cout << "Explosion" << std::endl;

		// destroy the explosion object after one iteration
		m_pWorld->removeCollisionObject(m_pExplosion);
		delete m_pExplosion;
		m_pExplosion = 0;
		m_bCanExplode = true;
	}	

	//if (m_pExplosion->getWorldTransform().getOrigin().getY() == 1.0f)
	//	m_pWorld->removeCollisionObject(m_pExplosion);

	// Check the game object list for the ball, and destroy if out of the scene
	for (GameObjects::iterator iter = m_objects.begin(); iter != m_objects.end(); ++iter) {
		//if ((*iter)->GetType() == EXPLODING && (*iter)->GetRigidBody().getWorldTransform().GetY() < 2) DestroyGameObject((*iter)->GetRigidBody());	// found the body, so return the corresponding game object
		if ((*iter)->GetType() == EXPLODING)
		{
			std::cout << "Ball Projectile In Game Objects List" << std::endl;
			if ((*iter)->GetRigidBody()->getWorldTransform().getOrigin().getY() < 0) {
				std::cout << "Ball Projectile Y < 0" << std::endl;
				//DestroyGameObject((*iter)->GetRigidBody());									// Destroy the ball if it goes out of the scene GIVES ERROR
			}
		}
	}
}

void JoeScene::InitializePhysics() {
	m_pCollisionConfiguration = new btDefaultCollisionConfiguration();											// create the collision configuration	
	m_pDispatcher = new btCollisionDispatcher(m_pCollisionConfiguration);										// create the dispatcher	
	m_pBroadphase = new btDbvtBroadphase();																		// create the broadphase	
	m_pSolver = new btSequentialImpulseConstraintSolver();														// create the constraint solver	
	m_pWorld = new btDiscreteDynamicsWorld(m_pDispatcher, m_pBroadphase, m_pSolver, m_pCollisionConfiguration);	// create the world
	CreateObjects();
	
	InitSDLAudio();
	Audio::Instance()->loadMediaAudio();

	//Audio::Instance()->playFX("chainsawFX");																	// Test effect plays
}

void InitSDLAudio() {
	//InitSDL();
	SDL_Init(SDL_INIT_AUDIO);
	//Mix_OpenAudio(22050, AUDIO_S16, 2, (4096 / 2));															// Initialise the mixer API

	/*
	gMusic = Mix_LoadMUS("Music/joe_riff3.wav"); 
	if (gMusic == NULL) {
		printf("Failed to load music! SDL_mixer Error: %s\n", Mix_GetError());
	}

	gSound = Mix_LoadWAV("Audio/ChainsawRun.wav");
	Mix_PlayMusic(gMusic, -1);
	*/
}

/*
bool InitSDL() {
	

	// attempt to initialise SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
		int imgFlags = IMG_INIT_PNG;																	// Initialize PNG loading
		if (!(IMG_Init(imgFlags) & imgFlags)) {
			printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
			//success = false;
		}

		//Initialize SDL_ttf
		if (TTF_Init() == -1) {
			printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
			return false;
		}
	}
	
	return true;
}
*/
void JoeScene::ShutdownPhysics() {
	delete m_pWorld;
	delete m_pSolver;
	delete m_pBroadphase;
	delete m_pDispatcher;
	delete m_pCollisionConfiguration;
}

void JoeScene::CreateObjects() {
	// create a ground plane
	GameObject* ground = CreateGameObject(new btBoxShape(btVector3(1, 50, 50)),
		0, btVector3(0.2f, 0.9f, 0.6f), btVector3(0.0f, 0.0f, 0.0f),
		//COLGROUP_STATIC, COLGROUP_BOX | COLGROUP_SPHERE);											// The ground plane can collide with boxes, spheres and bullets (which are boxes and spheres)
		COLGROUP_STATIC, 
		COLGROUP_BOX | COLGROUP_SPHERE | COLGROUP_BULLET);											// The ground plane can collide with boxes, spheres and bullets (which are boxes and spheres)

	ground->SetType(GROUND);																		// Check for collisions, masks and groups are not as easy to access

	CreateLetters();

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
		//float xPos = -5 + 2 * (float)(i % BLOCKS_IN_ROW);											// Calculate x position for the object (5 columns)
		xPos = -5 + 2 * (float)(i % BLOCKS_IN_ROW);													// Calculate x position for the object (5 columns)
		if (i % BLOCKS_IN_ROW == 0) yPos += 2;														// Calculate y position for the object (5 rows)

		btVector3 position0(xPos, yPos, 4);															// create a btVector3 for the position
		btVector3 position1(xPos, yPos, 28);														// create a btVector3 for the position
		btVector3 position2(-7, yPos, xPos + 11);													// create a btVector3 for the position
		btVector3 position3(17, yPos, xPos + 11);													// create a btVector3 for the position	
		
		// create a box
		CreateGameObject(new btBoxShape(btVector3(1, 1, 1)),
			1.0f,
			//btVector3(1.0f, 0.2f, i % 2),															// alternate colors
			btVector3(0.8f + ((i % 2) * 0.2f), 0.8f + ((i % 2) * 0.2f), 1.0f),
			position0,
			COLGROUP_BOX,
			COLGROUP_BOX | COLGROUP_STATIC | COLGROUP_BULLET);										// boxes can collide with the ground plane and other boxes

		// create a box
		CreateGameObject(new btBoxShape(btVector3(1, 1, 1)),
			1.0f,
			//btVector3(1.0f, 0.2f, i % 2),															// alternate colors
			btVector3(0.8f + ((i % 2) * 0.2f), 0.8f + ((i % 2) * 0.2f), 1.0f),
			position1,
			COLGROUP_BOX,
			COLGROUP_BOX | COLGROUP_STATIC | COLGROUP_BULLET);										// boxes can collide with the ground plane and other boxes

		// create a box
		CreateGameObject(new btBoxShape(btVector3(1, 1, 1)),
			1.0f,
			btVector3(0.8f + ((i % 2) * 0.2f), 0.8f + ((i % 2) * 0.2f), 1.0f),						// alternate colors
			position2,
			COLGROUP_BOX,
			COLGROUP_BOX | COLGROUP_STATIC | COLGROUP_BULLET);										// boxes can collide with the ground plane and other boxes

		// create a box
		CreateGameObject(new btBoxShape(btVector3(1, 1, 1)),
			1.0f,
			btVector3(0.8f + ((i % 2) * 0.2f), 0.8f + ((i % 2) * 0.2f), 1.0f),						// alternate colors
			position3,
			COLGROUP_BOX,
			COLGROUP_BOX | COLGROUP_STATIC | COLGROUP_BULLET);										// boxes can collide with the ground plane and other boxes

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

// Spinning letter objects with torque applied
void JoeScene::CreateLetters() {	
	// Compound shape: Create two shapes for the rod and the load
	btCollisionShape* obj1 = new btBoxShape(btVector3(1.5f, 0.2f, 0.2f));
	btCollisionShape* obj2 = new btBoxShape(btVector3(0.2f, 1.0f, 0.2f));
	btCollisionShape* obj3 = new btBoxShape(btVector3(0.2f, 0.5f, 0.2f));
	btCollisionShape* pball = new btSphereShape(0.5f);
	
	// create a transform we'll use to set each object's position
	btTransform trans1, trans2, trans3;
	trans1.setIdentity();
	btCompoundShape* pCompound = new btCompoundShape();												// Create a compound shape
	// add the parts
	pCompound->addChildShape(trans1, obj1);
	trans1.setOrigin(btVector3(-1.75f, 0.0f, 0.0f));
	pCompound->addChildShape(trans1, pball);
	trans1.setOrigin(btVector3(-1.25f, 0.5f, 0.0f));
	pCompound->addChildShape(trans1, obj3);
	trans1.setOrigin(btVector3(1.25f, 0.0f, 0.0f));
	pCompound->addChildShape(trans1, obj2);
	trans1.setIdentity();
	trans1.setOrigin(btVector3(1.75f, 0.0f, 0.0f));
	pCompound->addChildShape(trans1, pball);

	GameObject* letterJ = CreateGameObject(pCompound, 2.0f, btVector3(0.9, 0.1, 0.1), btVector3(5.0f, 2.0f, -2.0f));	// Ch 7.3 - Create a game object using the compound shape
	//letterJ->GetRigidBody();
		
	// Letter 0
	trans2.setIdentity();
	btCompoundShape* pCompoundO = new btCompoundShape();												// Create a compound shape
	// add the parts
	pCompoundO->addChildShape(trans2, obj1);
	trans2.setOrigin(btVector3(-1.75f, 0.5f, 0.0f));
	pCompoundO->addChildShape(trans2, pball);
	trans2.setIdentity();
	trans2.setOrigin(btVector3(1.75f, 0.5f, 0.0f));
	pCompoundO->addChildShape(trans2, pball);
	trans2.setOrigin(btVector3(0.0f, 1.0f, 0.0f));
	pCompoundO->addChildShape(trans2, obj1);

	GameObject* letterO = CreateGameObject(pCompoundO, 2.0f, btVector3(0.9, 0.1, 0.1), btVector3(2.0f, 2.0f, -2.0f));	// Create a game object using the compound shape
		
	// Letter E
	trans3.setIdentity();
	btCompoundShape* pCompoundE = new btCompoundShape();												// Create a compound shape
																										// add the parts
	pCompoundE->addChildShape(trans3, obj1);
	trans3.setOrigin(btVector3(-1.75f, 0.5f, 0.0f));
	pCompoundE->addChildShape(trans3, pball);
	trans3.setOrigin(btVector3(-1.25f, 0.75f, 0.0f));
	pCompoundE->addChildShape(trans3, obj2);
	trans3.setOrigin(btVector3(0.0f, 0.5f, 0.0f));
	pCompoundE->addChildShape(trans3, obj3);
	trans3.setOrigin(btVector3(1.25f, 0.75f, 0.0f));
	pCompoundE->addChildShape(trans3, obj2);
	trans3.setIdentity();
	trans3.setOrigin(btVector3(1.75f, 0.5f, 0.0f));
	pCompoundE->addChildShape(trans3, pball);

	GameObject* letterE = CreateGameObject(pCompoundE, 2.0f, btVector3(0.9, 0.1, 0.1), btVector3(-1.0f, 2.0f, -2.0f));	// Create a game object using the compound shape
	
	letterJ->GetRigidBody()->applyTorqueImpulse(btVector3(0.0f, 1, 0.0f) * 10.0f);
	letterO->GetRigidBody()->applyTorque(btVector3(0.0f, 1, 0.0f) * 250.0f);
	//letterO->GetRigidBody()->applyTorqueImpulse(btVector3(0.0f, 1, 0.0f) * 10.0f);
	letterE->GetRigidBody()->applyTorqueImpulse(btVector3(0.0f, 1, 0.0f) * 15.0f);
	
	letterJ->SetType(BONUS);
	letterO->SetType(BONUS);
	letterE->SetType(BONUS);
}

// Keyboard
void JoeScene::Keyboard(unsigned char key, int x, int y) {	
	BulletOpenGLApplication::Keyboard(key, x, y);										// call the base implementation first

	switch (key) {
		// Force testing		
		//case 'g': {
		//	m_bApplyForce = true;														// if 'g' is held down, apply a force		
		//	m_pBox->GetRigidBody()->setActivationState(DISABLE_DEACTIVATION);			// prevent the box from deactivating
		//	break;
		//}
		// Impulse testing
		case 'e': {		
			std::cout << "button E pressed" << std::endl;
			if (m_pExplosion || !m_bCanExplode) break;									// Don't create a new explosion if one already exists or we haven't released the key, yet	

			std::cout << "Can  Explode" << std::endl;
			m_bCanExplode = false;														// Don't let us create another explosion until the key is released		

			//m_pExplosion = new btCollisionObject();									// create a collision object for our explosion
			//m_pExplosion->setCollisionShape(new btSphereShape(3.0f));			
			RayResult result;															// get the position that we clicked
			Raycast(m_cameraPosition, GetPickingRay(x, y), result, true);			

			CreateExplosion(result.hitPoint);											// JOR
			//btTransform explodeTrans;													// create a transform from the hit point
			//explodeTrans.setIdentity();
			//explodeTrans.setOrigin(result.hitPoint);
			//m_pExplosion->setWorldTransform(explodeTrans);			
			//m_pExplosion->setCollisionFlags(btCollisionObject::CF_NO_CONTACT_RESPONSE);	// set the collision flag			
			//m_pWorld->addCollisionObject(m_pExplosion);									// add the explosion trigger to our world
			break;
		}
		// Audio 
		case 'p': Audio::Instance()->playPauseMusic(); break;								// JOR Pause or play the music
		case '.': Audio::Instance()->musicForward(); break;									// JOR Skip music track backwards
		case ',': Audio::Instance()->musicBack(); break;									// JOR Skip music track forwards
		case 'o': ShootArrow(GetPickingRay(x, y)); break;									// Shoot old type of arrow
	}
}

void JoeScene::KeyboardUp(unsigned char key, int x, int y) {
	// call the base implementation first
	BulletOpenGLApplication::KeyboardUp(key, x, y);
	switch (key) {
		// Force testing
	
	//case 'g': {		
	//	m_bApplyForce = false;										// if 'g' is let go, stop applying the force		
	//	m_pBox->GetRigidBody()->forceActivationState(ACTIVE_TAG);	// allow the object to deactivate again
	//	break;
	//}
	
	// Impulse testing
	case 'e': m_bCanExplode = true; break;
	}
}

// Mouse
void JoeScene::Mouse(int button, int state, int x, int y) {
	BulletOpenGLApplication::Mouse(button, state, x, y);

	switch(button) {
		// Middle button fires ball
	case 1: 
		if (state == 0) {
			std::cout << "Middle Button Pressed" << std::endl;
			if (m_canFireBall) {
				Audio::Instance()->playFX("swoosh2FX");											// JOR Play swoosh sound 2 from map using ID 
				//Audio::Instance()->Fire2();													// JOR Play swoosh sound 2
				ShootBall(GetPickingRay(x, y));													// JOR Middle button pressed, shoot a sphere
				m_canFireBall = false;
			}
			break;
		} else {
			std::cout << "Middle Button Not Pressed" << std::endl;
		}
		break;
		// Right button fires arrow
	case 2: 
		if (state == 0) {
			std::cout << "Right Button Pressed" << std::endl;
			Audio::Instance()->playFX("swoosh1FX");											// JOR Play swoosh sound 1 from map using ID 
			//Audio::Instance()->Fire1();													// JOR Play swoosh sound 1
			//ShootArrow(GetPickingRay(x, y)); break;										// JOR Right mouse button pressed, shoot arrow
			ShootArrowCompound(GetPickingRay(x, y)); break;									// JOR Right mouse button pressed, shoot arrow
		}
		else {
			std::cout << "Right Button Not Pressed" << std::endl; break;
		}
	}
}

// Add to BulletOpenGLApplication function, 
// playing effect when a projectile collides with an object in it's mask
void JoeScene::CollisionEvent(btRigidBody * pBody0, btRigidBody * pBody1) {
	// Find the two colliding objects
	GameObject* pObj0 = FindGameObject((btRigidBody*)pBody0);
	GameObject* pObj1 = FindGameObject((btRigidBody*)pBody1);

	// Impulse testing
	if (pBody0 == m_pExplosion || pBody1 == m_pExplosion) {															// If one of the objects is an explosion
		btRigidBody* pOther;																						// get the pointer of the other object
		pBody0 == m_pExplosion ? pOther = (btRigidBody*)pBody1 : pOther = (btRigidBody*)pBody0;
		pOther->setActivationState(ACTIVE_TAG);																		// wake the object up		
		btVector3 dir = pOther->getWorldTransform().getOrigin() - m_pExplosion->getWorldTransform().getOrigin();	// calculate the vector between the object and the center of the explosion
		float dist = dir.length();																					// get the distance
		float strength = EXPLOSION_STRENGTH;																		// calculate the impulse strength
		if (dist != 0.0) strength /= dist;																			// follow an inverse-distance rule
		dir.normalize();																							// normalize the direction vector
		pOther->applyCentralImpulse(dir * strength);																// apply the impulse
		//pOther->applyImpulse(dir * strength, dir);
		//pOther->applyTorqueImpulse(dir*strength);
	}


	if (!pObj0 || !pObj1) return;																					// exit if we didn't find anything

	if (pObj0->GetType() == PROJECTILE || pObj1->GetType() == PROJECTILE) {
		//DoProjectileCollisionStuff(PROJECTILE);																					// Increment score & play sound
		//pObj0->GetType() == PROJECTILE ? DestroyGameObject(pObj0->GetRigidBody()) : DestroyGameObject(pObj1->GetRigidBody());	// Destroy the projectile

		if (pObj0->GetType() == GROUND || pObj1->GetType() == GROUND)
			DoProjectileCollisionStuff(GROUND);
		else {
			if (pObj0->GetType() == BONUS || pObj1->GetType() == BONUS)
				DoProjectileCollisionStuff(BONUS);
				//std::cout << "TEST BONUS" << std::endl;
			else
				DoProjectileCollisionStuff(PROJECTILE);

			pObj0->GetType() == PROJECTILE ? DestroyGameObject(pObj1->GetRigidBody()) : DestroyGameObject(pObj0->GetRigidBody());	// Destroy the projectile
			pObj1->GetType() == PROJECTILE ? DestroyGameObject(pObj1->GetRigidBody()) : DestroyGameObject(pObj0->GetRigidBody());	// Destroy the projectile
		}
	}
	/*
	if (pObj1->GetType() == PROJECTILE) {
		DestroyGameObject(pObj1->GetRigidBody());
		DoProjectileCollisionStuff();
		//Audio::Instance()->playFX("gunfireFX");
		//incrementScore(10);
	}
	*/
	
	// Create explosion for Ball projectile collision (not working)
	if (pObj0->GetType() == EXPLODING || pObj1->GetType() == EXPLODING) {
		std::cout << "Exploding Object Collision" << std::endl;
		btRigidBody* pExplosionBody;
		//GameObject* pExplosionObj;

		pObj1->GetType() == EXPLODING ? pExplosionBody = (btRigidBody*)pBody1 : pExplosionBody = (btRigidBody*)pBody0;
		//pObj0->GetType() == EXPLODING ? pExplosionObj = pObj0 : pExplosionObj = pObj1;

		//CreateExplosion(pExplosionBody->GetTransform());//
		//CreateExplosion2(pExplosionBody->getWorldTransform());
		//pExplosionBody->getWorldTransform().getOrigin()

		if (m_pExplosion || !m_bCanExplode) {
			std::cout << "Can't create explosion " << std::endl;
			//return;
		}

		CreateExplosion(pExplosionBody->getWorldTransform().getOrigin());
		 /*
		//CreateExplosion(btVector3(2.5, .5, 1
		std::cout << "Create Explosion X: " <<
		pExplosionBody->getWorldTransform().getOrigin().getX() <<
		" Y: " << pExplosionBody->getWorldTransform().getOrigin().getY() <<
		" Z: " << pExplosionBody->getWorldTransform().getOrigin().getZ() << std::endl;

		//m_bCanExplode = true; 
		*/

		m_canFireBall = true;

		// If it's a collision with the ground
		if (pObj0->GetType() == GROUND || pObj1->GetType() == GROUND)
			DoProjectileCollisionStuff(GROUND);
		else
			DoProjectileCollisionStuff(EXPLODING);

		DestroyGameObject(pExplosionBody);
		/*
		Audio::Instance()->playFX("explosionFX");
		incrementScore(50);
		*/
	}
	/*
	// Create explosion for Ball projectile collision (not working)
	if (pObj0->GetType() == EXPLODING || pObj1->GetType() == EXPLODING) {
		std::cout << "Exploding Object Collision" << std::endl;
		btRigidBody* pExplosionBody;
		//GameObject* pExplosionObj;

		pObj1->GetType() == EXPLODING ? pExplosionBody = (btRigidBody*)pBody1 : pExplosionBody = (btRigidBody*)pBody0;
		//pObj0->GetType() == EXPLODING ? pExplosionObj = pObj0 : pExplosionObj = pObj1;

		//CreateExplosion(pExplosionBody->GetTransform());//
		//CreateExplosion2(pExplosionBody->getWorldTransform());
		//pExplosionBody->getWorldTransform().getOrigin()

		if (m_pExplosion || !m_bCanExplode) {
			std::cout << "Can't create explosion " << std::endl;
			//return;
		}

		CreateExplosion(pExplosionBody->getWorldTransform().getOrigin());
		//CreateExplosion(btVector3(2.5, .5, 1));

		
		//std::cout << "Create Explosion X: " <<
		//pExplosionBody->getWorldTransform().getOrigin().getX() <<
		//" Y: " << pExplosionBody->getWorldTransform().getOrigin().getY() <<
		//" Z: " << pExplosionBody->getWorldTransform().getOrigin().getZ() << std::endl;

		//m_bCanExplode = true; 
		m_canFireBall = true;

		DestroyGameObject(pExplosionBody);

		DoProjectileCollisionStuff(EXPLODING);
		//Audio::Instance()->playFX("explosionFX");
		//incrementScore(50);
	}	
	*/
}

void JoeScene::CreateExplosion(const btVector3 &origin) {
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

// Play sound and increment score
void JoeScene::DoProjectileCollisionStuff(int type) {
	if (type == PROJECTILE) {
		Audio::Instance()->playFX("gunfireFX");
		incrementScore(10);
	} else if (type == EXPLODING) {
		Audio::Instance()->playFX("explosionFX");
		incrementScore(50);
	} else if (type == GROUND) {
		Audio::Instance()->playFX("gunfireFX");				// If colliding with ground play "bang" sound and don't increment score
	} else if (type == BONUS) {
		Audio::Instance()->playFX("bonusFX");
		incrementScore(25);
	}
}

// Ball projectile fires from camera position, when middle mouse button pressed
// Explode when colliding
void JoeScene::ShootBall(const btVector3 &direction) {

	// create game object (shape, mass, colour, initial position, group, mask, rotation)

	//GameObject* pObject = CreateGameObject(new btBoxShape(btVector3(.5, .5, .5)), 
	//	1, btVector3(0.4f, 0.f, 0.4f), m_cameraPosition);											// create a new box object
	GameObject* pObject = CreateGameObject(new btSphereShape(0.5),
		1.0f,																						// Mass
		btVector3(1.0f, 1.0f, 0.0f),																// Colour
		m_cameraPosition, 																			// Start at Cameras position
		COLGROUP_BULLET,																			// Collision groups are box and bullet
		COLGROUP_BOX | COLGROUP_STATIC);

	pObject->SetPlayAudio(false);
	//pObject->SetName("bullet");
//	pObject->SetType(PROJECTILE);																	// JOR Set the type as bullet
	pObject->SetType(EXPLODING);																	// JOR Set the type as bullet

	btVector3 velocity = direction;																	// calculate the velocity
	velocity.normalize();
	velocity *= 25.0f;

	pObject->GetRigidBody()->setLinearVelocity(velocity);											// set the linear velocity of the box
}

// Compound shape projectile
// Arrow projectile fires from camera position, when right mouse button pressed
void JoeScene::ShootArrowCompound(const btVector3 &direction) {
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
	btCompoundShape* pCompound = new btCompoundShape();												// Ch 7.3 - Create our compound shape
	trans.setRotation(btQuaternion(0, 1, 0, -1));
	pCompound->addChildShape(trans, head);
	trans.setRotation(btQuaternion(1, 0, 0, -1));
	trans.setOrigin(-btVector3(0, 0, 2));
	pCompound->addChildShape(trans, shaft);
	trans.setOrigin(-btVector3(0, -0.2, 3.75));														// add the rod
	pCompound->addChildShape(trans, flight1);
	trans.setOrigin(-btVector3(0, 0.2, 3.75));
	pCompound->addChildShape(trans, flight2);
	trans.setOrigin(-btVector3(-0.2, 0, 3.75));														// add the rod
	pCompound->addChildShape(trans, flight3);
	trans.setOrigin(-btVector3(0.2, 0, 3.75));
	pCompound->addChildShape(trans, flight4);
	
	trans.setIdentity();

	// create game object (shape, mass, colour, initial position, group, mask, rotation)
//	GameObject* arrow = CreateGameObject(pCompound, 2.0f, btVector3(0.8, 0.4, 0.1), m_cameraPosition);	// Create a game object using the compound shape
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
	
	btVector3 velocity = direction;																	// calculate the velocity
	velocity.normalize();
	velocity *= 25.0f;
	
	arrow->SetType(PROJECTILE);																		// Lets collision audio play
	arrow->GetRigidBody()->setLinearVelocity(velocity);												// set the linear velocity of the arrow shaft
}

// Bunch of shapes fired together and hoping for the best
void JoeScene::ShootArrow(const btVector3 &direction) {
	
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

void JoeScene::RenderScene() {
	BulletOpenGLApplication::RenderScene();														// Call the base implementation first
																								//int score = 0;
	char scoreText[20];
	//snprintf(scoreText, sizeof(scoreText), "Score: %d", score);
	snprintf(scoreText, sizeof(scoreText), "Score: %d", m_score);

	char* test = "Joe O'Regan K00203642";

	//Text::Instance()->PrintText(20, 20, "Score: ");
	Text::Instance()->PrintText(20, 20, scoreText);
	Text::Instance()->PrintText(m_screenWidth - 240, 20, "Joe O'Regan K00203642");
}