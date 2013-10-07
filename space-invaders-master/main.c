#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "space.h"

void pause_game(Uint32 pause_time, unsigned int pause_len, enum state_t state) {
	if (SDL_GetTicks() > pause_time + pause_len) {
		state = game;
	}
}

int main() {
    SDL_Surface *screen;
    SDL_Surface *title_screen;
    SDL_Surface *cmap;
    SDL_Surface *ennemies_img;
    SDL_Surface *player_img;
    SDL_Surface *saucer_img;
    SDL_Surface *damage_img;
    SDL_Surface *damage_top_img;
    SDL_Surface *game_over_img;
    struct score_t score;
    struct ennemies_t ennemies;
    struct player_t player;
    struct bullet_t bullets[P_BULLETS];
    struct bullet_t e_bullets[E_BULLETS];
    unsigned int pause_len;
    Uint32 pause_time;
    enum state_t state;
    Uint32 title_time;
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		printf("Unable to initialize SDL: %s\n", SDL_GetError());
		return 1;
	}
	
	atexit(SDL_Quit);
	
	SDL_WM_SetCaption("Space ennemies ETNA", "P");
	
	screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
	
	if (screen == NULL) {
		printf("Unable to set video mode: %s\n", SDL_GetError());
		return 1;
	}

    title_screen = IMG_Load("images/title_screen.png");
    cmap = IMG_Load("images/cmap.png");
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
	reset_bullets(bullets, P_BULLETS);
	reset_bullets(e_bullets, E_BULLETS);
	state = menu;
	title_time = SDL_GetTicks();
		
	while (quit == 0) {
		keystate = SDL_GetKeyState(NULL);
		while (SDL_PollEvent(&event)) {
			switch(event.type) {
				case SDL_KEYDOWN:
					switch( event.key.keysym.sym ) {
						case SDLK_ESCAPE:
							quit = 1;
						break;
						case SDLK_SPACE:
							if (state == menu) {
								state = game;
							} else if (state == game) {
								player_shoot(bullets, score, player);
							} else if (state == game_over) {
								ennemies = reset_ennemies(ennemies);
								score = reset_score(score);
								player = reset_player(player);
								state = game;
							}
						break;
						default:
						break;
					}
				break;
			}
		}
		display_background(screen);
		if (state == menu) {
			char s[] = "Press SPACEBAR to start";
			display_title_screen(title_screen, screen);
			display_string(s, (SCREEN_WIDTH / 4), 400, cmap, screen);

		} else if (state == game) {
			if (keystate[SDLK_LEFT]) {
				player = move_player(left, player);
			}
			if (keystate[SDLK_RIGHT]) {
				player = move_player(right, player);
			}
			display_hud(screen, score, player, cmap);
			display_player(player_img, screen, player);
			display_ennemies(ennemies, ennemies_img, screen);
			display_bullets(bullets, P_BULLETS, screen);
			display_bullets(e_bullets, E_BULLETS, screen);
			ennemies = ennemy_hit_collision(ennemies, bullets, score);
			score = update_score(ennemies, score);
			player = player_hit_collision(e_bullets, player, state, pause_time, pause_len);
			ennemy_player_collision(ennemies, player, state, pause_time, pause_len);
			ennemies = move_ennemies(ennemies.speed, ennemies);
			move_bullets(bullets, P_BULLETS, -30);
			move_bullets(e_bullets, E_BULLETS, 15);
			score = calculate_level(ennemies, score, state, pause_time, pause_len);
			if (score.passed > 0) {
                ennemies = reset_ennemies(ennemies);
                ennemies.score = score.points;
                score.passed = 0;
			}
			ennemy_ai(ennemies, player, e_bullets);
			state = game_over_ai(player, state);
			pause_game(pause_time, pause_len, state);
		
		} else if (state == game_over) {
			display_hud(screen, score, player, cmap);
            display_player(player_img, screen, player);
            display_ennemies(ennemies, ennemies_img, screen);
            display_bullets(bullets, P_BULLETS, screen);
            display_bullets(e_bullets, E_BULLETS, screen);
			display_game_over(game_over_img, screen);
		} else if (state == pause) {
			display_hud(screen, score, player, cmap);
            display_player(player_img, screen, player);
            display_ennemies(ennemies, ennemies_img, screen);
            display_bullets(bullets, P_BULLETS, screen);
            display_bullets(e_bullets, E_BULLETS, screen);
			pause_game(pause_time, pause_len, state);
		}

		SDL_Flip(screen);

		next_game_tick += 1000 / 30;
		sleep = next_game_tick - SDL_GetTicks();

		if(sleep >= 0) {
            SDL_Delay(sleep);
        }
	}
	return 0;
}
