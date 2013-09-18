/*
	Big file of various convenience functions and macros
*/

#ifndef _VADERS_UTIL_H_
#define _VADERS_UTIL_H_

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <math.h>
#include <assert.h>
#include <stdint.h>

#define bool int
#define true 1
#define false 0

#define SQR(x) ((x)*(x))

#define PI			3.14159265351f
#define PIOVER2 	1.57079632679f
#define TWOPIE 		6.28318530718f
#define RAD_TO_DEG 	57.29577951308f
#define DEG_TO_RAD 	0.01745329252f

typedef struct
{
	float x, y;
} vec2;

typedef struct
{
	int x, y;
} point;

typedef struct  
{
	int x, y, w, h;
} rect;

typedef struct
{
	int x, y, r;
} circle;

typedef void (*clearmem)(void *);

typedef struct
{	
	int head;
	int capacity;
	int elemSize;
	void *elements;
	clearmem clearFunction;
} stack;

SDL_Surface *load_image(const char* filename);
void apply_surface(int x, int y, SDL_Surface *source, SDL_Surface *destination);

vec2 v2make(float x, float y);
vec2 v2add(vec2 v1, vec2 v2);
vec2 v2sub(vec2 v1, vec2 v2);
vec2 v2mul(vec2 v1, vec2 v2);
vec2 v2div(vec2 v1, vec2 v2);
vec2 v2scale(vec2 vec, float scalar);
float v2mag(vec2 vec);
vec2 v2norm(vec2 vec);
vec2 v2perp(vec2 vec);
vec2 v2neg(vec2 vec);
point pointmake(int x, int y);
point point_from_vec2(vec2 vec);

float clampf(float val, float min, float max);

//rectangle functions
rect rect_make(int x, int y, int width, int height);
rect rect_copy(rect other);
int rect_left(rect r);
int rect_right(rect r);
int rect_top(rect r);
int rect_bottom(rect r);

//circle functions
circle circle_make(int x, int y, int radius);
circle circle_copy(circle other);

//collision functions
bool intersect_rr(rect r1, rect r2);
bool intersect_rc(rect r, circle c);
bool intersect_cr(circle c, rect r);
bool intersect_cc(circle c1, circle c2);

void stack_init(stack *self, int size, int elemSize, clearmem clearFunc);
void stack_push(stack *self, void *elem);
void stack_pop(stack *self, void *destination);
void stack_peek(stack *self, void *destination);
void stack_free(stack *self);
uint32_t stack_size(stack *self);

#endif
