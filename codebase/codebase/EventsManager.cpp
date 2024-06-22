#include "EventsManager.h"

#include "Game.h"

EventsManager::EventsManager()
{
}

EventsManager::~EventsManager()
{
}

int EventsManager::Init()
{
	initState = 0;

	if (initState == 0)
	{
		// for now we'll just hardcode all of this because I haven't implemented json configs yet lol
		keyMap["A"] = SDL_SCANCODE_A;
		keyMap["B"] = SDL_SCANCODE_B;
		keyMap["C"] = SDL_SCANCODE_C;
		keyMap["D"] = SDL_SCANCODE_D;
		keyMap["E"] = SDL_SCANCODE_E;
		keyMap["F"] = SDL_SCANCODE_F;
		keyMap["G"] = SDL_SCANCODE_G;
		keyMap["H"] = SDL_SCANCODE_H;
		keyMap["I"] = SDL_SCANCODE_I;
		keyMap["J"] = SDL_SCANCODE_J;
		keyMap["K"] = SDL_SCANCODE_K;
		keyMap["L"] = SDL_SCANCODE_L;
		keyMap["M"] = SDL_SCANCODE_M;
		keyMap["N"] = SDL_SCANCODE_N;
		keyMap["O"] = SDL_SCANCODE_O;
		keyMap["P"] = SDL_SCANCODE_P;
		keyMap["Q"] = SDL_SCANCODE_Q;
		keyMap["R"] = SDL_SCANCODE_R;
		keyMap["S"] = SDL_SCANCODE_S;
		keyMap["T"] = SDL_SCANCODE_T;
		keyMap["U"] = SDL_SCANCODE_U;
		keyMap["V"] = SDL_SCANCODE_V;
		keyMap["W"] = SDL_SCANCODE_W;
		keyMap["X"] = SDL_SCANCODE_X;
		keyMap["Y"] = SDL_SCANCODE_Y;
		keyMap["Z"] = SDL_SCANCODE_Z;
		keyMap["1"] = SDL_SCANCODE_1;
		keyMap["2"] = SDL_SCANCODE_2;
		keyMap["3"] = SDL_SCANCODE_3;
		keyMap["4"] = SDL_SCANCODE_4;
		keyMap["5"] = SDL_SCANCODE_5;
		keyMap["6"] = SDL_SCANCODE_6;
		keyMap["7"] = SDL_SCANCODE_7;
		keyMap["8"] = SDL_SCANCODE_8;
		keyMap["9"] = SDL_SCANCODE_9;
		keyMap["0"] = SDL_SCANCODE_0;
		keyMap["ESC"] = SDL_SCANCODE_ESCAPE;
		keyMap["TAB"] = SDL_SCANCODE_TAB;
		keyMap["SPACE"] = SDL_SCANCODE_SPACE;

		mousePosition[DIR::X] = 0;
		mousePosition[DIR::Y] = 0;

		mouseWheelDirection[DIR::X] = 0;
		mouseWheelDirection[DIR::Y] = 0;
	}

	return initState;
}

int EventsManager::Exit()
{
	initState;

	return initState;
}

void EventsManager::Events()
{
	pressedKeys.clear();
	downKeys; // not reset because stores which keys are currently held down
	mousePosition[DIR::X] = 0;
	mousePosition[DIR::Y] = 0;
	pressedMouseButtons.clear();
	downMouseButtons; // not reset because stores which keys are currently held down
	mouseWheelDirection[DIR::X] = 0;
	mouseWheelDirection[DIR::Y] = 0;

	while (SDL_PollEvent(&currentEvent))
	{
		switch (currentEvent.type)
		{
		case SDL_QUIT:
		{
			Game::S().SetRunning(false);
			break;
		}
		case SDL_MOUSEMOTION:
		{
			mousePosition[DIR::X] = currentEvent.motion.x;
			mousePosition[DIR::Y] = currentEvent.motion.y;
			focusedWindowID = currentEvent.motion.windowID;
			break;
		}
		case SDL_MOUSEBUTTONDOWN:
		{
			Uint8 ButtonNumber = currentEvent.button.button;
			pressedMouseButtons.insert(ButtonNumber);
			downMouseButtons.insert(ButtonNumber);
			break;
		}
		case SDL_MOUSEBUTTONUP:
		{
			Uint8 ButtonNumber = currentEvent.button.button;
			//pressedMouseButtons.erase(ButtonNumber); // we don't want to remove this, this is cleared each frame.
			downMouseButtons.erase(ButtonNumber);
			break;
		}
		case SDL_KEYDOWN:
		{
			/*if (pressedKeys.size() > 3)
			{
				Game::S()->log(std::format("more than 5 keys were pressed at once, ignoring newer ones. ({})", std::to_string(currentEvent.key.keysym.scancode)));
				break;
			}*/
			SDL_Scancode downKeyScanCode = currentEvent.key.keysym.scancode;
			//std::string keyname = SDL_GetScancodeName(downKeyScanCode);
			pressedKeys.insert(downKeyScanCode);
			downKeys.insert(downKeyScanCode);
			if (isTextInputting())
			{
				switch (downKeyScanCode)
				{
				case SDL_SCANCODE_BACKSPACE:
				{
					if (finishedTextInput.length() > 0)
					{
						finishedTextInput.pop_back();
					}
					break;
				}
				case SDL_SCANCODE_RETURN:
				{
					finishedTextInput += "\n";
					break;
				}
				case SDL_SCANCODE_ESCAPE:
				{
					stopTextInput();
					break;
				}
				default:
				{
					break;
				}
				}
			}
			break;
		}
		case SDL_KEYUP:
		{
			SDL_Scancode upKeyScanCode = currentEvent.key.keysym.scancode;
			std::string keyname = SDL_GetScancodeName(upKeyScanCode);
			//pressedKeys.erase(upKeyScanCode);
			downKeys.erase(upKeyScanCode);
			break;
		}
		case SDL_WINDOWEVENT:
		{
			Game::S().GRAPHICS.getGameWindow().SDLEvents(&currentEvent);
			break;
		}
		case SDL_TEXTINPUT:
		{
			// the finished text ?
			// probably in letter-by-letter input ?
			finishedTextInput += currentEvent.text.text;
			break;
		}
		case SDL_TEXTEDITING:
		{
			unfinishedTextInput = currentEvent.edit.text;
			break;
		}
		case SDL_MOUSEWHEEL:
		{
			mouseWheelDirection[DIR::X] = currentEvent.wheel.x;
			mouseWheelDirection[DIR::Y] = currentEvent.wheel.y;
			break;
		}
		case SDL_RENDER_TARGETS_RESET:
		{
			// reset GraphicManager buffer
			// means all customly created textures need to be deleted! (reset all buffers)
			break;
		}
		case SDL_RENDER_DEVICE_RESET:
		{
			// reset GraphicManager buffer
			// means all customly created textures need to be deleted! (reset all buffers) ... this event shouldn't happen iirc
			break;
		}
		default:
		{
			// other unhandled events
			break;
		}
		}
	}
}

void EventsManager::Updates()
{
}

void EventsManager::Render()
{
}

bool EventsManager::isKeyPressed(std::string KeyName)
{
	if (!keyMap.contains(KeyName))
	{
		return false; // can't know which key is meant.
	}
	return pressedKeys.contains(keyMap[KeyName]);
}

bool EventsManager::isKeyDown(std::string KeyName)
{
	if (!keyMap.contains(KeyName))
	{
		return false; // can't know which key is meant.
	}
	return downKeys.contains(keyMap[KeyName]);
}

bool EventsManager::isButtonPressed(Uint8 ButtonName)
{
	return pressedMouseButtons.contains(ButtonName);
}

bool EventsManager::isButtonDown(Uint8 ButtonName)
{
	return downMouseButtons.contains(ButtonName);
}

std::array<Sint32, 2> EventsManager::getMousePosition()
{
	return mousePosition;
}

Uint32 EventsManager::getFocusedWindow()
{
	return focusedWindowID;
}

std::array<Sint32, 2> EventsManager::getMouseWheelDirection()
{
	return mouseWheelDirection;
}

void EventsManager::startTextInput()
{
	textInputting = true;
	finishedTextInput = "";
	SDL_StartTextInput();
}

void EventsManager::stopTextInput()
{
	SDL_StopTextInput();
	textInputting = false;
}

void EventsManager::setInputText(std::string T)
{
	finishedTextInput = T;
}

std::string EventsManager::getInputText()
{
	return finishedTextInput;
}

bool EventsManager::isTextInputting()
{
	return textInputting;
}
