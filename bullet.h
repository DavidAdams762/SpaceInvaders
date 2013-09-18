#ifndef _VADERS_BULLET_H_
#define _VADERS_BULLET_H_

#include <SDL/SDL.h>
#include "globals.h"
#include "util.h"
#include "collider.h"

static const int MAX_BULLETS = 20;

static const int BULLET_PLAYER_SOURCE = 0;
static const int BULLET_ENEMY_SOURCE = 1;

typedef struct
{
	vec2 position;
	vec2 velocity;
	collider_t collider;
	int sourceType;
	SDL_Surface *image;
	bool destroy;
} bullet_t;

void bullet_update(bullet_t *self);
void bullet_draw(bullet_t *bself);

bullet_t **bullets;
stack freeIndices;

bool bullet_add(bullet_t *self);
void bullet_remove(int index);
int bullet_next_index();

void bullets_init();
void bullets_update();
void bullets_draw();
void bullets_cleanup();

#endif