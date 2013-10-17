#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "space.h"

void pause_game(Uint32 pause_time, int pause_len, enum state_t state)
{
  if (SDL_GetTicks() > pause_time + pause_len)
    {
      state = game;
    }
}

int main()
{
  SDL_Surface *screen;
  SDL_Surface *title_screen;
  SDL_Surface *font_img;
  SDL_Surface *ennemies_img;
  SDL_Surface *player_img;
  SDL_Surface *game_over_img;
  struct score_t score;
  struct ennemies_t ennemies;
  struct player_t player;
  struct bullets_t bullets[PLAYER_B];
  struct bullets_t ennemy_bullets[ENNEMY_B];
  int pause_len;
  Uint32 pause_time;
  enum state_t state;
  Uint32 title_time;
  int i;
  char *result;
  result = malloc(sizeof(char) * 255);
  i = 0;

  if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
      printf("Unable to initialize SDL: %s\n", SDL_GetError());
      return 1;
    }

  atexit(SDL_Quit);

  SDL_WM_SetCaption("Space ennemies ETNA", "P");

  screen = SDL_SetVideoMode(WIDTH, HEIGHT, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);

  if (screen == NULL)
    {
      printf("Unable to set video mode: %s\n", SDL_GetError());
      return 1;
    }

  title_screen = IMG_Load("images/title_screen.png");
  font_img = IMG_Load("images/font.png");
  ennemies_img = IMG_Load("images/ennemies.png");
  player_img = IMG_Load("images/player_ship.png");
  game_over_img = IMG_Load("images/gameover.png");
  Uint32 next_game_tick = SDL_GetTicks();
  int sleep = 0;
  Uint8 *keystate = 0;
  int quit = 0;
  SDL_Event event;

  score = reset_score(score);
  ennemies = reset_ennemies(ennemies);
  player = reset_player(player);
  reset_bullets(bullets, PLAYER_B);
  reset_bullets(ennemy_bullets, ENNEMY_B);
  state = menu;
  title_time = SDL_GetTicks();

  while (quit == 0)
    {
      keystate = SDL_GetKeyState(NULL);
      while (SDL_PollEvent(&event))
	{
	  if (event.type == SDL_KEYDOWN)
	    {
	      if (event.key.keysym.sym == SDLK_ESCAPE)
		    quit = 1;
	      else if ( event.key.keysym.sym == SDLK_SPACE)
            {
              if (state == menu)
                state = game;
              else if (state == game)
                player_shoot(bullets, score, player);
              else if (state == game_over)
                {
                  ennemies = reset_ennemies(ennemies);
                  score = reset_score(score);
                  player = reset_player(player);
                  state = game;
                }
            }
            else if ( event.key.keysym.sym == SDLK_s)
            {
                if (((state != game) && (state != pause)) || state == game_over)
                    state = options;
            }
            else if ( event.key.keysym.sym == SDLK_p)
            {
                if (state == game)
                    state = pause;
                 else if (state == pause)
                     state = game;
            }

	    }
	     break;
	}
      display_background(screen);
      if (state == menu)
	{
	  char space[] = "Press SPACEBAR to start";
	  display_title_screen(title_screen, screen);
	  display_string(space, (WIDTH / 4), 400, font_img, screen);
	  char s[] = "Press key s to see best scores";
      display_string(s, (WIDTH / 4), 430, font_img, screen);
	}
	else if (state == options) {
        if (i == 0)
            result = display_scores(score, screen, font_img);
        i++;
        display_string(result, (WIDTH / 4), 0, font_img, screen);
    }
      else if (state == game)
	{
	  if (keystate[SDLK_LEFT])
	    player = move_player(left, player);
	  if (keystate[SDLK_RIGHT])
	    player = move_player(right, player);
	  display_status(screen, score, player, font_img);
	  display_player(player_img, screen, player);
	  display_ennemies(ennemies, ennemies_img, screen);
	  display_bullets(bullets, PLAYER_B, screen);
	  display_bullets(ennemy_bullets, ENNEMY_B, screen);
	  ennemies = ennemy_hit_collision(ennemies, bullets, score);
	  score = update_score(ennemies, score);
	  player = player_hit_collision(ennemy_bullets, player, state, pause_time, pause_len);
	  ennemy_player_collision(ennemies, player, state, pause_time, pause_len);
	  ennemies = move_ennemies(ennemies.speed, ennemies);
	  move_bullets(bullets, PLAYER_B, -30);
	  move_bullets(ennemy_bullets, ENNEMY_B, 15);
	  score = check_level(ennemies, score, state, pause_time, pause_len);
	  if (score.passed > 0)
	    {
	      ennemies = reset_ennemies(ennemies);
	      ennemies.score = score.points;
	      score.passed = 0;
	    }
	  ennemy_moving(ennemies, player, ennemy_bullets);
	  state = state_game_over(player, state);
	  pause_game(pause_time, pause_len, state);
	}
      else if (state == game_over)
	{
	  i = 0;
	  display_status(screen, score, player, font_img);
	  display_player(player_img, screen, player);
	  display_ennemies(ennemies, ennemies_img, screen);
	  display_bullets(bullets, PLAYER_B, screen);
	  display_bullets(ennemy_bullets, ENNEMY_B, screen);
	  display_game_over(game_over_img, screen);
	  if (i == 0)
          result = display_scores(score, screen, font_img);
      i++;
      display_string(result, (WIDTH / 4), 400, font_img, screen);
	}
      else if (state == pause)
	{
	  display_string("PAUSE", (WIDTH) / 2, (HEIGHT) / 2, font_img, screen);
	  display_status(screen, score, player, font_img);
	  display_player(player_img, screen, player);
	  display_ennemies(ennemies, ennemies_img, screen);
	  display_bullets(bullets, PLAYER_B, screen);
	  display_bullets(ennemy_bullets, ENNEMY_B, screen);
	  pause_game(pause_time, pause_len, state);
	}

      SDL_Flip(screen);

      next_game_tick += 1000 / 30;
      sleep = next_game_tick - SDL_GetTicks();

      if(sleep >= 0)
	SDL_Delay(sleep);
    }
  return 0;
}
