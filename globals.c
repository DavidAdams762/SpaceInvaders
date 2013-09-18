#include <SDL/SDL.h>
#include "globals.h"
#include "util.h"

struct globals_t globals;

void init_globals()
{
	globals.window.width = 800;
	globals.window.height = 600;
	globals.window.bpp = 32;

	globals.screen = NULL;
	
	globals.bRun = true;

	globals.time.start = SDL_GetTicks();
	globals.time.sinceStart = 0;
	globals.time.delta = 0;
	globals.time.deltaf = 0.0f;
	globals.time.last = 0;
}
