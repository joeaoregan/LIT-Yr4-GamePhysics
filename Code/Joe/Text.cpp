#include "Text.h"
//#include "../Added/GLDebugFont.h"

Text* Text::s_pInstance;			// Singleton so only one instance of Audio exists in the game, for easy access

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

// JOR
void Text::DisplayText(float x, float y, int r, int g, int b, const char *string) {
	//glColor3f(r, g, b);
	//glColor3f(0, 0, 0);
	glColor3f(1, 1, 1);
	//glRasterPos2f(x, y);
	glRasterPos3f(x, y, 0);
	//glRasterPos2f(m_screenWidth + x, y);
	for (int i = 0; i < strlen(string); i++) {
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, string[i]);
	}
}

// JOR - From DemoApplication.cpp
void Text::DisplayString(int x, int y, char* message) {
	//glRasterPos3f(btScalar(x), btScalar(y), btScalar(0));
	glRasterPos3f(10, 10, 20);
	GLDebugDrawString(x, y, message);
}

// Function taken from the freeGLUT examples
void Text::PrintText(int nX, int nY, char* pszText) {
	int lines;
	char *p;

	// Prepare the OpenGL state
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	// Have an orthogonal projection matrix set
	glOrtho(0, glutGet(GLUT_WINDOW_WIDTH),
		0, glutGet(GLUT_WINDOW_HEIGHT),
		-1, +1
	);

	// Now the matrix mode
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	// Now the main text
	//glColor3ub(0, 0, 0);
	glColor3ub(255, 0, 0);	// RGB Colour Values: Red
	glRasterPos2i(nX, nY);

	for (p = pszText, lines = 0; *p; p++) {
		if (*p == '\n') {
			lines++;
			glRasterPos2i(nX, nY - (lines * 18));
		}

		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *p);
	}

	// Revert to the old matrix modes
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	// Restore the old OpenGL states
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
}
