#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "space.h"

struct player_t reset_player(struct player_t player)
{
  player.hitbox.x = (WIDTH / 2) - (P_WIDTH / 2);
  player.hitbox.y = HEIGHT - (P_HEIGHT + 10);
  player.hitbox.w = P_WIDTH;
  player.hitbox.h = P_HEIGHT;
  player.lives = 3;

  return player;
}

void display_player(SDL_Surface *player_img, SDL_Surface *screen, struct player_t player)
{
  SDL_Rect src;

  src.x = 0;
  src.y = 0;
  src.w = P_WIDTH;
  src.h = P_HEIGHT;

  SDL_BlitSurface(player_img, &src, screen, &player.hitbox);
}

struct player_t move_player(enum direction_t direction, struct player_t player)
{
  if (direction == left)
    {
      if (player.hitbox.x > 0)
	{
	  player.hitbox.x -= 10;
	}
    }
  else if (direction == right)
    {
      if (player.hitbox.x + player.hitbox.w < WIDTH)
	{
	  player.hitbox.x += 10;
	}
    }
  return player;
}
