/*
** affichage.c for q in /Users/Ophelie/Sites/SpaceInvaders/space-invaders-master
** 
** Made by Ophelie Volga
** Login   <volga_o@etna-alternance.net>
** 
** Started on  Thu Oct 17 21:47:55 2013 Ophelie Volga
** Last update Thu Oct 17 21:48:10 2013 Ophelie Volga
*/

#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "space.h"

void display_background (SDL_Surface *screen)
{
  SDL_Rect src;
  src.x = 0;
  src.y = 0;
  src.w = screen->w;
  src.h = screen->h;
  SDL_FillRect(screen, &src, SDL_MapRGB(screen->format, 0, 0, 0));
}

int display_char(char c, int x, int y, SDL_Surface *font_img, SDL_Surface *screen)
{
  SDL_Rect src;
  SDL_Rect dest;
  int i,j;
  char *map[] = {"ABCDEFGHIJKLMNOPQRSTUVWXYZ", "abcdefghijklmnopqrstuvwxyz", "0123456789"};
  src.x = 0;
  src.y = 0;
  src.w = 17;
  src.h = 20;
  dest.x = x;
  dest.y = y;
  dest.w = 17;
  dest.h = 20;
  for (i = 0; i < 3; i++)
    {
      for(j = 0; j < strlen(map[i]); j++)
	{
	  if (c == map[i][j])
	    {
	      SDL_BlitSurface(font_img, &src, screen, &dest);
	      return 0;
	    }
	  src.x += 17;
	}
      src.y += 20;
      src.x = 0;
    }
  return 0;
}

void display_string(char s[], int x, int y, SDL_Surface *font_img, SDL_Surface *screen)
{
  int i;
  for (i = 0; i < strlen(s); i++)
    {
      display_char(s[i], x, y, font_img, screen);
      x += 17;
    }
}

void display_status(SDL_Surface *screen, struct score_t score, struct player_t player, SDL_Surface *font_img)
{
  SDL_Rect r;
  r.x = 0;
  r.y = 0;
  r.w = 800;
  r.h = 20;
  SDL_FillRect(screen, &r, SDL_MapRGB(screen->format, 41, 41, 41));

  char score_label[] = "Score";
  display_string(score_label, 50, 0, font_img, screen);

  char score_num[10];
  snprintf(score_num, 10, "%d", score.points);
  display_string(score_num, 150, 0, font_img, screen);

  char level[] = "Level";
  display_string(level, 300, 0, font_img, screen);

  char level_num[2];
  snprintf(level_num, 2, "%d", score.level);
  display_string(level_num, 400, 0, font_img, screen);

  char lives[] = "Lives";
  display_string(lives, 550, 0, font_img, screen);

  char lives_num[2];
  snprintf(lives_num, 2, "%d", player.lives);
  display_string(lives_num, 650, 0, font_img, screen);
}

void display_title_screen(SDL_Surface *title_screen, SDL_Surface *screen)
{
  SDL_Rect src;
  SDL_Rect dest;

  src.x = 0;
  src.y = 0;
  src.w = title_screen->w;
  src.h = title_screen->h;

  dest.x = (WIDTH / 2) - (title_screen->w / 2);
  dest.y = (HEIGHT / 3) - (title_screen->h / 2);
  dest.w = title_screen->w;
  dest.h = title_screen->h;

  SDL_BlitSurface(title_screen, &src, screen, &dest);
}

void display_game_over(SDL_Surface *game_over_img, SDL_Surface *screen)
{
  SDL_Rect src;
  SDL_Rect dest;

  src.x = 0;
  src.y = 0;
  src.w = game_over_img->w;
  src.h = game_over_img->h;

  dest.x = (WIDTH / 2) - (game_over_img->w / 2);
  dest.y = (HEIGHT / 2) - (game_over_img->h / 2);
  dest.w = game_over_img->w;
  dest.h = game_over_img->h;

	SDL_BlitSurface(game_over_img, &src, screen, &dest);
}

char *display_scores(struct score_t score, SDL_Surface *screen, SDL_Surface *font_img)
{
     FILE *f;
     char *result = malloc(255 * sizeof(char));
     char *tmp = malloc(255 * sizeof(char));
     char *nfile;
     nfile = "scores.txt";


     if (file_exists(nfile) == 0)
         create_file_scores(nfile);

     tmp = get_scores(nfile, score.points);

     change_scores(nfile, tmp);
     result = get_format_scores(nfile);

     return result;
}
