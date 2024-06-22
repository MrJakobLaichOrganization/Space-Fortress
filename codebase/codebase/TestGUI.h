#ifndef __TESTGUI__
#define __TESTGUI__

#include <SDL.h>

#include "GUIBaseElement.h"

class TestGUI : public GUIBaseElement
{
public:
	TestGUI();
	~TestGUI();

	int Init();
	int Exit();

	void Events();
	void Updates();
	void Render();

private:
	int initState = -1;
	// GUI render stuff
	SDL_Texture* testTexture = NULL;
};

#endif // !__TESTGUI__
