#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include "space.h"

int load_image(char filename[], SDL_Surface **surface, enum ck_t colour_key);

struct score_t init_score(struct score_t score) {
	score.shots = 0;
	score.points = 0;
	score.level = 1;
	return score;
}

struct score_t update_score(struct invaders_t invaders, struct score_t score) {
    score.points = invaders.score;
    return score;
}

struct player_t init_player(struct player_t player) {
	player.hitbox.x = (WIDTH / 2) - (P_WIDTH / 2);
	player.hitbox.y = HEIGHT - (P_HEIGHT + 10);
	player.hitbox.w = P_WIDTH;
	player.hitbox.h = P_HEIGHT;
	player.lives = 3;

	return player;
}

void init_bullets(struct bullet_t b[], int max) {
	for (int i = 0; i < max; i++) {
		b[i].alive = 0;
		b[i].hitbox.x = 0;
		b[i].hitbox.y = 0;
		b[i].hitbox.w = B_WIDTH;
		b[i].hitbox.h = B_HEIGHT;
	}
}

void draw_background (SDL_Surface *screen) {
	SDL_Rect src;
	src.x = 0;
	src.y = 0;
	src.w = screen->w;
	src.h = screen->h;
	SDL_FillRect(screen, &src, SDL_MapRGB(screen->format, 0, 0, 0));
}

int draw_char(char c, int x, int y, SDL_Surface *cmap, SDL_Surface *screen) {
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
	for (i = 0; i < 3; i++) {
		for(j = 0; j < strlen(map[i]); j++) {
			if (c == map[i][j]) {
				SDL_BlitSurface(cmap, &src, screen, &dest);
				return 0;
			}
			src.x += 17;
		}
		src.y += 20;//move down one line on the image file
		src.x = 0; //reset to start of line
	}
	return 0;
}

void draw_string(char s[], int x, int y, SDL_Surface *cmap, SDL_Surface *screen) {
	int i;
	for (i = 0; i < strlen(s); i++) {
		draw_char(s[i], x, y, cmap, screen);
		x += 17;
	}
}

enum state_t pause_for(unsigned int len, enum state_t state, Uint32 pause_time, unsigned int pause_len) {
	state = pause;
	pause_time = SDL_GetTicks();
	pause_len = len;
	return state;
}

void draw_hud(SDL_Surface *screen, struct score_t score, struct player_t player, SDL_Surface *cmap) {
	SDL_Rect r;
	r.x = WIDTH;
	r.y = 0;
	r.w = SCREEN_WIDTH - WIDTH;
	r.h = SCREEN_HEIGHT;
	SDL_FillRect(screen, &r, SDL_MapRGB(screen->format, 41, 41, 41));

	char score_label[] = "Score";
	draw_string(score_label, WIDTH, 0, cmap, screen);
	
	char score_num[10];
	snprintf(score_num, 10, "%d", score.points);
	draw_string(score_num, WIDTH, 20, cmap, screen);

	char level[] = "Level";
	draw_string(level, WIDTH, 60, cmap, screen);
	
	char level_num[2];
	snprintf(level_num, 2, "%d", score.level);
	draw_string(level_num, WIDTH, 80, cmap, screen);
	
	char lives[] = "Lives";
	draw_string(lives, WIDTH, 120, cmap, screen);
	
	char lives_num[2];
	snprintf(lives_num, 2, "%d", player.lives);
	draw_string(lives_num, WIDTH, 140, cmap, screen);

	char credits[] = "Made by";
    draw_string(credits, WIDTH, 480, cmap, screen);

	char name1[] = "David";
	draw_string(name1, WIDTH, 500, cmap, screen);

	char name2[] = "Eddy";
    draw_string(name2, WIDTH, 520, cmap, screen);

	char name3[] = "Kenny";
	draw_string(name3, WIDTH, 540, cmap, screen);

	char name4[] = "Ophelie";
    draw_string(name4, WIDTH, 560, cmap, screen);
}

void draw_title_screen(SDL_Surface *title_screen, SDL_Surface *screen) {
	
	SDL_Rect src;
	SDL_Rect dest;

	src.x = 0;
	src.y = 0;
	src.w = title_screen->w;
	src.h = title_screen->h;

	dest.x = (SCREEN_WIDTH / 2) - (title_screen->w / 2);
	dest.y = (HEIGHT / 3) - (title_screen->h / 2);
	dest.w = title_screen->w;
	dest.h = title_screen->h;
	
	SDL_BlitSurface(title_screen, &src, screen, &dest);
}

void draw_player(SDL_Surface *player_img, SDL_Surface *screen, struct player_t player) {

	SDL_Rect src;

	src.x = 0;
	src.y = 0;
	src.w = P_WIDTH;
	src.h = P_HEIGHT;

	SDL_BlitSurface(player_img, &src, screen, &player.hitbox);
}

void draw_bullets(struct bullet_t b[], int max, SDL_Surface *screen) {
	for (int i = 0; i < max; i++) {
		if (b[i].alive == 1) {
			SDL_FillRect(screen, &b[i].hitbox, SDL_MapRGB(screen->format, 255, 255, 255));
		}
	}
}

void draw_game_over(SDL_Surface *game_over_img, SDL_Surface *screen) {
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

int move_bullets(struct bullet_t b[], int max, int speed) {
	for (int i = 0; i < max; i++) {
		if (b[i].alive == 1) {
			b[i].hitbox.y += speed;
			if (b[i].hitbox.y <= 0) {
				b[i].alive = 0;	
			}
			if (b[i].hitbox.y + b[i].hitbox.h >= HEIGHT) {
				b[i].alive = 0;	
			}
		}
	}
	return 0;
}

struct player_t move_player(enum direction_t direction, struct player_t player) {
	if (direction == left) {
		if (player.hitbox.x > 0) {
			player.hitbox.x -= 10;
		}
	}
	else if (direction == right) {
		if (player.hitbox.x + player.hitbox.w < WIDTH){
			player.hitbox.x += 10;
		}
	}
	return player;
}

int collision(SDL_Rect a, SDL_Rect b) {
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

struct invaders_t ennemy_hit_collision(struct invaders_t invaders, struct bullet_t *bullets, struct score_t score) {
	int c;
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 10; j++) {
			if (invaders.ennemy[i][j].alive == 1) {
				for (int k = 0; k < P_BULLETS; k++) {
					if (bullets[k].alive == 1) {
						c = collision(bullets[k].hitbox, invaders.ennemy[i][j].hitbox);
						if (c == 1) {
							invaders.ennemy[i][j].alive = 0;
							bullets[k].alive = 0;
							bullets[k].hitbox.x = 0;
							bullets[k].hitbox.y = 0;
							invaders.killed++;
							invaders.score += invaders.ennemy[i][j].points;
						}
					}
				}
			}
		}
	}
	return invaders;
}

struct invaders_t init_invaders(struct invaders_t invaders) {
	invaders.direction = right;
	invaders.speed = 1;
	invaders.state = 0;
	invaders.killed = 0;
	invaders.score = 0;
	invaders.state_speed = 1000;
	invaders.state_time = SDL_GetTicks();

	int x = 0;
	int y = 30;

	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 8; j++) {
			invaders.ennemy[i][j].alive = 1;
			invaders.ennemy[i][j].hitbox.x = x;
			invaders.ennemy[i][j].hitbox.y = y;
			invaders.ennemy[i][j].hitbox.w = E_WIDTH;
			invaders.ennemy[i][j].hitbox.h = E_HEIGHT;

			x += E_WIDTH + 20; // gap size

			if (i == 0) {
				invaders.ennemy[i][j].colour = purple;
				invaders.ennemy[i][j].points = 30;

			}
			else if (i >= 1 && i < 3) {
				invaders.ennemy[i][j].colour = green;
				invaders.ennemy[i][j].points = 20;

			}
			else {
				invaders.ennemy[i][j].colour = red;
				invaders.ennemy[i][j].points = 10;
			}
		}

		x = 0;
		y += E_HEIGHT + 20;
	}
	return invaders;
}

void draw_invaders(struct invaders_t invaders, SDL_Surface *invadersmap, SDL_Surface *screen) {
	SDL_Rect src, dest;

	src.w = E_WIDTH;
	src.h = E_HEIGHT;

	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 8; j++) {
			if (invaders.ennemy[i][j].alive == 1) {
				//purple
				if(i == 0) {
					if (invaders.state == 0) {
						src.x = 0;
						src.y = 0;
					} else {
						src.x = 30;
						src.y = 0;
					}

				//green
				}
				else if (i > 0 && i < 3) {
					if (invaders.state == 0) {
						src.x = 0;
						src.y = E_HEIGHT;
					}
					else {
						src.x = 30;
						src.y = E_HEIGHT;
					}

				//red
				}
				else {
					if (invaders.state == 0) {
						src.x = 0;
						src.y = E_HEIGHT * 2;
					}
					else {
						src.x = 30;
						src.y = E_HEIGHT * 2;
					}
				}
				dest.x = invaders.ennemy[i][j].hitbox.x;
				dest.y = invaders.ennemy[i][j].hitbox.y;
				dest.w = invaders.ennemy[i][j].hitbox.w;
				dest.h = invaders.ennemy[i][j].hitbox.h;

				SDL_BlitSurface(invadersmap, &src, screen, &dest);
			}
		}
	}
}

struct invaders_t set_invaders_speed(struct invaders_t invaders) {
	switch (invaders.killed) {
		case 10:
			invaders.speed = 2;
			invaders.state_speed = 800;
			break;
		case 20:
			invaders.speed = 4;
			invaders.state_speed = 600;
			break;
		case 30:
			invaders.speed = 6;
			invaders.state_speed = 400;
			break;
	}
	return invaders;
}

struct invaders_t move_invaders_down(struct invaders_t invaders) {
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 10; j++) {
			invaders.ennemy[i][j].hitbox.y += 15;
		}
	}
	return invaders;
}

struct invaders_t move_invaders(int speed, struct invaders_t invaders) {
	invaders = set_invaders_speed(invaders);
	switch (invaders.direction) {
		case left:
			for (int i = 0; i < 10; i++) {
				for (int j = 0; j < 5; j++) {
					if (invaders.ennemy[j][i].alive == 1) {
						if (invaders.ennemy[j][i].hitbox.x <= 0) {
							invaders.direction = right;
							invaders = move_invaders_down(invaders);
							return invaders;
						}
						if (invaders.state_time + invaders.state_speed < SDL_GetTicks()) {
							invaders.state_time = SDL_GetTicks();
							if (invaders.state == 1) {
								invaders.state = 0;
							}
							else {
								invaders.state = 1;
							}
						}
						invaders.ennemy[j][i].hitbox.x -= invaders.speed;
					}
				}
			}
			break;
		case right:
			for (int i = 9; i >= 0; i--) {
				for (int j = 0; j < 5; j++) {
					if (invaders.ennemy[j][i].alive == 1) {
						if (invaders.ennemy[j][i].hitbox.x + E_WIDTH >= WIDTH) {
							invaders.direction = left;
							invaders = move_invaders_down(invaders);
							return invaders;
						}
						if (invaders.state_time + invaders.state_speed < SDL_GetTicks()) {
							invaders.state_time = SDL_GetTicks();
							if (invaders.state == 1) {
								invaders.state = 0;
							}
							else {
								invaders.state = 1;
							}
						}
						invaders.ennemy[j][i].hitbox.x += invaders.speed;
					}
				}
			}
			break;
		default:
			break;

	}
	return invaders;
}

//Look for collisions based on ennemy bullet and player rectangles
struct player_t player_hit_collision(struct bullet_t *e_bullets, struct player_t player, enum state_t state, Uint32 pause_time, unsigned int pause_len) {

	int i,c;

	for(i = 0; i < E_BULLETS; i++) {
	
		if (e_bullets[i].alive == 1) {

			c = collision(e_bullets[i].hitbox, player.hitbox);

			if (c == 1) {
				
				if (player.lives >= 0) {
				
					player.lives--;
					state = pause_for(500, state, pause_time, pause_len);
				}
			}
		}
	}
	return player;
}

//Look for collisions based on invader and player rectangles
int ennemy_player_collision(struct invaders_t invaders, struct player_t player, enum state_t state, Uint32 pause_time, unsigned int pause_len) {

	int i,j,c;

	for(i = 0; i < 5; i++) {

		for(j = 0; j < 10; j++) {
		
			if (invaders.ennemy[i][j].alive == 1) {
					
				c = collision(player.hitbox, invaders.ennemy[i][j].hitbox);

				if (c == 1) {
				
					player.lives--;
					state = pause_for(500, state, pause_time, pause_len);
					init_invaders(invaders);
					return 1;
				}
			}
		}
	}

	return 0;
}

//Determine for game over event
enum state_t game_over_ai(struct player_t player, enum state_t state) {
	if (player.lives < 0) {
		state = game_over;
	}
	return state;
}

//Shoot bullet/s from player
void player_shoot(struct bullet_t *bullets, struct score_t score, struct player_t player) {
	int i;
	for (i = 0; i < P_BULLETS; i++) {
		if (bullets[i].alive == 0) {
			score.shots++;
			bullets[i].hitbox.x = player.hitbox.x + (P_WIDTH / 2);
			bullets[i].hitbox.y = player.hitbox.y - (bullets[i].hitbox.h + 10);
			bullets[i].alive = 1;
			break;
		}
	}
}

struct score_t calculate_level(struct invaders_t invaders, struct score_t score, enum state_t state, Uint32 pause_time, unsigned int pause_len) {
	if (invaders.killed != 0 && invaders.killed % 40 == 0) {
		score.level++;
		init_invaders(invaders);
		state = pause_for(500, state, pause_time, pause_len);
	}
	return score;
}

//Determine when invaders should shoot
void ennemy_ai(struct invaders_t invaders, struct player_t player, struct bullet_t *e_bullets) {
	int i, j, k;
	for (i = 0; i < 10; i++) {
		for (j = 4; j >= 0; j--) {
			if (invaders.ennemy[j][i].alive == 1) {
				int mid_point = player.hitbox.x + (player.hitbox.w / 2);
				int start = invaders.ennemy[j][i].hitbox.x;
				int end = invaders.ennemy[j][i].hitbox.x + invaders.ennemy[j][i].hitbox.w;

				if (mid_point > start && mid_point < end) {
					for (k = 0; k < E_BULLETS; k++) {
						if (e_bullets[k].alive == 0) {
							int r = rand() % 30;
							if (r == 1) {
								e_bullets[k].hitbox.x = start + (E_WIDTH / 2) ;
								e_bullets[k].hitbox.y = invaders.ennemy[j][i].hitbox.y;
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

//Determin when to pause game and how long for
void pause_game(Uint32 pause_time, unsigned int pause_len, enum state_t state) {
	if (SDL_GetTicks() > pause_time + pause_len) {
		state = game;
	}
}



//Load image files
int load_image(char filename[], SDL_Surface **surface, enum ck_t colour_key) {
	SDL_Surface *temp;
	temp = SDL_LoadBMP(filename);
    Uint32 colourkey;
	if (temp == NULL) {
		printf("Unable to load %s.\n", filename);
		return 1;
	}

	if (colour_key == magenta) {
		colourkey = SDL_MapRGB(temp->format, 255, 0, 255);
	} else if (colour_key == lime) {
		colourkey = SDL_MapRGB(temp->format, 0, 255, 0);
	}

	SDL_SetColorKey(temp, SDL_SRCCOLORKEY, colourkey);

	//convert the image surface to the same type as the screen
	(*surface) = SDL_DisplayFormat(temp);
	
	if ((*surface) == NULL) {
		printf("Unable to convert bitmap.\n");
		return 1;
	}
	
	SDL_FreeSurface(temp);

	return 0;
}
    static SDL_Surface *screen;
    static SDL_Surface *title_screen;
    static SDL_Surface *cmap;
    static SDL_Surface *invadersmap;
    static SDL_Surface *player_img;
    static SDL_Surface *saucer_img;
    static SDL_Surface *damage_img;
    static SDL_Surface *damage_top_img;
    static SDL_Surface *game_over_img;
    struct score_t score;
    struct invaders_t invaders;
    static struct player_t player;
    struct bullet_t bullets[P_BULLETS];
    struct bullet_t e_bullets[E_BULLETS];
    unsigned int pause_len;
    Uint32 pause_time;
    enum state_t state;
    Uint32 title_time;

//Main program
int main() {
	/* Initialize SDL’s video system and check for errors */
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

    title_screen = IMG_Load("title_screen.png");
    cmap = IMG_Load("cmap.png");
    invadersmap = IMG_Load("invaders.png");
    player_img = IMG_Load("player_ship.png");
    game_over_img = IMG_Load("gameover.png");
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
