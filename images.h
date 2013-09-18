#ifndef _VADERS_IMAGES_H_
#define _VADERS_IMAGES_H_

#include <SDL/SDL.h>
#include "globals.h"
#include "util.h"

#define MAX_IMAGES 100

typedef struct 
{
	SDL_Surface *image;
	char *name;
} image_asset_t;

image_asset_t image_assets[MAX_IMAGES];
int imageAssetIndex;

void init_image_assets();
void free_image_assets();
bool push_image_asset(char *name, SDL_Surface *img);
SDL_Surface *get_image(char *name);

#endif