#include "util.h"

SDL_Surface *load_image(const char* filename)
{
	SDL_Surface *loadedImage = NULL;
	SDL_Surface *optimizedImage = NULL;

	if (!filename) return NULL;
		
	loadedImage = IMG_Load(filename);

	if (loadedImage != NULL) {
		optimizedImage = SDL_DisplayFormatAlpha(loadedImage);
		SDL_FreeSurface(loadedImage);
	}

	return optimizedImage;
}

void apply_surface(int x, int y, SDL_Surface *source, SDL_Surface *destination)
{
	SDL_Rect offset;

	if (source == NULL || destination == NULL) {
		return;
	}

	offset.x = x;
	offset.y = y;

	SDL_BlitSurface(source, NULL, destination, &offset);
}

vec2 v2make(float x, float y)
{
	vec2 v; v.x = x; v.y = y;
	return v;
}

vec2 v2add(vec2 v1, vec2 v2)
{
	return v2make(v1.x + v2.x, v1.y + v2.y);
}

vec2 v2sub(vec2 v1, vec2 v2)
{
	return v2make(v1.x - v2.x, v1.y - v2.y);
}

vec2 v2mul(vec2 v1, vec2 v2)
{
	return v2make(v1.x * v2.x, v1.y * v2.y);
}

vec2 v2div(vec2 v1, vec2 v2)
{
	return v2make(v1.x / v2.x, v1.y / v2.y);
}

vec2 v2scale(vec2 vec, float scalar)
{
	return v2make(vec.x * scalar, vec.y * scalar);
}

float v2mag(vec2 vec)
{
	return sqrtf(SQR(vec.x) + SQR(vec.y));
}

vec2 v2norm(vec2 vec)
{
	float mag = v2mag(vec);
	assert(mag != 0);
	return v2make(vec.x / mag, vec.y / mag);
}

vec2 v2perp(vec2 vec)
{
	return v2make(-vec.y, vec.x);
}

vec2 v2neg(vec2 vec)
{
	return v2make(-vec.x, -vec.y);
}

point pointmake(int x, int y)
{
	point p; p.x = x; p.y = y;
	return p;
}

point point_from_vec2(vec2 vec)
{
	point p; p.x = (int)vec.x; p.y = (int)vec.y;
	return p;
}

float clampf(float val, float min, float max)
{
	if (val < min) val = min;
	if (val > max) val = max;
	return val;
}

rect rect_make(int x, int y, int width, int height)
{
	rect newrect = {x, y, width, height};
	return newrect;
}

rect rect_copy(rect other)
{
	rect newrect = {other.x, other.y, other.w, other.h};
	return newrect;
}

int rect_left(rect r)
{
	return r.x;
}

int rect_right(rect r)
{
	return r.x + r.w;
}

int rect_top(rect r)
{
	return r.y;
}

int rect_bottom(rect r)
{
	return r.y + r.h;
}

circle circle_make(int x, int y, int radius)
{
	circle newcirc = {x, y, radius};
	return newcirc;
}

circle circle_copy(circle other)
{
	circle newcirc = {other.x, other.y, other.r};
	return newcirc;
}

bool intersect_rr(rect r1, rect r2)
{
	int r1l = rect_left(r1), r1r = rect_right(r1), r1t = rect_top(r1), r1b = rect_bottom(r1);
	int r2l = rect_left(r2), r2r = rect_right(r2), r2t = rect_top(r2), r2b = rect_bottom(r2);

	return (r1r >= r2l && r1l <= r2r) && (r1b >= r2t && r1t <= r2b);	
}

bool intersect_rc(rect r, circle c)
{
	int rl = rect_left(r) - c.r, 
		rr = rect_right(r) + c.r, 
		rt = rect_top(r) - c.r, 
		rb = rect_bottom(r) + c.r;

	return (c.x >= rl && c.x <= rr && c.y >= rt && c.y <= rb);
}

bool intersect_cr(circle c, rect r)
{
	return intersect_rc(r, c);
}

bool intersect_cc(circle c1, circle c2)
{
	return (SQR(c1.x - c2.x) + SQR(c1.y - c2.y) <= SQR(c1.r + c2.r));
}

void stack_init(stack *self, int size, int elemSize, clearmem clearFunc)
{
	self->capacity = size;
	self->elemSize = elemSize;
	self->head = -1;
	self->elements = malloc(self->capacity * self->elemSize);
	self->clearFunction = clearFunc;
}

void stack_push(stack *self, void *elem)
{
	self->head++;
	assert(self->head < self->capacity);

	void *addr = (uint8_t *)self->elements + self->head * self->elemSize;
	memcpy(addr, elem, self->elemSize);
}

void stack_pop(stack *self, void *destination)
{
	assert(self->head >= 0);
	self->head--;

	void *addr = (uint8_t *)self->elements + (self->head + 1) * self->elemSize;
	memcpy(destination, addr, self->elemSize);
}

void stack_peek(stack *self, void *destination)
{
	assert(self->head >= 0);

	void *addr = (uint8_t *)self->elements + (self->head + 1) * self->elemSize;
	memcpy(destination, addr, self->elemSize);
}

void stack_free(stack *self)
{
	if (self->clearFunction != NULL) {
		while (self->head >= 0) {
			self->head--;
			void *addr = (uint8_t *)self->elements + self->head * self->elemSize;
			self->clearFunction(addr);
		}
	}

	free(self->elements);
}

uint32_t stack_size(stack *self) {
	return self->head + 1;
}
