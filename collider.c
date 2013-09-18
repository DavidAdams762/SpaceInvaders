#include "collider.h"
#include "util.h"
#include "globals.h"


#include <SDL/SDL_gfxPrimitives.h>

void collider_init(collider_t *self, vec2 pos, int width, int height, int tag, int collflag)
{
	self->bounds = rect_make(pos.x, pos.y, width, height);
	self->tag = tag;
	self->collidesWith = collflag;
	self->colliding = false;
	self->enabled = true;
}

void collider_check_collision(collider_t *self, collider_t *other)
{
	assert(self && other);

	if ((self->collidesWith & other->tag) > 0) {
		if (self->enabled && other->enabled) {
			if (intersect_rr(self->bounds, other->bounds)) {
				self->colliding = true;
			}
		}
	}
}

void colliders_init()
{
	colliders = (collider_t **)malloc(sizeof(collider_t *) * MAX_COLLIDERS);
	memset(colliders, 0, MAX_COLLIDERS * sizeof(collider_t *));
	stack_init(&colliderFreeIndices, MAX_COLLIDERS, sizeof(int), NULL);

	for (int i = 0; i < MAX_COLLIDERS; ++i) {
		colliders[i] = NULL;
		stack_push(&colliderFreeIndices, (void *)&i);
	}
}

void colliders_add(collider_t *self)
{
	assert(self);

	int index = colliders_next_index();
	
	//if we run out of space for colliders crashing is preferable
	assert(index >= 0);

	colliders[index] = self;
}

int colliders_next_index()
{
	int result = -1;

	if (stack_size(&colliderFreeIndices) > 0) {
		stack_pop(&colliderFreeIndices, &result);
	}

	if (result == -1) {
		fprintf(stderr, "Error: %d\n", stack_size(&colliderFreeIndices));
	}

	return result;
}

void colliders_remove(collider_t *self)
{
	assert(self);

	for (int i = 0; i < MAX_COLLIDERS; ++i) {
		if (self == colliders[i]) {
			colliders[i] = NULL;
			stack_push(&colliderFreeIndices, (void *)&i);
			return;
		}
	}
}

void colliders_update()
{
	for (int i = 0; i < MAX_COLLIDERS; ++i) {
		for (int j = 0; j < MAX_COLLIDERS; ++j) {
			if (i != j) {
				if (colliders[i] && colliders[j]) {
					collider_check_collision(colliders[i], colliders[j]);
				}
			}
		}
	}
}

void colliders_debug_draw()
{
	for (int i = 0; i < MAX_COLLIDERS; ++i) {
		if (colliders[i]) {
			int x1 = colliders[i]->bounds.x;
			int y1 = colliders[i]->bounds.y;
			int x2 = x1 + colliders[i]->bounds.w;
			int y2 = y1 + colliders[i]->bounds.h;
			rectangleRGBA(globals.screen, x1, y1, x2, y2, 255, 0, 0, 255);
		}
	}
}
