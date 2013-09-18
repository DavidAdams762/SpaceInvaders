#ifndef _VADERS_PLAYER_H_
#define _VADERS_PLAYER_H_

#include <SDL/SDL.h>
#include "globals.h"
#include "util.h"

typedef struct
{
	vec2 position;
	SDL_Surface *image;
	int width, height;
	float moveSpeed;
	float bulletSpeed;
} player_t;

player_t *player_make();
void _player_movement(player_t *self);
void _player_shooting(player_t *self);
void player_update(player_t *self);
void player_draw(player_t *self);

#endif