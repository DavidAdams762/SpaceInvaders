#ifndef _VADERS_COLLIDER_H_
#define _VADERS_COLLIDER_H_

#include "util.h"

#define COLLISION_TAG_PLAYER 			0x1
#define COLLISION_TAG_PLAYER_BULLET 	0x2
#define COLLISION_TAG_ENEMY				0x4
#define COLLISION_TAG_ENEMY_BULLET		0x8

static const int MAX_COLLIDERS = 100;

typedef struct 
{
	rect bounds;
	int tag;
	int collidesWith;
	bool colliding;
	bool enabled;
} collider_t;

void collider_init(collider_t *self, vec2 pos, int width, int height, int tag, int collflag);
void collider_check_collision(collider_t *self, collider_t *other);

collider_t **colliders;
stack colliderFreeIndices;

void colliders_init();
void colliders_add(collider_t *self);
void colliders_remove(collider_t *self);
void colliders_update();
void colliders_debug_draw();
int colliders_next_index();

#endif