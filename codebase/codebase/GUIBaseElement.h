#ifndef __GUIBASEELEMENT__
#define __GUIBASEELEMENT__

// base class for GUI elements

#include <string>

struct RenderRectangle
{
	float left = 0.0f;
	float top = 0.0f;
	float right = 1.0f;
	float bottom = 1.0f;
};

class GUIBaseElement
{
public:
	virtual int Init() = 0;
	virtual int Exit() = 0;

	virtual void Events() = 0;
	virtual void Updates() = 0;
	virtual void Render() = 0;

	virtual RenderRectangle& getRenderRectangle() { return inWindowRect; }
	virtual void setRenderRectangle(RenderRectangle rect) { inWindowRect = rect; }

	virtual std::string getGUIName() { return GUIName; }
	virtual void setGUIName(std::string N) { GUIName = N; }

protected:
	std::string GUIName = "";
	// GUI rendering data
	RenderRectangle inWindowRect;
};

#endif // !__GUIBASEELEMENT__
