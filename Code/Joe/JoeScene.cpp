// Added Ch 8

//#include <SDL.h>
#include "Audio.h"
//#include <SDL_image.h>
//#include "SDL_ttf.h"
//#include <SDL_mixer.h>
#include "JoeScene.h"

//#include "GLDebugFont.h"

#define BLOCKS_IN_ROW 11

//Mix_Music  *gMusic = NULL;

//Mix_Chunk *gSound = NULL;

void InitSDLAudio();
//bool InitSDL();

/*
void displayText(float x, float y, int r, int g, int b, const char *string) {
	//glColor3f(r, g, b);
	glColor3f(0, 0, 0);
	glRasterPos2f(x, y);
	for (int i = 0; i < strlen(string); i++) {
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, string[i]);
	}
}
*/
JoeScene::JoeScene() : BulletOpenGLApplication() {}

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
	CreateGameObject(new btBoxShape(btVector3(1, 50, 50)),
		0, btVector3(0.2f, 0.9f, 0.6f), btVector3(0.0f, 0.0f, 0.0f),
		COLGROUP_STATIC, COLGROUP_BOX | COLGROUP_SPHERE);										// The ground plane can collide with boxes, spheres and bullets (which are boxes and spheres)

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
