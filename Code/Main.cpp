/*
	Joe O'Regan
	K00203642

	09/11/2017

	Game Physics CA
	Scene using Bullet and OpenGL
*/

//#include "BulletOpenGLApplication.h"
#include "BasicDemo.h"
#include "FreeGLUTCallbacks.h"

int main(int argc, char** argv) {
	//BulletOpenGLApplication demo;
	BasicDemo demo;
	return glutmain(argc, argv, 1024, 768, "Year 4 - Game Physics CA - Bullet Physics and OpenGL - Joe O'Regan K00203642", &demo);
}