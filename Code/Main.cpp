/*
	Joe O'Regan
	K00203642

	09/11/2017

	Game Physics CA
	Scene using Bullet and OpenGL
*/

//#include "BulletOpenGLApplication.h"
#include "BasicDemo.h"
#include "CollisionFilteringDemo.h"							// Ch8
#include "FreeGLUTCallbacks.h"

int main(int argc, char** argv) {
	/* swap these statements to launch a different demo */
	//BulletOpenGLApplication demo;
	//BasicDemo demo;										// Ch8 Removed
	CollisionFilteringDemo demo;							// Ch8 Added
	return glutmain(argc, argv, 1024, 768, "Year 4 - Game Physics CA - Bullet Physics and OpenGL - Joe O'Regan K00203642", &demo);
}