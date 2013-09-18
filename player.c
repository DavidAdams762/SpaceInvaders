#include "player.h"
#include "util.h"
#include "globals.h"
#include "input.h"
#include "bullet.h"
#include "images.h"
#include "collider.h"
#include <math.h>

const vec2 PLAYER_BULLET_OFFSET = {30, 24};

player_t *player_make()
{
	player_t *self = (player_t *)malloc(sizeof(player_t));
	self->position = v2make(globals.window.width / 2 - 32, globals.window.height - 64);
	self->image = get_image("tank");
	self->width = self->image->w;
	self->height = self->image->h;
	self->moveSpeed = 500.0f;
	self->bulletSpeed = 500.0f;
	return self;
}

void _player_movement(player_t *self)
{
	if (input_get_key(SDLK_LEFT)) {
		self->position.x -= self->moveSpeed * globals.time.deltaf;
	}

	if (input_get_key(SDLK_RIGHT)) {
		self->position.x += self->moveSpeed * globals.time.deltaf;
	}

	self->position.x = clampf(self->position.x, 0, globals.window.width - 64);
}

void _player_shooting(player_t *self)
{
	if (input_get_key_down(SDLK_z)) {
		bullet_t *bullet = (bullet_t *)malloc(sizeof(bullet_t));
		bullet->position = v2add(self->position, PLAYER_BULLET_OFFSET);
		bullet->velocity = v2make(0.0f, -self->bulletSpeed);
		bullet->sourceType = BULLET_PLAYER_SOURCE;
		bullet->image = get_image("bullet");
		bullet->destroy = false;
		collider_init(&bullet->collider, bullet->position, 4, 4, COLLISION_TAG_PLAYER_BULLET, COLLISION_TAG_ENEMY);
		colliders_add(&bullet->collider);

		bullet_add(bullet);
	}
}

void player_update(player_t *self)
{
	assert(self);

	_player_movement(self);
	_player_shooting(self);	
}

void player_draw(player_t *self)
{
	assert(self);

	apply_surface(self->position.x, self->position.y, self->image, globals.screen);
}

