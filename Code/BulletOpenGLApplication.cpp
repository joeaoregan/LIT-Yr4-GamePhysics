#include "BulletOpenGLApplication.h"

BulletOpenGLApplication::BulletOpenGLApplication() :
		m_cameraPosition(10.0f, 5.0f, 0.0f),
		m_cameraTarget(0.0f, 0.0f, 0.0f),
		m_upVector(0.0f, 1.0f, 0.0f),
		m_nearPlane(1.0f),
		m_farPlane(1000.0f)
{}

BulletOpenGLApplication::~BulletOpenGLApplication() {}

void BulletOpenGLApplication::Initialize() {
	// this function is called inside glutmain() after creating the window, but before handing control to FreeGLUT
	glClearColor(0.6, 0.65, 0.85, 0);	// set the backbuffer clearing color to a lightish blue
}
void BulletOpenGLApplication::Keyboard(unsigned char key, int x, int y) {}
void BulletOpenGLApplication::KeyboardUp(unsigned char key, int x, int y) {}
void BulletOpenGLApplication::Special(int key, int x, int y) {}
void BulletOpenGLApplication::SpecialUp(int key, int x, int y) {}

void BulletOpenGLApplication::Reshape(int w, int h) {
	// this function is called once during application intialization and again every time we resize the window
	m_screenWidth = w;		// grab the screen height
	m_screenHeight = h;		// grab the screen height	
	glViewport(0, 0, w, h);	// set the viewport	
	UpdateCamera();			// update the camera
}

void BulletOpenGLApplication::Idle() {
	// this function is called frequently, whenever FreeGlut isn't busy processing its own events. It should be used
	// to perform any updating and rendering tasks 
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// clear the backbuffer	
	UpdateCamera();										// update the camera
	glutSwapBuffers();									// swap the front and back buffers
}

void BulletOpenGLApplication::Mouse(int button, int state, int x, int y) {}
void BulletOpenGLApplication::PassiveMotion(int x, int y) {}
void BulletOpenGLApplication::Motion(int x, int y) {}
void BulletOpenGLApplication::Display() {}

void BulletOpenGLApplication::UpdateCamera() {
	if (m_screenWidth == 0 && m_screenHeight == 0) return;		// exit in erroneous situations	
	
	glMatrixMode(GL_PROJECTION);								// select the projection matrix
	
	glLoadIdentity();											// set it to the matrix-equivalent of 1
	
	float aspectRatio = m_screenWidth / (float)m_screenHeight;	// determine the aspect ratio of the screen
	// create a viewing frustum based on the aspect ratio and the boundaries of the camera
	glFrustum(-aspectRatio * m_nearPlane, aspectRatio * m_nearPlane, -m_nearPlane, m_nearPlane, m_nearPlane, m_farPlane);
	// the projection matrix is now set
								
	glMatrixMode(GL_MODELVIEW);									// select the view matrix
	
	glLoadIdentity();											// set it to '1'
	// create a view matrix based on the camera's position and where it's looking
	gluLookAt(m_cameraPosition[0], m_cameraPosition[1], m_cameraPosition[2], m_cameraTarget[0], m_cameraTarget[1], m_cameraTarget[2], m_upVector.getX(), m_upVector.getY(), m_upVector.getZ());
	// the view matrix is now set
}