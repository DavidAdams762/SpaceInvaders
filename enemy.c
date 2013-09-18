#include <SDL/SDL.h>
#include <float.h>
#include "globals.h"
#include "util.h"
#include "enemy.h"
#include "images.h"
#include "collider.h"

void enemy_update(enemy_t *self)
{
	assert(self);

	if (self->collider.colliding) {
		enemy_kill(self);
	}
}

void enemy_move(enemy_t *self, vec2 dist)
{
	assert(self);

	self->position = v2add(self->position, dist);
	self->collider.bounds.x = self->position.x;
	self->collider.bounds.y = self->position.y;
}

void enemy_kill(enemy_t *self)
{
	if (!self->dead) {
		self->dead = true;
		self->collider.enabled = false;
		enemies_update_bounds();
		enemies_update_move_delay();
	}
}

void enemy_draw(enemy_t *self)
{
	assert(self && self->image);
	apply_surface(self->position.x, self->position.y, self->image, globals.screen);
}


void enemies_init()
{
	enemies = (enemy_t **)malloc(MAX_ENEMIES * sizeof(enemy_t *));
	memset(enemies, 0, MAX_ENEMIES * sizeof(enemy_t *));
	enemies_reset_formation();
}

void enemies_reset_formation()
{
	for (int i = 0; i < MAX_ENEMIES; ++i) {
		if (enemies[i] == NULL) {
			enemy_t *newEnemy = (enemy_t *)malloc(sizeof(enemy_t));
			enemies[i] = newEnemy;
		}

		enemy_t *e = enemies[i];
		float x = (i % 10) * 72.0f + 2;
		float y = (i / 10) * 72.0f;
		e->position = v2make(x, y);
		collider_init(&e->collider, e->position, 48, 48, COLLISION_TAG_ENEMY, COLLISION_TAG_PLAYER_BULLET);
		colliders_add(&e->collider);
		e->dead = false;

		e->image = get_image("enemy1");
	}

	enemies_update_bounds();
	enemies_update_move_delay();
	moveDelayTimer = moveDelay;
	enemiesMoveRight = true;
}

void enemies_update()
{
	for (int i = 0; i < MAX_ENEMIES; ++i) {
		if (enemies[i] && !enemies[i]->dead) {
			enemy_update(enemies[i]);
		}	
	}

	moveDelayTimer -= globals.time.deltaf;
	if (moveDelayTimer <= 0.0f) {
		moveDelayTimer = moveDelay;
		enemies_move(enemies_calc_move());
	}
}

void enemies_move(vec2 dist)
{
	for (int i = 0; i < MAX_ENEMIES; ++i) {
		if (enemies[i] && !enemies[i]->dead) {
			enemy_move(enemies[i], dist);
		}
	}

	enemies_update_bounds();
}

vec2 enemies_calc_move()
{
	vec2 result = v2make(0, 0);
	if (enemiesMoveRight) {
		if (enemiesRight + HORIZONTAL_MOVEMENT_PER_TICK > globals.window.width) {
			enemiesMoveRight = false;
			result.y = VERTICAL_MOVEMENT_PER_TICK;
		} else {
			result.x = HORIZONTAL_MOVEMENT_PER_TICK;
		}
	} else {
		if (enemiesLeft - HORIZONTAL_MOVEMENT_PER_TICK < 0) {
			enemiesMoveRight = true;
			result.y = VERTICAL_MOVEMENT_PER_TICK;
		} else {
			result.x = -HORIZONTAL_MOVEMENT_PER_TICK;
		}
	}

	return result;
}

void enemies_update_bounds()
{
	float minX = FLT_MAX;
	float minY = FLT_MAX;
	float maxX = -FLT_MAX;
	float maxY = -FLT_MAX;

	for (int i = 0; i < MAX_ENEMIES; ++i) {
		enemy_t *e = enemies[i];
		
		if (!e->dead) {
			if (e->position.x < minX) {
				minX = e->position.x;
			}		
			if (e->position.x + 48 > maxX) {
				maxX = e->position.x + 48;
			}
			if (e->position.y < minY) {
				minY = e->position.y;
			}
			if (e->position.y > maxY + 48) {
				maxY = e->position.y + 48;
			}
		}
	}

	enemiesLeft = minX;
	enemiesRight = maxX;
	enemiesTop = minY;
	enemiesBottom = maxY;
}

void enemies_update_move_delay()
{
	enemiesAlive = 0;
	for (int i = 0; i < MAX_ENEMIES; ++i) {
		if (!enemies[i]->dead) {
			enemiesAlive++;
		}
	}

	moveDelay = MIN_MOVE_DELAY + (MAX_MOVE_DELAY - MIN_MOVE_DELAY) * ((float)enemiesAlive / (float)MAX_ENEMIES);
}

void enemies_draw()
{
	int i = 0;
	for (i = 0; i < MAX_ENEMIES; ++i) {
		if (enemies[i] && !enemies[i]->dead) {
			enemy_draw(enemies[i]);
		}
	}
}

void enemies_cleanup()
{
	for (int i = 0; i < MAX_ENEMIES; ++i) {
		if (enemies[i]) {
			free(enemies[i]);
			enemies[i] = NULL;
		}
	}

	free(enemies);
}