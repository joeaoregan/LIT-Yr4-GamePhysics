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

	// create some floats for our ambient, diffuse, specular and position
	GLfloat ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };				// dark grey
	GLfloat diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };				// white
	GLfloat specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };			// white
	GLfloat position[] = { 5.0f, 10.0f, 1.0f, 0.0f };

	// set the ambient, diffuse, specular and position for LIGHT0
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
	glLightfv(GL_LIGHT0, GL_POSITION, position);

	glEnable(GL_LIGHTING);										// enables lighting
	glEnable(GL_LIGHT0);										// enables the 0th light
	glEnable(GL_COLOR_MATERIAL);								// colors materials when lighting is enabled

	// enable specular lighting via materials
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
	glMateriali(GL_FRONT, GL_SHININESS, 15);
		
	glShadeModel(GL_SMOOTH);									// enable smooth shading

	// enable depth testing to be 'less than'
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glClearColor(0.6, 0.65, 0.85, 0);							// set the backbuffer clearing color to a lightish blue
}

void BulletOpenGLApplication::Keyboard(unsigned char key, int x, int y) {}
void BulletOpenGLApplication::KeyboardUp(unsigned char key, int x, int y) {}
void BulletOpenGLApplication::Special(int key, int x, int y) {}
void BulletOpenGLApplication::SpecialUp(int key, int x, int y) {}

void BulletOpenGLApplication::Reshape(int w, int h) {
	// this function is called once during application intialization and again every time we resize the window
	m_screenWidth = w;											// grab the screen height
	m_screenHeight = h;											// grab the screen height	
	glViewport(0, 0, w, h);										// set the viewport	
	UpdateCamera();												// update the camera
}

void BulletOpenGLApplication::Idle() {
	// this function is called frequently, whenever FreeGlut isn't busy processing its own events. It should be used
	// to perform any updating and rendering tasks 
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);			// clear the backbuffer	
	UpdateCamera();												// update the camera
	DrawBox(btVector3(1, 1, 1));								// draw a simple box of size 1 also draw it red
	glutSwapBuffers();											// swap the front and back buffers
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


void BulletOpenGLApplication::DrawBox(const btVector3 &halfSize, const btVector3 &color) {
	float halfWidth = halfSize.x();
	float halfHeight = halfSize.y();
	float halfDepth = halfSize.z();
		
	glColor3f(color.x(), color.y(), color.z());					// set the object's color
	
	// create the vertex positions
	btVector3 vertices[8] = {
	btVector3(halfWidth,halfHeight,halfDepth),
	btVector3(-halfWidth,halfHeight,halfDepth),
	btVector3(halfWidth,-halfHeight,halfDepth),
	btVector3(-halfWidth,-halfHeight,halfDepth),
	btVector3(halfWidth,halfHeight,-halfDepth),
	btVector3(-halfWidth,halfHeight,-halfDepth),
	btVector3(halfWidth,-halfHeight,-halfDepth),
	btVector3(-halfWidth,-halfHeight,-halfDepth) };
	
	// create the indexes for each triangle, using the vertices above. 
	// Make it static so we don't waste processing time recreating it over and over again
	static int indices[36] = {0,1,2,3,2,1,4,0,6,6,0,2,
								5,1,4,4,1,0,7,3,1,7,1,5,
								5,4,7,7,4,6,7,2,3,7,6,2 };
			
	glBegin(GL_TRIANGLES);	// start processing vertices as triangles
	
	// increment the loop by 3 each time since we create a triangle with 3 vertices at a time.							
	for (int i = 0; i < 36; i += 3) {
		// get the three vertices for the triangle based on the index values set above
		// use const references so we don't copy the object (a good rule of thumb is to never allocate/deallocate
		// memory during *every* render/update call. This should only happen sporadically)
		const btVector3 &vert1 = vertices[indices[i]];
		const btVector3 &vert2 = vertices[indices[i + 1]];
		const btVector3 &vert3 = vertices[indices[i + 2]];
		
		// create a normal that is perpendicular to the face (use the cross product)
		btVector3 normal = (vert3 - vert1).cross(vert2 - vert1);
		normal.normalize();	
			
		glNormal3f(normal.getX(), normal.getY(), normal.getZ());	// set the normal for the subsequent vertices
		
		// create the vertices
		glVertex3f(vert1.x(), vert1.y(), vert1.z());
		glVertex3f(vert2.x(), vert2.y(), vert2.z());
		glVertex3f(vert3.x(), vert3.y(), vert3.z());
	}
	
	glEnd();	// stop processing vertices
}