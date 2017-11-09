/*
	Joe O'Regan
	K00203642

	09/11/2017

	Game Physics CA
	Scene using Bullet and OpenGL
*/

#include "BulletOpenGLApplication.h"
#include "FreeGLUTCallbacks.h"

int main(int argc, char** argv) {
	BulletOpenGLApplication demo;

	return glutmain(argc, argv, 1024, 768, "Introduction to Game Physics with Bullet Physics and OpenGL", &demo);
}