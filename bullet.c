#include <SDL/SDL.h>
#include "globals.h"
#include "util.h"
#include "bullet.h"
#include "collider.h"

const float BULLET_DESTROY_HEIGHT = -5.0f;

void bullet_update(bullet_t *self)
{
	assert(self);
	self->position = v2add(self->position, v2scale(self->velocity, globals.time.deltaf));

	self->collider.bounds.x = self->position.x;
	self->collider.bounds.y = self->position.y;

	if (self->collider.colliding) {
		self->destroy = true;
	}

	if (self->position.y <= BULLET_DESTROY_HEIGHT) {
		self->destroy = true;
	}
}

void bullet_draw(bullet_t *self)
{
	assert(self && self->image);
	apply_surface(self->position.x, self->position.y, self->image, globals.screen);
}

bool bullet_add(bullet_t *self)
{
	assert(self);

	int index = bullet_next_index();

	//this allows for gracefully not adding bullets
	if (index < 0) {
		return false;
	}

	bullets[index] = self;

	return true;
}

void bullet_remove(int index)
{
	assert(index >= 0 && index < MAX_BULLETS);

	if (bullets[index]) {
		colliders_remove(&bullets[index]->collider);
		free(bullets[index]);
		bullets[index] = NULL;

		stack_push(&freeIndices, (void *)&index);
	}
}

int bullet_next_index()
{
	int result;

	if (stack_size(&freeIndices) > 0) {
		stack_pop(&freeIndices, &result);
	} else {
		result = -1;
	}

	return result;
}

void bullets_init()
{
	bullets = (bullet_t **)malloc(MAX_BULLETS * sizeof(bullet_t *));
	stack_init(&freeIndices, MAX_BULLETS, sizeof(int), NULL);

	for (int i = 0; i < MAX_BULLETS; ++i) {
		bullets[i] = NULL;
		stack_push(&freeIndices, (void *)&i);
	}	
}

void bullets_update()
{
	int i;

	for (i = 0; i < MAX_BULLETS; ++i) {
		if (bullets[i]) {
			bullet_update(bullets[i]);
		}
	}

	for (i = 0; i < MAX_BULLETS; ++i) {
		if (bullets[i] && bullets[i]->destroy) {
			bullet_remove(i);
		}
	}
}

void bullets_draw()
{
	for (int i = 0; i < MAX_BULLETS; ++i) {
		if (bullets[i]) {
			bullet_draw(bullets[i]);
		}
	}
}

void bullets_cleanup()
{
	for (int i = 0; i < MAX_BULLETS; ++i) {
		if (bullets[i]) {
			free(bullets[i]);
			bullets[i] = NULL;
		}
	}

	free(bullets);
}
