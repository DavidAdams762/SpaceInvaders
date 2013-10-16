#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "space.h"

struct ennemies_t reset_ennemies(struct ennemies_t ennemies)
{
  ennemies.direction = right;
  ennemies.speed = 1;
  ennemies.state = 0;
  ennemies.killed = 0;
  ennemies.score = 0;
  ennemies.state_speed = 1000;
  ennemies.state_time = SDL_GetTicks();

  int x = 0;
  int y = 30;

  for (int i = 0; i < 5; i++)
    {
    for (int j = 0; j < 8; j++)
      {
	ennemies.ennemy[i][j].alive = 1;
	ennemies.ennemy[i][j].hitbox.x = x;
	ennemies.ennemy[i][j].hitbox.y = y;
	ennemies.ennemy[i][j].hitbox.w = E_WIDTH;
	ennemies.ennemy[i][j].hitbox.h = E_HEIGHT;

	x += E_WIDTH + 20; // gap size

	if (i == 0) {
	  ennemies.ennemy[i][j].colour = purple;
	  ennemies.ennemy[i][j].points = 30;
	}
	else if (i >= 1 && i < 3) {
	  ennemies.ennemy[i][j].colour = green;
	  ennemies.ennemy[i][j].points = 20;
	}
	else {
	  ennemies.ennemy[i][j].colour = red;
	  ennemies.ennemy[i][j].points = 10;
	}
      }
    x = 0;
    y += E_HEIGHT + 20;
    }
  return ennemies;
}

void display_ennemies(struct ennemies_t ennemies, SDL_Surface *ennemies_img, SDL_Surface *screen)
{
  SDL_Rect src, dest;

  src.w = E_WIDTH;
  src.h = E_HEIGHT;

  for (int i = 0; i < 5; i++)
    {
      for (int j = 0; j < 8; j++)
	{
	  if (ennemies.ennemy[i][j].alive == 1)
	    {
	      if(i == 0)
		{
		  if (ennemies.state == 0)
		    {
		      src.x = 0;
		      src.y = 0;
		    }
		  else
		    {
		      src.x = 30;
		      src.y = 0;
		    }
		}
	      else if (i > 0 && i < 3)
		{
		  if (ennemies.state == 0)
		    {
		      src.x = 0;
		      src.y = E_HEIGHT;
		    }
		  else
		    {
		      src.x = 30;
		      src.y = E_HEIGHT;
		    }
		}
	      else
		{
		  if (ennemies.state == 0)
		    {
		      src.x = 0;
		      src.y = E_HEIGHT * 2;
		    }
		  else
		    {
		      src.x = 30;
		      src.y = E_HEIGHT * 2;
		    }
		}
	      dest.x = ennemies.ennemy[i][j].hitbox.x;
	      dest.y = ennemies.ennemy[i][j].hitbox.y;
	      dest.w = ennemies.ennemy[i][j].hitbox.w;
	      dest.h = ennemies.ennemy[i][j].hitbox.h;

	      SDL_BlitSurface(ennemies_img, &src, screen, &dest);
	    }
	}
    }
}

struct ennemies_t set_ennemies_speed(struct ennemies_t ennemies) {
  if (ennemies.killed == 10) {
    ennemies.speed = 2;
    ennemies.state_speed = 800;
  }
  else if (ennemies.killed == 20)
    {
      ennemies.speed = 4;
      ennemies.state_speed = 600;
    }
  else if (ennemies.killed == 30)
    {
      ennemies.speed = 6;
      ennemies.state_speed = 400;
    }
  return ennemies;
}

struct ennemies_t move_ennemies_down(struct ennemies_t ennemies) {
  for (int i = 0; i < 5; i++)
    {
      for (int j = 0; j < 10; j++)
	{
	  ennemies.ennemy[i][j].hitbox.y += 15;
	}
    }
  return ennemies;
}

struct ennemies_t move_ennemies(int speed, struct ennemies_t ennemies) {
  ennemies = set_ennemies_speed(ennemies);
  if (ennemies.direction == left)
    {
      for (int i = 0; i < 10; i++)
	{
	  for (int j = 0; j < 5; j++)
	    {
	      if (ennemies.ennemy[j][i].alive == 1)
		{
		  if (ennemies.ennemy[j][i].hitbox.x <= 0)
		    {
		      ennemies.direction = right;
		      ennemies = move_ennemies_down(ennemies);
		      return ennemies;
		    }
		  if (ennemies.state_time + ennemies.state_speed < SDL_GetTicks())
		    {
		      ennemies.state_time = SDL_GetTicks();
		      if (ennemies.state == 1)
			ennemies.state = 0;
		      else
			ennemies.state = 1;
		    }
		  ennemies.ennemy[j][i].hitbox.x -= ennemies.speed;
		}
	    }
	}
    }
  else if (ennemies.direction == right)
    {
      for (int i = 9; i >= 0; i--)
	{
	  for (int j = 0; j < 5; j++)
	    {
	      if (ennemies.ennemy[j][i].alive == 1)
		{
		  if (ennemies.ennemy[j][i].hitbox.x + E_WIDTH >= WIDTH)
		    {
		      ennemies.direction = left;
		      ennemies = move_ennemies_down(ennemies);
		      return ennemies;
		    }
		  if (ennemies.state_time + ennemies.state_speed < SDL_GetTicks())
		    {
		      ennemies.state_time = SDL_GetTicks();
		      if (ennemies.state == 1)
			ennemies.state = 0;
		      else
			ennemies.state = 1;
		    }
		  ennemies.ennemy[j][i].hitbox.x += ennemies.speed;
		}
	    }
	}
    }
  return ennemies;
}

void ennemy_ai(struct ennemies_t ennemies, struct player_t player, struct bullet_t *e_bullets) {
  int i, j, k;
  for (i = 0; i < 10; i++)
    {
      for (j = 4; j >= 0; j--)
	{
	  if (ennemies.ennemy[j][i].alive == 1)
	    {
	      int mid_point = player.hitbox.x + (player.hitbox.w / 2);
	      int start = ennemies.ennemy[j][i].hitbox.x;
	      int end = ennemies.ennemy[j][i].hitbox.x + ennemies.ennemy[j][i].hitbox.w;

	      if (mid_point > start && mid_point < end)
		{
		  for (k = 0; k < E_BULLETS; k++)
		    {
		      if (e_bullets[k].alive == 0)
			{
			  int r = rand() % 30;
			  if (r == 1)
			    {
			      e_bullets[k].hitbox.x = start + (E_WIDTH / 2);
			      e_bullets[k].hitbox.y = ennemies.ennemy[j][i].hitbox.y;
			      e_bullets[k].alive = 1;
			    }
			  break;
			}
		    }
		}
	      break;
	    }
	}
    }
}
