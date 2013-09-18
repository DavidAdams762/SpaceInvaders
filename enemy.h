#ifndef _VADERS_ENEMY_H_
#define _VADERS_ENEMY_H_

#include <SDL/SDL.h>
#include "globals.h"
#include "util.h"
#include "collider.h"

static const int MAX_ENEMIES = 30;
static const float MAX_MOVE_DELAY = 1.5f;
static const float MIN_MOVE_DELAY = 0.025f;
static const float HORIZONTAL_MOVEMENT_PER_TICK = 24.0f;
static const float VERTICAL_MOVEMENT_PER_TICK = 24.0f;

typedef struct
{
	vec2 position;
	collider_t collider;
	SDL_Surface *image;
	bool dead;
} enemy_t;

enemy_t **enemies;
float enemiesLeft;
float enemiesRight;
float enemiesTop;
float enemiesBottom;
float moveDelay;
float moveDelayTimer;
int enemiesAlive;
bool enemiesMoveRight;

void enemy_update(enemy_t *self);
void enemy_move(enemy_t *self, vec2 dist);
void enemy_draw(enemy_t *self);
void enemy_kill(enemy_t *self);

void enemies_init();
void enemies_update();
void enemies_update_bounds();
void enemies_update_move_delay();
void enemies_move(vec2 dist);
vec2 enemies_calc_move();
void enemies_draw();
void enemies_cleanup();

void enemies_reset_formation();

#endif