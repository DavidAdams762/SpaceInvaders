#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "space.h"

void reset_bullets(struct bullet_t b[], int max)
{
  for (int i = 0; i < max; i++)
    {
      b[i].alive = 0;
      b[i].hitbox.x = 0;
      b[i].hitbox.y = 0;
      b[i].hitbox.w = B_WIDTH;
      b[i].hitbox.h = B_HEIGHT;
    }
}

void display_bullets(struct bullet_t b[], int max, SDL_Surface *screen)
{
  for (int i = 0; i < max; i++)
    {
      if (b[i].alive == 1)
	{
	  SDL_FillRect(screen, &b[i].hitbox, SDL_MapRGB(screen->format, 255, 255, 255));
	}
    }
}

int move_bullets(struct bullet_t b[], int max, int speed)
{
  for (int i = 0; i < max; i++)
    {
      if (b[i].alive == 1)
	{
	  b[i].hitbox.y += speed;
	  if (b[i].hitbox.y <= 0)
	    {
	      b[i].alive = 0;
	    }
	  if (b[i].hitbox.y + b[i].hitbox.h >= HEIGHT) {
	    b[i].alive = 0;
	  }
	}
    }
  return 0;
}

void player_shoot(struct bullet_t *bullets, struct score_t score, struct player_t player)
{
  int i;
  for (i = 0; i < P_BULLETS; i++)
    {
      if (bullets[i].alive == 0)
	{
	  score.shots++;
	  bullets[i].hitbox.x = player.hitbox.x + (P_WIDTH / 2);
	  bullets[i].hitbox.y = player.hitbox.y - (bullets[i].hitbox.h + 10);
	  bullets[i].alive = 1;
	  break;
	}
    }
}
