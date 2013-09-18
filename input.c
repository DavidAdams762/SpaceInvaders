#include "input.h"
#include "util.h"

void input_init()
{
	oldKeystates = (Uint8 *)malloc(sizeof(SDLKey) * SDLK_LAST);
	newKeystates = SDL_GetKeyState(NULL);
}

void input_cleanup()
{
	free(oldKeystates);
}

void input_update()
{
	int i;
	for (i = 0; i < SDLK_LAST; i++)
		oldKeystates[i] = newKeystates[i];
}

bool input_get_key(SDLKey key)
{
	return newKeystates[key];
}

bool input_get_key_down(SDLKey key)
{
	return (newKeystates[key] && !oldKeystates[key]);
}

bool input_get_key_up(SDLKey key)
{
	return (!newKeystates[key] && oldKeystates[key]);
}
