#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "space.h"

int collision(SDL_Rect a, SDL_Rect b)
{
  if (a.y + a.h < b.y) {
    return 0;
  }
  if (a.y > b.y + b.h) {
    return 0;
  }
  if (a.x > b.x + b.w) {
    return 0;
  }
  if (a.x + a.w < b.x) {
    return 0;
  }
  return 1;
}

struct ennemies_t ennemy_hit_collision(struct ennemies_t ennemies, struct bullets_t *bullets, struct score_t score)
{
  int c;
  for (int i = 0; i < 5; i++)
    {
      for (int j = 0; j < 10; j++)
	{
	  if (ennemies.ennemy[i][j].alive == 1)
	    {
	      for (int k = 0; k < PLAYER_B; k++)
		{
		  if (bullets[k].alive == 1)
		    {
		      c = collision(bullets[k].hitbox, ennemies.ennemy[i][j].hitbox);
		      if (c == 1)
			{
			  ennemies.ennemy[i][j].alive = 0;
			  bullets[k].alive = 0;
			  bullets[k].hitbox.x = 0;
			  bullets[k].hitbox.y = 0;
			  ennemies.killed++;
			  ennemies.score += ennemies.ennemy[i][j].points;
			}
		    }
		}
	    }
	}
    }
  return ennemies;
}

struct player_t player_hit_collision(struct bullets_t *ennemy_bullets, struct player_t player, enum state_t state, Uint32 pause_time, int pause_len)
{
  int i,c;

  for(i = 0; i < ENNEMY_B; i++)
    {
      if (ennemy_bullets[i].alive == 1)
	{
	  c = collision(ennemy_bullets[i].hitbox, player.hitbox);
	  if (c == 1)
	    {
	      if (player.lives >= 0)
		{
		  player.lives--;
		}
	    }
	}
    }
  return player;
}

struct ennemies_t ennemy_player_collision(struct ennemies_t ennemies, struct player_t player, enum state_t state, Uint32 pause_time, int pause_len)
{
  int i,j,c;

  for(i = 0; i < 5; i++)
    {
      for(j = 0; j < 10; j++)
	{
	  if (ennemies.ennemy[i][j].alive == 1)
	    {
	      c = collision(player.hitbox, ennemies.ennemy[i][j].hitbox);
	      if (c == 1)
		{
		  player.lives--;
		  ennemies = reset_ennemies(ennemies);
		  return ennemies;
		}
	    }
	}
    }
  return ennemies;
}
