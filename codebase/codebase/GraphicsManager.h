#ifndef __GRAPHICSMANAGER__
#define __GRAPHICSMANAGER__

#include <SDL.h>

#include "Window.h"

class GraphicsManager
{
public:
	GraphicsManager();
	~GraphicsManager();

	int Init();
	int Exit();

	void Event();
	void Update();
	void Render();

private:
	int initState = -1;
	// Window data
	Window gameWindow;
};

#endif // !__GRAPHICMANAGER__
