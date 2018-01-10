#include "../Added/GLDebugFont.h"

//#include "btBulletDynamicsCommon.h"
#include "../BulletOpenGLApplication.h"

class Text {
public:
	// Text singleton
	static Text* Instance() {
		if (s_pInstance == 0) {
			s_pInstance = new Text();
			return s_pInstance;
		}
		return s_pInstance;
	}

	void DisplayText(float x, float y, int r, int g, int b, const char *string);					// JOR

	void DisplayString(int x, int y, char* message);

	void PrintText(int nX, int nY, char* pszText);													// One example, freeGLUT

private:
	static Text* s_pInstance;
};