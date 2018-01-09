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