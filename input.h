#ifndef _VADERS_INPUT_H_
#define _VADERS_INPUT_H_

#include <SDL/SDL.h>
#include "util.h"

Uint8 *oldKeystates;
Uint8 *newKeystates;

void input_init();
void input_cleanup();
void input_update();
bool input_get_key(SDLKey key);
bool input_get_key_down(SDLKey key);
bool input_get_key_up(SDLKey key);

#endif