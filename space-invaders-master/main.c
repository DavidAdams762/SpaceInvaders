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
    SDL_Surface *invadersmap;
    SDL_Surface *player_img;
    SDL_Surface *saucer_img;
    SDL_Surface *damage_img;
    SDL_Surface *damage_top_img;
    SDL_Surface *game_over_img;
    struct score_t score;
    struct invaders_t invaders;
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
	
	SDL_WM_SetCaption("Space Invaders ETNA", "P");
	
	screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
	
	if (screen == NULL) {
		printf("Unable to set video mode: %s\n", SDL_GetError());
		return 1;
	}

    title_screen = IMG_Load("images/title_screen.png");
    cmap = IMG_Load("images/cmap.png");
    invadersmap = IMG_Load("images/invaders.png");
    player_img = IMG_Load("images/player_ship.png");
    game_over_img = IMG_Load("images/gameover.png");
	Uint32 next_game_tick = SDL_GetTicks();
	int sleep = 0;
	Uint8 *keystate = 0;
	int quit = 0;
	SDL_Event event;

	score = init_score(score);
	invaders = init_invaders(invaders);
	player = init_player(player);
	init_bullets(bullets, P_BULLETS);
	init_bullets(e_bullets, E_BULLETS);
	state = menu;
	title_time = SDL_GetTicks();
		
	/* Animate */
	while (quit == 0) {
		
		/* Grab a snapshot of the keyboard. */
		keystate = SDL_GetKeyState(NULL);

		while (SDL_PollEvent(&event)) {

			switch(event.type) {

				case SDL_KEYDOWN:

					switch( event.key.keysym.sym ) {

						//exit out of game loop if escape is pressed
						case SDLK_ESCAPE:

							quit = 1;
						break;

						case SDLK_SPACE:

							if (state == menu) {

								state = game;

							} else if (state == game){

								player_shoot(bullets, score, player);

							} else if (state == game_over) {

								invaders = init_invaders(invaders);
								score = init_score(score);
								player = init_player(player);
								state = game;
							}
						break;

						default:
						break;
					}
				break;
			}
		}
	
		draw_background(screen);

		if (state == menu) {
			char s[] = "Press SPACEBAR to start";

			draw_title_screen(title_screen, screen);
			draw_string(s, (SCREEN_WIDTH / 4), 400, cmap, screen);

		} else if (state == game) {
			if (keystate[SDLK_LEFT]) {

				player = move_player(left, player);
			}

			if (keystate[SDLK_RIGHT]) {
				player = move_player(right, player);
			}
			draw_hud(screen, score, player, cmap);
			draw_player(player_img, screen, player);
			draw_invaders(invaders, invadersmap, screen);
			draw_bullets(bullets, P_BULLETS, screen);
			draw_bullets(e_bullets, E_BULLETS, screen);
			invaders = ennemy_hit_collision(invaders, bullets, score);
			score = update_score(invaders, score);
			player = player_hit_collision(e_bullets, player, state, pause_time, pause_len);
			ennemy_player_collision(invaders, player, state, pause_time, pause_len);
			invaders = move_invaders(invaders.speed, invaders);
			move_bullets(bullets, P_BULLETS, -30);
			move_bullets(e_bullets, E_BULLETS, 15);
			score = calculate_level(invaders, score, state, pause_time, pause_len);
			if (score.passed > 0) {
                invaders = init_invaders(invaders);
                invaders.score = score.points;
                score.passed = 0;
			}
			ennemy_ai(invaders, player, e_bullets);
			state = game_over_ai(player, state);
			pause_game(pause_time, pause_len, state);
		
		} else if (state == game_over) {
			
			draw_hud(screen, score, player, cmap);
            draw_player(player_img, screen, player);
            draw_invaders(invaders, invadersmap, screen);
            draw_bullets(bullets, P_BULLETS, screen);
            draw_bullets(e_bullets, E_BULLETS, screen);
			draw_game_over(game_over_img, screen);

		} else if (state == pause) {
			
			draw_hud(screen, score, player, cmap);
            draw_player(player_img, screen, player);
            draw_invaders(invaders, invadersmap, screen);
            draw_bullets(bullets, P_BULLETS, screen);
            draw_bullets(e_bullets, E_BULLETS, screen);
			pause_game(pause_time, pause_len, state);
		}

		/* Ask SDL to update the entire screen. */
		SDL_Flip(screen);

		next_game_tick += 1000 / 30;
		sleep = next_game_tick - SDL_GetTicks();

		if( sleep >= 0 ) {
            SDL_Delay(sleep);
        }
	}

	return 0;
}
