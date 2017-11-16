/*
	Ch 4.2 - toggle wireframe debug drawing & toggle AABB debug drawing
*/

#include "BulletOpenGLApplication.h"

#define RADIANS_PER_DEGREE 0.01745329f														// Constant for 3D Math
#define CAMERA_STEP_SIZE 5.0f																// Constant for camera speed

BulletOpenGLApplication::BulletOpenGLApplication() :
	m_cameraPosition(10.0f, 5.0f, 0.0f),
	m_cameraTarget(0.0f, 0.0f, 0.0f),
	m_cameraDistance(15.0f),
	m_cameraPitch(20.0f),
	m_cameraYaw(0.0f),
	m_upVector(0.0f, 1.0f, 0.0f),
	m_nearPlane(1.0f),
	m_farPlane(1000.0f),
	m_pBroadphase(0),
	m_pCollisionConfiguration(0),
	m_pDispatcher(0),
	m_pSolver(0),
	m_pWorld(0)
//	m_pMotionState(0)
{}

BulletOpenGLApplication::~BulletOpenGLApplication() {
	ShutdownPhysics();																		// shutdown the physics system
}

void BulletOpenGLApplication::Initialize() {
	// this function is called inside glutmain() after creating the window, but before handing control to FreeGLUT

	// create some floats for our ambient, diffuse, specular and position
	GLfloat ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };											// dark grey
	GLfloat diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };											// white
	GLfloat specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };										// white
	GLfloat position[] = { 5.0f, 10.0f, 1.0f, 0.0f };

	// set the ambient, diffuse, specular and position for LIGHT0
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
	glLightfv(GL_LIGHT0, GL_POSITION, position);

	glEnable(GL_LIGHTING);																	// enables lighting
	glEnable(GL_LIGHT0);																	// enables the 0th light
	glEnable(GL_COLOR_MATERIAL);															// colors materials when lighting is enabled

	// enable specular lighting via materials
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
	glMateriali(GL_FRONT, GL_SHININESS, 15);
		
	glShadeModel(GL_SMOOTH);																// enable smooth shading

	// enable depth testing to be 'less than'
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glClearColor(0.6, 0.65, 0.85, 0);														// set the backbuffer clearing color to a lightish blue

	InitializePhysics();																	// initialize the physics system

	m_pDebugDrawer = new DebugDrawer();														// Ch 4.2 - create the debug drawer
	m_pDebugDrawer->setDebugMode(0);														// Ch 4.2 - set the initial debug level to 0
	m_pWorld->setDebugDrawer(m_pDebugDrawer);												// Ch 4.2 - add the debug drawer to the world
}

void BulletOpenGLApplication::Keyboard(unsigned char key, int x, int y) {
	// This function is called by FreeGLUT whenever generic keys are pressed down.
	switch (key) {	
		case 'z': ZoomCamera(+CAMERA_STEP_SIZE); break;										// 'z' zooms in
		case 'x': ZoomCamera(-CAMERA_STEP_SIZE); break;										// 'x' zoom out
		case 'w': m_pDebugDrawer->ToggleDebugFlag(btIDebugDraw::DBG_DrawWireframe); break;	// Ch 4.2 - toggle wireframe debug drawing
		case 'b':m_pDebugDrawer->ToggleDebugFlag(btIDebugDraw::DBG_DrawAabb); break;		// Ch 4.2 - toggle AABB debug drawing
	}
}

void BulletOpenGLApplication::KeyboardUp(unsigned char key, int x, int y) {}

void BulletOpenGLApplication::Special(int key, int x, int y) {
	// This function is called by FreeGLUT whenever special keys are pressed down, like the arrow keys, or Insert, Delete etc.
	switch (key) {
		// the arrow keys rotate the camera up/down/left/right
		case GLUT_KEY_LEFT:
			RotateCamera(m_cameraYaw, +CAMERA_STEP_SIZE); break;
		case GLUT_KEY_RIGHT:
			RotateCamera(m_cameraYaw, -CAMERA_STEP_SIZE); break;
		case GLUT_KEY_UP:
			RotateCamera(m_cameraPitch, +CAMERA_STEP_SIZE); break;
		case GLUT_KEY_DOWN:
			RotateCamera(m_cameraPitch, -CAMERA_STEP_SIZE); break;
	}
}

void BulletOpenGLApplication::SpecialUp(int key, int x, int y) {}

void BulletOpenGLApplication::Reshape(int w, int h) {
	// this function is called once during application intialization and again every time we resize the window
	m_screenWidth = w;																		// grab the screen height
	m_screenHeight = h;																		// grab the screen height	
	glViewport(0, 0, w, h);																	// set the viewport	
	UpdateCamera();																			// update the camera
}

void BulletOpenGLApplication::Idle() {
	// this function is called frequently, whenever FreeGlut isn't busy processing its own events. 
	// It should be used to perform any updating and rendering tasks 
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);										// clear the backbuffer	

	float dt = m_clock.getTimeMilliseconds();												// get the time since the last iteration
	m_clock.reset();																		// reset the clock to 0
	UpdateScene(dt / 1000.0f);																// update the scene (convert ms to s)

	UpdateCamera();																			// update the camera

	//DrawBox(btVector3(1, 1, 1), btVector3(1.0f, 0.2f, 0.2f));								// draw a simple box of size 1 also draw it red

	RenderScene();																			// Render the scene

	glutSwapBuffers();																		// swap the front and back buffers
}

void BulletOpenGLApplication::Mouse(int button, int state, int x, int y) {}
void BulletOpenGLApplication::PassiveMotion(int x, int y) {}
void BulletOpenGLApplication::Motion(int x, int y) {}
void BulletOpenGLApplication::Display() {}

void BulletOpenGLApplication::UpdateCamera() {
	if (m_screenWidth == 0 && m_screenHeight == 0) return;									// exit in erroneous situations	
	
	glMatrixMode(GL_PROJECTION);															// select the projection matrix
	
	glLoadIdentity();																		// set it to the matrix-equivalent of 1
	
	float aspectRatio = m_screenWidth / (float)m_screenHeight;								// determine the aspect ratio of the screen
	// create a viewing frustum based on the aspect ratio and the boundaries of the camera
	glFrustum(-aspectRatio * m_nearPlane, aspectRatio * m_nearPlane, -m_nearPlane, m_nearPlane, m_nearPlane, m_farPlane);
	// the projection matrix is now set
								
	glMatrixMode(GL_MODELVIEW);																// select the view matrix
	
	glLoadIdentity();																		// set it to '1'

	// our values represent the angles in degrees, but 3D math typically demands angular values are in radians.
	float pitch = m_cameraPitch * RADIANS_PER_DEGREE;
	float yaw = m_cameraYaw * RADIANS_PER_DEGREE;
		
	btQuaternion rotation(m_upVector, yaw);													// create a quaternion defining the angular rotation around the up vector
		
	btVector3 cameraPosition(0,0,0);														// set the camera's position to 0,0,0, then
	cameraPosition[2] = -m_cameraDistance;													// move the 'z' position to the current value of m_cameraDistance.
		
	btVector3 forward(cameraPosition[0], cameraPosition[1], cameraPosition[2]);				// create a Bullet Vector3 to represent the camera position
	if (forward.length2() < SIMD_EPSILON) {													// and if its value is too small
		forward.setValue(1.f,0.f,0.f);														// scale it up
	}
	
	// figure out the 'right' vector by using the cross product on the 'forward' and 'up' vectors
	btVector3 right = m_upVector.cross(forward);	
	
	btQuaternion roll(right, - pitch);														// create a quaternion that represents the camera's roll
	
	// turn the rotation (around the Y-axis) and roll (around the forward axis) into transformation matrices and 
	// apply them to the camera position. This gives us the final position
	cameraPosition = btMatrix3x3(rotation) * btMatrix3x3(roll) * cameraPosition;
	
	// save our new position in the member variable, and shift it relative to the target position (so that we orbit it)
	m_cameraPosition[0] = cameraPosition.getX();
	m_cameraPosition[1] = cameraPosition.getY();
	m_cameraPosition[2] = cameraPosition.getZ();
	m_cameraPosition += m_cameraTarget;

	// create a view matrix based on the camera's position and where it's looking
	gluLookAt(m_cameraPosition[0], m_cameraPosition[1], m_cameraPosition[2], m_cameraTarget[0], m_cameraTarget[1], m_cameraTarget[2], m_upVector.getX(), m_upVector.getY(), m_upVector.getZ());
	// the view matrix is now set
}

/*
void BulletOpenGLApplication::DrawBox(btScalar* transform, const btVector3 &halfSize, const btVector3 &color) {
	glPushMatrix();																			// push the transform onto the stack
	glMultMatrixf(transform);
*/
void BulletOpenGLApplication::DrawBox(const btVector3 &halfSize) {
	float halfWidth = halfSize.x();
	float halfHeight = halfSize.y();
	float halfDepth = halfSize.z();
		
//	glColor3f(color.x(), color.y(), color.z());												// set the object's color
	
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
			
	glBegin(GL_TRIANGLES);																	// start processing vertices as triangles
	
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
			
		glNormal3f(normal.getX(), normal.getY(), normal.getZ());							// set the normal for the subsequent vertices
		
		// create the vertices
		glVertex3f(vert1.x(), vert1.y(), vert1.z());
		glVertex3f(vert2.x(), vert2.y(), vert2.z());
		glVertex3f(vert3.x(), vert3.y(), vert3.z());
	}
	
	glEnd();																				// stop processing vertices
	
//	glPopMatrix();																			// pop the transform from the stack in preparation for the next object
}

void BulletOpenGLApplication::RotateCamera(float &angle, float value) {	
	angle -= value;																			// change the value (it is passed by reference, so we can edit it here)
	// keep the value within bounds
	if (angle < 0) angle += 360;
	if (angle >= 360) angle -= 360;	
	UpdateCamera();																			// update the camera since we changed the angular value
}

void BulletOpenGLApplication::ZoomCamera(float distance) {	
	m_cameraDistance -= distance;															// change the distance value	
	if (m_cameraDistance < 0.1f) m_cameraDistance = 0.1f;									// prevent it from zooming in too far	
	UpdateCamera();																			// update the camera since we changed the zoom distance
	
}

void BulletOpenGLApplication::RenderScene() {	
	btScalar transform[16];																	// create an array of 16 floats (representing a 4x4 matrix)
	/*
	if (m_pMotionState) {
		m_pMotionState->GetWorldTransform(transform);										// get the world transform from our motion state
		DrawBox(transform, btVector3(1,1,1), btVector3(1.0f,0.2f,0.2f));					// feed the data into DrawBox
	}
	*/
	
	for (GameObjects::iterator i = m_objects.begin(); i != m_objects.end(); ++i) {			// iterate through all of the objects in our world		
		GameObject* pObj = *i;																// get the object from the iterator
		pObj->GetTransform(transform);														// read the transform		
		DrawShape(transform, pObj->GetShape(), pObj->GetColor());							// get data from the object and draw it
	}		

	m_pWorld->debugDrawWorld();																// Ch 4.2 - after rendering all game objects, perform debug rendering. 
																							// Bullet will figure out what needs to be drawn then call to our DebugDrawer class to do the rendering for us
}

void BulletOpenGLApplication::UpdateScene(float dt) {	
	if (m_pWorld) {																			// check if the world object exists		
		m_pWorld->stepSimulation(dt);														// step the simulation through time. This is called every update and the amount of elasped time was determined back in ::Idle() by our clock object.
	}
}


void BulletOpenGLApplication::DrawShape(btScalar* transform, const btCollisionShape* pShape, const btVector3 &color) {	
	glColor3f(color.x(), color.y(), color.z());																			// set the color
		
	glPushMatrix();																										// push the matrix stack
	glMultMatrixf(transform);
		
	switch(pShape->getShapeType()) {																					// make a different draw call based on the object type		
	case BOX_SHAPE_PROXYTYPE: {																							// an internal enum used by Bullet for boxes		
		const btBoxShape* box = static_cast<const btBoxShape*>(pShape);													// assume the shape is a box, and typecast it		
		btVector3 halfSize = box->getHalfExtentsWithMargin();															// get the 'halfSize' of the box		
		DrawBox(halfSize);																								// draw the box	
		break;	
	}
	default:		
		break;																											// unsupported type
	}
		
	glPopMatrix();																										// pop the stack
}

GameObject* BulletOpenGLApplication::CreateGameObject(btCollisionShape* pShape, const float &mass, const btVector3 &color, const btVector3 &initialPosition, const btQuaternion &initialRotation) {
	GameObject* pObject = new GameObject(pShape, mass, color, initialPosition, initialRotation);						// create a new game object
		
	m_objects.push_back(pObject);																						// push it to the back of the list
		
	if (m_pWorld) {																										// check if the world object is valid		
		m_pWorld->addRigidBody(pObject->GetRigidBody());																// add the object's rigid body to the world
	}
	return pObject;
}