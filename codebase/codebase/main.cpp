
#define SOL_ALL_SAFETIES_ON 1
#define SOL_LUAJIT 1

#include "sol/sol.hpp"
#include <SDL.h>

int main(int argc, char* argv[])
{
	sol::state lua;

	lua.open_libraries(sol::lib::base);

	lua.script("print('lua loaded! wohoo')");
	lua.script("print('but jokes aside, lua needs to be moved to wherever it has to go')");

	return 0;
}
