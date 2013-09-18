#ifndef _VADERS_GLOBAL_H_
#define _VADERS_GLOBAL_H_

#include <SDL/SDL.h>
#include "util.h"

struct globals_t
{
	struct window_t
	{
		int width;
		int height;
		int bpp;
	} window;
	
	SDL_Surface *screen;
	bool bRun;

	struct time_t
	{
		long start;
		long sinceStart;
		long delta;
		float deltaf;
		long last;
	} time;
};

void init_globals();

extern struct globals_t globals;

#endif

