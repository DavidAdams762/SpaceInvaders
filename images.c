#include <string.h>
#include <SDL/SDL.h>
#include "globals.h"
#include "util.h"
#include "images.h"

void init_image_assets()
{
	int i;
	imageAssetIndex = 0;
}

void free_image_assets()
{
	int i;
	for (i = 0; i < imageAssetIndex; i++)
	{
		SDL_FreeSurface(image_assets[i].image);
	}
}

bool push_image_asset(char *name, SDL_Surface *img)
{
	image_asset_t newAsset;

	if (imageAssetIndex >= MAX_IMAGES - 1)
		return false;

	newAsset.name = name;
	newAsset.image = img;

	image_assets[imageAssetIndex++] = newAsset;
	return true;
}

SDL_Surface *get_image(char *name)
{
	int i;
	for (i = 0; i < MAX_IMAGES; i++)
	{
		if (strcmp(name, image_assets[i].name) == 0)
		{
			return image_assets[i].image;
		}
	}

	return NULL;
}
