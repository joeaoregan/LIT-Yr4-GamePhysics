// Added Ch 8

//#include <SDL.h>
#include "Audio.h"
//#include <SDL_image.h>
//#include "SDL_ttf.h"
//#include <SDL_mixer.h>
#include "JoeScene.h"

#define BLOCKS_IN_ROW 11

//Mix_Music  *gMusic = NULL;

//Mix_Chunk *gSound = NULL;

void InitSDLAudio();
//bool InitSDL();

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

	// create 25 boxes and spheres in a 5x5 stack formation 
	float yPos = 0;
	for (int i = 0; i < 55; i++) {
		float xPos = -5 + 2 * (float)(i % BLOCKS_IN_ROW);														// Calculate x position for the object (5 columns)
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
			//btVector3(0.9f, 0.9f + ((i%2) * 0.025f), 1.0f),																		// alternate colors
			position,
			COLGROUP_SPHERE,
			COLGROUP_SPHERE | COLGROUP_STATIC);																	// spheres can collide with the ground plane and other spheres
		*/
	}
}
