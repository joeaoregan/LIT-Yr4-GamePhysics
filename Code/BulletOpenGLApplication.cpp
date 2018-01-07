/*
	Ch 4.2 - toggle wireframe debug drawing & toggle AABB debug drawing
*/

#include "Joe/Audio.h"
#include "BulletOpenGLApplication.h"
#include <iostream>

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
	m_pWorld(0),
	m_pPickedBody(0),																		// Ch5.2
	m_pPickConstraint(0)																	// Ch5.2
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
		//case 'z': ZoomCamera(+CAMERA_STEP_SIZE); break;									// 'z' zooms in
		case 'z': RotateCamera(m_cameraPitch, +CAMERA_STEP_SIZE); break; break;				// 'z' rotate camera up
		//case 'x': ZoomCamera(-CAMERA_STEP_SIZE); break;									// 'x' zoom out
		case 'x': RotateCamera(m_cameraPitch, -CAMERA_STEP_SIZE); break;					// 'x' rotate camera down
		case 'w': m_pDebugDrawer->ToggleDebugFlag(btIDebugDraw::DBG_DrawWireframe); break;	// Ch 4.2 - toggle wireframe debug drawing
		case 'b': m_pDebugDrawer->ToggleDebugFlag(btIDebugDraw::DBG_DrawAabb); break;		// Ch 4.2 - toggle AABB debug drawing
		case 'd': {																			// Ch 5.1
			RayResult result;																// create a temp object to store the raycast result
			// perform the raycast
			if (!Raycast(m_cameraPosition, GetPickingRay(x, y), result)) return;			// return if the test failed		
			DestroyGameObject(result.pBody);												// destroy the corresponding game object
			break;
		}
		// Audio 
		case 'p': Audio::Instance()->playPauseMusic(); break;								// JOR Pause or play the music
		case '.': Audio::Instance()->musicForward(); break;									// JOR Skip music track backwards
		case ',': Audio::Instance()->musicBack(); break;									// JOR Skip music track forwards
	}
}

void BulletOpenGLApplication::KeyboardUp(unsigned char key, int x, int y) {}

void BulletOpenGLApplication::Special(int key, int x, int y) {
	// This function is called by FreeGLUT whenever special keys are pressed down, like the arrow keys, or Insert, Delete etc.
	switch (key) {
		// the arrow keys rotate the camera up/down/left/right
		case GLUT_KEY_LEFT:
			RotateCamera(m_cameraYaw, +CAMERA_STEP_SIZE); break;			// Zoom in
		case GLUT_KEY_RIGHT:
			RotateCamera(m_cameraYaw, -CAMERA_STEP_SIZE); break;			// Zoom out
		case GLUT_KEY_UP:
			//RotateCamera(m_cameraPitch, +CAMERA_STEP_SIZE); break;
			ZoomCamera(+CAMERA_STEP_SIZE); break;
		case GLUT_KEY_DOWN:
			//RotateCamera(m_cameraPitch, -CAMERA_STEP_SIZE); break;
			ZoomCamera(-CAMERA_STEP_SIZE); break;
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

void BulletOpenGLApplication::Mouse(int button, int state, int x, int y) {
		switch(button) {
		case 0: {																			// Ch5.2 left mouse button
			if (state == 0) CreatePickingConstraint(x, y);									// button down, create the picking constraint when we click the LMB
			else RemovePickingConstraint();													// button up, remove the picking constraint when we release the LMB
			break;
		}
		case 2: if (state == 0) {
			std::cout << "Right Button Pressed" << std::endl;
			Audio::Instance()->playFX("swoosh1FX");											// JOR Play swoosh sound 1 from map using ID 
			//Audio::Instance()->Fire1();													// JOR Play swoosh sound 1
			ShootBox(GetPickingRay(x, y)); break;											// Ch5.1right mouse button, pressed down, shoot a box
		}
		case 1: if (state == 0) {
			std::cout << "Middle Button Pressed" << std::endl;
			Audio::Instance()->playFX("swoosh2FX");											// JOR Play swoosh sound 2 from map using ID 
			//Audio::Instance()->Fire2();													// JOR Play swoosh sound 2
			ShootBall(GetPickingRay(x, y)); break;											// JOR Middle button pressed, shoot a sphere
		}
	}
}
void BulletOpenGLApplication::PassiveMotion(int x, int y) {}

// Ch5.2
void BulletOpenGLApplication::Motion(int x, int y) {
	// did we pick a body with the LMB?
	if (m_pPickedBody) {
		btGeneric6DofConstraint* pickCon = static_cast<btGeneric6DofConstraint*>(m_pPickConstraint);
		if (!pickCon) return;
	
		// use another picking ray to get the target direction
		btVector3 dir = GetPickingRay(x,y) - m_cameraPosition;
		dir.normalize();
	
		// use the same distance as when we originally picked the object
		dir *= m_oldPickingDist;
		btVector3 newPivot = m_cameraPosition + dir;
			
		pickCon->getFrameOffsetA().setOrigin(newPivot);										// set the position of the constraint
	}
}

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
	static int indices[36] = {0,1,2,3,2,1,4,0,6,6,0,2, 5,1,4,4,1,0,7,3,1,7,1,5, 5,4,7,7,4,6,7,2,3,7,6,2 };
			
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

	CheckForCollisionEvents();																// Ch 6. 1 - Check for any new collisions/serparations
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
	case SPHERE_SHAPE_PROXYTYPE: {																						// Ch 6.3 - Draw Sphere shape		
		const btSphereShape* sphere = static_cast<const btSphereShape*>(pShape);										// assume the shape is a sphere and typecast it		
		float radius = sphere->getMargin();																				// get the sphere's size from the shape		
		DrawSphere(radius);																								// draw the sphere
		break;
	}
	case CYLINDER_SHAPE_PROXYTYPE: {																					// Ch 6.3 - Draw Cylinder shape		
		const btCylinderShape* pCylinder = static_cast<const btCylinderShape*>(pShape);									// assume the object is a cylinder
		// get the relevant data
		float radius = pCylinder->getRadius();
		float halfHeight = pCylinder->getHalfExtentsWithMargin()[1];		
		DrawCylinder(radius,halfHeight);																				// draw the cylinder
		break;
	}
	case CONVEX_HULL_SHAPE_PROXYTYPE: DrawConvexHull(pShape); break;													// Ch 7.2 - draw the convex hull shape...whatever it is
	case COMPOUND_SHAPE_PROXYTYPE: {																					// Ch 7.3 - P95 (106)
		const btCompoundShape* pCompound = static_cast<const btCompoundShape*>(pShape);									// get the shape		
		for (int i = 0; i < pCompound->getNumChildShapes(); ++i) {														// iterate through the children
			// get the transform of the sub-shape
			btTransform thisTransform = pCompound->getChildTransform(i);
			btScalar thisMatrix[16];
			thisTransform.getOpenGLMatrix(thisMatrix);				
			DrawShape(thisMatrix, pCompound->getChildShape(i), color);													// call drawshape recursively for each child. The matrix stack takes care of positioning/orienting the object for us
		}
		break;
	}

	default: break;																										// unsupported type
	}
		
	glPopMatrix();																										// pop the stack
}

//GameObject* BulletOpenGLApplication::CreateGameObject(btCollisionShape* pShape, const float &mass, const btVector3 &color, const btVector3 &initialPosition, const btQuaternion &initialRotation) { // Ch 8 Removed
GameObject* BulletOpenGLApplication::CreateGameObject(btCollisionShape* pShape, const float &mass, const btVector3 &color, const btVector3 &initialPosition, short group, short mask, const btQuaternion &initialRotation, bool playAudio) {	// Ch8 Added
	GameObject* pObject = new GameObject(pShape, mass, color, initialPosition, initialRotation, false);					// create a new game object
		
	m_objects.push_back(pObject);																						// push it to the back of the list
		
	if (m_pWorld) {																										// check if the world object is valid		
		//m_pWorld->addRigidBody(pObject->GetRigidBody());																// Ch8 REMOVED add the object's rigid body to the world
		m_pWorld->addRigidBody(pObject->GetRigidBody(), group, mask);													// Ch8 ADDED
	}
	return pObject;
}

// Ch5.1
btVector3 BulletOpenGLApplication::GetPickingRay(int x, int y) {
	// calculate the field-of-view
	float tanFov = 1.0f / m_nearPlane;
	float fov = btScalar(2.0) * btAtan(tanFov);
	
	// get a ray pointing forward from the camera and extend it to the far plane
	btVector3 rayFrom = m_cameraPosition;
	btVector3 rayForward = (m_cameraTarget - m_cameraPosition);
	rayForward.normalize();
	rayForward*= m_farPlane;
	
	// find the horizontal and vertical vectors relative to the current camera view
	btVector3 ver = m_upVector;
	btVector3 hor = rayForward.cross(ver);
	hor.normalize();
	ver = hor.cross(rayForward);
	ver.normalize();
	hor *= 2.f * m_farPlane * tanFov;
	ver *= 2.f * m_farPlane * tanFov;
	
	btScalar aspect = m_screenWidth / (btScalar)m_screenHeight;															// calculate the aspect ratio
	
	// adjust the forward-ray based on the X/Y coordinates that were clicked
	hor*=aspect;
	btVector3 rayToCenter = rayFrom + rayForward;
	btVector3 dHor = hor * 1.f/float(m_screenWidth);
	btVector3 dVert = ver * 1.f/float(m_screenHeight);
	btVector3 rayTo = rayToCenter - 0.5f * hor + 0.5f * ver;
	rayTo += btScalar(x) * dHor;
	rayTo -= btScalar(y) * dVert;
		
	return rayTo;																										// return the final result
}

// Ch5.1
void BulletOpenGLApplication::ShootBox(const btVector3 &direction) {
	GameObject* pObject = CreateGameObject(new btBoxShape(btVector3(1, 1, 1)), 1, btVector3(0.4f, 0.f, 0.4f), m_cameraPosition);	// create a new box object
								
	btVector3 velocity = direction;																									// calculate the velocity
	velocity.normalize();
	velocity *= 25.0f;

	pObject->SetType(BULLET);																										// JOR Set the type as bullet

	pObject->GetRigidBody()->setLinearVelocity(velocity);																			// set the linear velocity of the box
}

void BulletOpenGLApplication::ShootBall(const btVector3 &direction) {

	// create game object (shape, mass, colour, initial position, group, mask, rotation)

	//GameObject* pObject = CreateGameObject(new btBoxShape(btVector3(.5, .5, .5)), 1, btVector3(0.4f, 0.f, 0.4f), m_cameraPosition);	// create a new box object
	GameObject* pObject = CreateGameObject(new btSphereShape(0.5),
		1.0f,																															// Mass
		btVector3(1.0f, 1.0f, 0.0f),																									// Colour
		m_cameraPosition, 																												// Start at Cameras position
		COLGROUP_BULLET,																												// Collision groups are box and bullet
		COLGROUP_BOX);

	pObject->SetPlayAudio(false);
	//pObject->SetName("bullet");
	pObject->SetType(BULLET);																											// JOR Set the type as bullet

	btVector3 velocity = direction;																										// calculate the velocity
	velocity.normalize();
	velocity *= 25.0f;

	pObject->GetRigidBody()->setLinearVelocity(velocity);																				// set the linear velocity of the box
}

// Ch5.1
//bool BulletOpenGLApplication::Raycast(const btVector3 &startPosition, const btVector3 &direction, RayResult &output) {					// Ch 6.3 Changed
bool BulletOpenGLApplication::Raycast(const btVector3 &startPosition, const btVector3 &direction, RayResult &output, bool includeStatic) {	// Ch 6.3
	if (!m_pWorld) return false;
		
	// get the picking ray from where we clicked
	btVector3 rayTo = direction;
	btVector3 rayFrom = m_cameraPosition;
			
	btCollisionWorld::ClosestRayResultCallback rayCallback(rayFrom,rayTo);												// create our raycast callback object

	m_pWorld->rayTest(rayFrom,rayTo,rayCallback);																		// perform the raycast
		
	// did we hit something?
	if (rayCallback.hasHit()) {		
		btRigidBody* pBody = (btRigidBody*)btRigidBody::upcast(rayCallback.m_collisionObject);							// if so, get the rigid body we hit
		if (!pBody) return false;
			
		// prevent us from picking objects like the ground plane
		if (!includeStatic)																								// skip this check if we want it to hit static objects
			if (pBody->isStaticObject() || pBody->isKinematicObject()) return false;									// prevent us from picking objects like the ground plane
	    
		// set the result data
		output.pBody = pBody;
		output.hitPoint = rayCallback.m_hitPointWorld;
		return true;
	}
		
	return false;																										// we didn't hit anything
}

// Ch5.1
void BulletOpenGLApplication::DestroyGameObject(btRigidBody* pBody) {
	// we need to search through the objects in order to find the corresponding iterator (can only erase from an std::vector by passing an iterator)
	for (GameObjects::iterator iter = m_objects.begin(); iter != m_objects.end(); ++iter) {
		if ((*iter)->GetRigidBody() == pBody) {
			GameObject* pObject = *iter;			
			m_pWorld->removeRigidBody(pObject->GetRigidBody());															// remove the rigid body from the world			
			m_objects.erase(iter);																						// erase the object from the list			
			delete pObject;																								// delete the object from memory			
			return;																										// done
		}
	}
}

// Ch5.2
void BulletOpenGLApplication::CreatePickingConstraint(int x, int y) {
	if (!m_pWorld) return;
	
	// perform a raycast and return if it fails
	RayResult output;
	if (!Raycast(m_cameraPosition, GetPickingRay(x, y), output)) return;
		
	m_pPickedBody = output.pBody;																						// store the body for future reference
			
	m_pPickedBody->setActivationState(DISABLE_DEACTIVATION);															// prevent the picked object from falling asleep
	
	btVector3 localPivot = m_pPickedBody->getCenterOfMassTransform().inverse() * output.hitPoint;						// get the hit position relative to the body we hit
	
	// create a transform for the pivot point
	btTransform pivot;
	pivot.setIdentity();
	pivot.setOrigin(localPivot);
	
	// P70 create our constraint object
	btGeneric6DofConstraint* dof6 = new btGeneric6DofConstraint(*m_pPickedBody, pivot, true);
	bool bLimitAngularMotion = true;
	if (bLimitAngularMotion) {
		dof6->setAngularLowerLimit(btVector3(0, 0, 0));
		dof6->setAngularUpperLimit(btVector3(0, 0, 0));		
	}
	
	// add the constraint to the world
	m_pWorld->addConstraint(dof6, true);																				// Inform the world of its existence
												
	m_pPickConstraint = dof6;																							// store a pointer to our constraint
			
	// define the 'strength' of our constraint (each axis)
	float cfm = 0.5f;
	dof6->setParam(BT_CONSTRAINT_STOP_CFM, cfm, 0);
	dof6->setParam(BT_CONSTRAINT_STOP_CFM, cfm, 1);
	dof6->setParam(BT_CONSTRAINT_STOP_CFM, cfm, 2);
	dof6->setParam(BT_CONSTRAINT_STOP_CFM, cfm, 3);
	dof6->setParam(BT_CONSTRAINT_STOP_CFM, cfm, 4);
	dof6->setParam(BT_CONSTRAINT_STOP_CFM, cfm, 5);
	
	// define the 'error reduction' of our constraint (each axis)
	float erp = 0.5f;
	dof6->setParam(BT_CONSTRAINT_STOP_ERP, erp, 0);
	dof6->setParam(BT_CONSTRAINT_STOP_ERP, erp, 1);
	dof6->setParam(BT_CONSTRAINT_STOP_ERP, erp, 2);
	dof6->setParam(BT_CONSTRAINT_STOP_ERP, erp, 3);
	dof6->setParam(BT_CONSTRAINT_STOP_ERP, erp, 4);
	dof6->setParam(BT_CONSTRAINT_STOP_ERP, erp, 5);
			
	m_oldPickingDist = (output.hitPoint - m_cameraPosition).length();													// save this data for future reference	
}

// Ch5.2
void BulletOpenGLApplication::RemovePickingConstraint() {	
	if (!m_pPickConstraint || !m_pWorld) return;																		// exit in erroneous situations
		
	m_pWorld->removeConstraint(m_pPickConstraint);																		// remove the constraint from the world
		
	delete m_pPickConstraint;																							// delete the constraint object
	
	// reactivate the body
	m_pPickedBody->forceActivationState(ACTIVE_TAG);
	m_pPickedBody->setDeactivationTime(0.f);
	
	// clear the pointers
	m_pPickConstraint = 0;
	m_pPickedBody = 0;	
}

// Ch 6.1
void BulletOpenGLApplication::CheckForCollisionEvents() {	
	CollisionPairs pairsThisUpdate;																						// keep a list of the collision pairs we found during the current update

	// iterate through all of the manifolds in the dispatcher
	for (int i = 0; i < m_pDispatcher->getNumManifolds(); ++i) {
		btPersistentManifold* pManifold = m_pDispatcher->getManifoldByIndexInternal(i);									// get the manifold
		
		// ignore manifolds that have no contact points.	
		if (pManifold->getNumContacts() > 0) {																			// Number of contacts detected between 2 bodies
			// get the two rigid bodies involved in the collision
			const btRigidBody* pBody0 = static_cast<const btRigidBody*>(pManifold->getBody0());							// Body in object pair that passed broad phase
			const btRigidBody* pBody1 = static_cast<const btRigidBody*>(pManifold->getBody1());							// Body in object pair that passed broad phase
    
			// always create the pair in a predictable order (use the pointer value..)
			bool const swapped = pBody0 > pBody1;
			const btRigidBody* pSortedBodyA = swapped ? pBody1 : pBody0;
			const btRigidBody* pSortedBodyB = swapped ? pBody0 : pBody1;
						
			CollisionPair thisPair = std::make_pair(pSortedBodyA, pSortedBodyB);										// create the pair
						
			pairsThisUpdate.insert(thisPair);																			// insert the pair into the current list

			// if this pair doesn't exist in the list from the previous update, it is a new pair and we must send a collision event
			if (m_pairsLastUpdate.find(thisPair) == m_pairsLastUpdate.end()) {
				CollisionEvent((btRigidBody*)pBody0, (btRigidBody*)pBody1);												// Collision event detected
			}
		}
	}
		
	CollisionPairs removedPairs;																						// create another list for pairs that were removed this update
	
	// this handy function gets the difference beween two sets. It takes the difference between
	// collision pairs from the last update, and this update and pushes them into the removed pairs list
	std::set_difference( m_pairsLastUpdate.begin(), m_pairsLastUpdate.end(),
	pairsThisUpdate.begin(), pairsThisUpdate.end(),
	std::inserter(removedPairs, removedPairs.begin()));
	
	// iterate through all of the removed pairs sending separation events for them
	for (CollisionPairs::const_iterator iter = removedPairs.begin(); iter != removedPairs.end(); ++iter) {
		SeparationEvent((btRigidBody*)iter->first, (btRigidBody*)iter->second);											// Separation event detected
	}
		
	m_pairsLastUpdate = pairsThisUpdate;																				// in the next iteration we'll want to compare against the pairs we found in this iteration
}

// Ch 6.1 Added, 6.2 Removed
void BulletOpenGLApplication::CollisionEvent(btRigidBody * pBody0, btRigidBody * pBody1) {
	
	// find the two colliding objects
	//GameObject* pObj0 = FindGameObject(pBody0);
	//GameObject* pObj1 = FindGameObject(pBody1);
		
	//if (!pObj0 || !pObj1) return;																						// exit if we didn't find anything


	// set their colors to white
	//pObj0->SetColor(btVector3(1.0,1.0,1.0));
	//pObj1->SetColor(btVector3(1.0,1.0,1.0));
}

// Ch 6.1 Added, 6.2 Removed
void BulletOpenGLApplication::SeparationEvent(btRigidBody * pBody0, btRigidBody * pBody1) {
	
	// get the two separating objects
	GameObject* pObj0 = FindGameObject((btRigidBody*)pBody0);
	GameObject* pObj1 = FindGameObject((btRigidBody*)pBody1);
		
	if (!pObj0 || !pObj1) return;																						// exit if we didn't find anything

	// set their colors to black
	//pObj0->SetColor(btVector3(0.0,0.0,0.0));
	//pObj1->SetColor(btVector3(0.0,0.0,0.0));
	

	//DestroyGameObject(pBody0);	// destroys all collidable objects

	/*
	//if (!pObj1->GetPlayAudio() || !pObj0->GetPlayAudio()) {
	if (!pObj1->GetPlayAudio()) {
		//if(!pObj0->GetPlayAudio())

		//Audio::Instance()->Fire1();
		Audio::Instance()->playFX("gunfireFX");

		pObj0->SetPlayAudio(true);
		pObj1->SetPlayAudio(true);
	}
	*/
	if (pObj0->GetType() == BULLET) {
		Audio::Instance()->playFX("gunfireFX");
		DestroyGameObject(pObj0->GetRigidBody());
	}
	if (pObj1->GetType() == BULLET) {
		Audio::Instance()->playFX("gunfireFX");
		DestroyGameObject(pObj1->GetRigidBody());
	}
}

// Ch 6.1
GameObject* BulletOpenGLApplication::FindGameObject(btRigidBody* pBody) {
	// search through our list of gameobjects finding the one with a rigid body that matches the given one
	for (GameObjects::iterator iter = m_objects.begin(); iter != m_objects.end(); ++iter) {
		if ((*iter)->GetRigidBody() == pBody) return *iter;																// found the body, so return the corresponding game object
	}
	return 0;
}

// Ch 7.1
void BulletOpenGLApplication::DrawSphere(const btScalar &radius) {
	// some constant values for more many segments to build the sphere from
	static int lateralSegments = 25;
	static int longitudinalSegments = 25;

	// iterate laterally
	for(int i = 0; i <= lateralSegments; i++) {
		// do a little math to find the angles of this segment
		btScalar lat0 = SIMD_PI * (-btScalar(0.5) + (btScalar) (i - 1) / lateralSegments);
		btScalar z0  = radius*sin(lat0);
		btScalar zr0 =  radius*cos(lat0);

		btScalar lat1 = SIMD_PI * (-btScalar(0.5) + (btScalar) i / lateralSegments);
		btScalar z1 = radius*sin(lat1);
		btScalar zr1 = radius*cos(lat1);
				
		glBegin(GL_QUAD_STRIP);																							// start rendering strips of quads (polygons with 4 poins)
		// iterate longitudinally
		for(int j = 0; j <= longitudinalSegments; j++) {
			// determine the points of the quad from the lateral angles
			btScalar lng = 2 * SIMD_PI * (btScalar) (j - 1) / longitudinalSegments;
			btScalar x = cos(lng);
			btScalar y = sin(lng);
			// draw the normals and vertices for each point in the quad since it is a STRIP of quads, 
			// we only need to add two points each time to create a whole new quad
			
			// calculate the normal
			btVector3 normal = btVector3(x*zr0, y*zr0, z0);
			normal.normalize();
			glNormal3f(normal.x(), normal.y(), normal.z());				
			glVertex3f(x * zr0, y * zr0, z0);																			// create the first vertex
			
			// calculate the next normal
			normal = btVector3(x*zr1, y*zr1, z1);
			normal.normalize();
			glNormal3f(normal.x(), normal.y(), normal.z());			
			glVertex3f(x * zr1, y * zr1, z1);																			// create the second vertex
			
			// and repeat...
		}
		glEnd();
	}
}

// Ch 7.1
void BulletOpenGLApplication::DrawCylinder(const btScalar &radius, const btScalar &halfHeight) {
	static int slices = 15;
	static int stacks = 10;
	// tweak the starting position of the cylinder to match the physics object
	glRotatef(-90.0, 1.0, 0.0, 0.0);
	glTranslatef(0.0, 0.0, -halfHeight);	
	GLUquadricObj *quadObj = gluNewQuadric();																			// create a quadric object to render with
	// set the draw style of the quadric
	gluQuadricDrawStyle(quadObj, (GLenum)GLU_FILL);
	gluQuadricNormals(quadObj, (GLenum)GLU_SMOOTH);	
	gluDisk(quadObj, 0, radius, slices, stacks);																		// create a disk to cap the cylinder	
	gluCylinder(quadObj, radius, radius, 2.f*halfHeight, slices, stacks);												// create the main hull of the cylinder (no caps)
	// shift the position and rotation again
	glTranslatef(0.0f, 0.0f, 2.f*halfHeight);
	glRotatef(-180.0f, 0.0f, 1.0f, 0.0f);	
	gluDisk(quadObj, 0, radius, slices, stacks);																		// draw the cap on the other end of the cylinder	
	gluDeleteQuadric(quadObj);																							// don't need the quadric anymore, so remove it  to save memory
}

// Ch 7.2
void BulletOpenGLApplication::DrawConvexHull(const btCollisionShape* shape) {
	// get the polyhedral data from the convex hull
	const btConvexPolyhedron* pPoly = shape->isPolyhedral() ? ((btPolyhedralConvexShape*) shape)->getConvexPolyhedron() : 0;
	if (!pPoly) return;
		
	glBegin (GL_TRIANGLES);																								// begin drawing triangles
	
	// iterate through all faces
	for (int i = 0; i < pPoly->m_faces.size(); i++) {			
		int numVerts = pPoly->m_faces[i].m_indices.size();																// get the indices for the face
		if (numVerts>2)	{	
			// iterate through all index triplets
			for (int v = 0; v <pPoly->m_faces[i].m_indices.size()-2;v++) {
				// grab the three vertices
				btVector3 v1 = pPoly->m_vertices[pPoly->m_faces[i].m_indices[0]];
				btVector3 v2 = pPoly->m_vertices[pPoly->m_faces[i].m_indices[v+1]];
				btVector3 v3 = pPoly->m_vertices[pPoly->m_faces[i].m_indices[v+2]];
				// calculate the normal
				btVector3 normal = (v3-v1).cross(v2-v1);
				normal.normalize ();
				// draw the triangle
				glNormal3f(normal.getX(),normal.getY(),normal.getZ());
				glVertex3f (v1.x(), v1.y(), v1.z());
				glVertex3f (v2.x(), v2.y(), v2.z());
				glVertex3f (v3.x(), v3.y(), v3.z());
			}
		}
	}
	
	glEnd ();	// done drawing
}
