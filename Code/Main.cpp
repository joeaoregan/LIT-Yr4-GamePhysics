/*
	Joe O'Regan
	K00203642

	09/11/2017

	Game Physics CA
	Scene using Bullet and OpenGL
*/

//#include "BulletOpenGLApplication.h"
#include "BasicDemo.h"
#include "CollisionFilteringDemo.h"							// Ch 8
#include "SoftBodyDemo.h"									// Ch 9
#include "FreeGLUTCallbacks.h"
#include <time.h>

#include "Joe/JoeScene.h"
#include "Joe/JoeSceneV2.h"

int main(int argc, char** argv) {
	srand(time(NULL));										// JOR For Random track
	/* swap these statements to launch a different demo */
	//BulletOpenGLApplication demo;
	//BasicDemo demo;										// Ch8 Removed
	//CollisionFilteringDemo demo;							// Ch8 Added, Ch 9 Removed
	//SoftBodyDemo demo;									// Ch9 Added

	JoeScene demo;											// 21/12/2017
	//JoeSceneV2 demo;										// 09/01/2017

	return glutmain(argc, argv, 1024, 768, "Year 4 - Game Physics CA - Bullet Physics and OpenGL - Joe O'Regan K00203642", &demo);
}