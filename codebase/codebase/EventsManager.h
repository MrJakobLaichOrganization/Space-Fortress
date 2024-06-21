#ifndef __EVENTSMANAGER__
#define __EVENTSMANAGER__

#include <SDL.h>

#include <string>
#include <set>
#include <map>
#include <array>

enum MB // MouseButton
{
	LEFT = 0,
	MIDDLE = 1, // not sure if this is part of the button events of wheel events tbh
	RIGHT = 2
};

enum DIR // Direction
{
	HORIZONTAL = 0,
	VERTICAL = 1,
	X = HORIZONTAL,
	Y = VERTICAL
};

class EventsManager
{
public:
	EventsManager();
	~EventsManager();

	int Init();
	int Exit();

	void Events();
	void Updates();
	void Render();

	bool isKeyPressed(std::string KeyName);
	bool isKeyDown(std::string KeyName);

	bool isButtonPressed(Uint8 ButtonName);
	bool isButtonDown(Uint8 ButtonName);

	std::array<Sint32, 2> getMousePosition();
	Uint32 getFocusedWindow();

	std::array<Sint32, 2> getMouseWheelDirection();

	void startTextInput();
	void stopTextInput();
	void setInputText(std::string T = "");
	std::string getInputText();
	bool isTextInputting();

private:
	int initState = -1;
	SDL_Event currentEvent;
	// KEYBOARD KEYS
	std::set<SDL_Scancode> pressedKeys;
	std::set<SDL_Scancode> downKeys;
	std::map<std::string, SDL_Scancode> keyMap; // maps string to keyboard scancodes (see scancodes.txt file for reference)
	// MOUSE POSITION
	std::array<Sint32, 2> mousePosition = { 0, 0 };
	Uint32 focusedWindowID = 0;
	// MOUSE BUTTONS
	std::set<Uint8> pressedMouseButtons;
	std::set<Uint8> downMouseButtons;
	// MOUSE WHEEL
	std::array<Sint32, 2> mouseWheelDirection = { 0, 0 };
	// Text input data
	std::string finishedTextInput = "";
	std::string unfinishedTextInput = ""; // this is currently unused, I forgot what it could be used for tbh LMAO
	bool textInputting = false;
};

#endif // !__EVENTSMANAGER__
