#include <stdlib.h>
#include <stdio.h>
#include "SDL/SDL.h"

#define SCREEN_WIDTH 800 
#define SCREEN_HEIGHT 600
#define E_WIDTH 30
#define WIDTH 600 
#define HEIGHT 600
#define E_WIDTH 30
#define E_HEIGHT 30
#define P_WIDTH 30
#define P_HEIGHT 10
#define B_WIDTH 5
#define B_HEIGHT 15
#define P_BULLETS 1
#define E_BULLETS 3

enum colour_t {red, green, purple};
enum direction_t {left, right, stationary};
enum state_t {menu, options, game, game_over, pause};
enum ck_t {magenta, lime};

struct score_t {
	unsigned int shots;
	unsigned int points;
	unsigned int level;
};

struct ennemy_t {
	SDL_Rect hitbox;
	enum colour_t colour;
	unsigned int alive;
	unsigned int points;
};

struct invaders_t {
	struct ennemy_t ennemy[5][10];
	enum direction_t direction;
	unsigned int killed;
	int speed;
	int state;
	int state_speed;
	Uint32 state_time;
};

struct player_t {
	SDL_Rect hitbox;
	int lives;
};

struct bullet_t {
	SDL_Rect hitbox;
	unsigned int alive;
};

//global variables, for convenience.
static SDL_Surface *screen;
static SDL_Surface *title_screen;
static SDL_Surface *cmap;
static SDL_Surface *invadersmap;
static SDL_Surface *player_img;
static SDL_Surface *damage_img;
static SDL_Surface *damage_top_img;
static SDL_Surface *game_over_img;
struct score_t score;
struct invaders_t invaders;
struct player_t player;
struct bullet_t bullets[P_BULLETS];
struct bullet_t e_bullets[E_BULLETS];
unsigned int pause_len;
Uint32 pause_time;
enum state_t state;
Uint32 title_time;

void draw_string(char s[], int x, int y);
void pause_for(unsigned int len);

int load_image(char filename[], SDL_Surface **surface, enum ck_t colour_key);

//Initialize the score structure and game state
void init_score() {
	score.shots = 0;
	score.points = 0;
	score.level = 1;
}

//Initialize the enemies starting positions, direction, speed and colour
void init_invaders() {
	invaders.direction = right;
	invaders.speed = 1;
	invaders.state = 0;
	invaders.killed = 0;
	invaders.state_speed = 1000;
	invaders.state_time = SDL_GetTicks();

	int i,j;
	int x = 0;
	int y = 30;
	
	for (i = 0; i < 5; i++) {
	
		for (j = 0; j < 8; j++) {
		
			invaders.ennemy[i][j].alive = 1;
			invaders.ennemy[i][j].hitbox.x = x;
			invaders.ennemy[i][j].hitbox.y = y;
			invaders.ennemy[i][j].hitbox.w = E_WIDTH;
			invaders.ennemy[i][j].hitbox.h = E_HEIGHT;
			
			x += E_WIDTH + 20; // gap size
			
			if (i == 0) {
				
				invaders.ennemy[i][j].colour = purple;
				invaders.ennemy[i][j].points = 30;
	
			} else if (i >= 1 && i < 3) {
			
				invaders.ennemy[i][j].colour = green;
				invaders.ennemy[i][j].points = 20;
		
			} else {
		
				invaders.ennemy[i][j].colour = red;
				invaders.ennemy[i][j].points = 10;
			}
		}
		
		x = 0; //reset line
		y += E_HEIGHT + 20;
	}
}

//Initialize the player starting position and dimensions
void init_player() {

	player.hitbox.x = (WIDTH / 2) - (P_WIDTH / 2);
	player.hitbox.y = HEIGHT - (P_HEIGHT + 10);
	player.hitbox.w = P_WIDTH;
	player.hitbox.h = P_HEIGHT;
	player.lives = 3;
}

//Initialize the player bullets dimensions
void init_bullets(struct bullet_t b[], int max) {

	int i;

	for (i = 0; i < max; i++) {
	
		b[i].alive = 0;
		b[i].hitbox.x = 0;
		b[i].hitbox.y = 0;
		b[i].hitbox.w = B_WIDTH;
		b[i].hitbox.h = B_HEIGHT;
	}
}

//Draw the background
void draw_background () {

	SDL_Rect src;

	src.x = 0;
	src.y = 0;
	src.w = screen->w;
	src.h = screen->h;
	
	SDL_FillRect(screen,&src,0);
}

//Draw the HUD
void draw_hud() {
	
	SDL_Rect r;
	
	r.x = WIDTH;
	r.y = 0;
	r.w = SCREEN_WIDTH - WIDTH;
	r.h = SCREEN_HEIGHT;

	SDL_FillRect(screen, &r, 41);
	
	char score_label[] = "Score";
	draw_string(score_label, WIDTH, 0);
	
	char score_num[10];
	snprintf(score_num, 10, "%d", score.points);
	draw_string(score_num, WIDTH, 20);

	char level[] = "Level";
	draw_string(level, WIDTH, 60);
	
	char level_num[2];
	snprintf(level_num, 2, "%d", score.level);
	draw_string(level_num, WIDTH, 80);
	
	char lives[] = "Lives";
	draw_string(lives, WIDTH, 120);
	
	char lives_num[2];
	snprintf(lives_num, 2, "%d", player.lives);
	draw_string(lives_num, WIDTH, 140);

	char credits[] = "Made by";
    draw_string(credits, WIDTH, 480);

	char name1[] = "David";
	draw_string(name1, WIDTH, 500);

	char name2[] = "Eddy";
    draw_string(name2, WIDTH, 520);

	char name3[] = "Kenny";
	draw_string(name3, WIDTH, 540);

	char name4[] = "Ophelie";
    draw_string(name4, WIDTH, 560);
}

//Draw the title screen
void draw_title_screen() {
	
	SDL_Rect src;
	SDL_Rect dest;

	src.x = 0;
	src.y = 0;
	src.w = title_screen->w;
	src.h = title_screen->h;

	dest.x = (SCREEN_WIDTH / 2) - (title_screen->w / 2);
	dest.y = 0;
	dest.w = title_screen->w;
	dest.h = title_screen->h;
	
	SDL_BlitSurface(title_screen, &src, screen, &dest);
}

//Draw the invaders if there alive
void draw_invaders() {

	SDL_Rect src, dest;
	int i,j;
	
	src.w = E_WIDTH;
	src.h = E_HEIGHT;
	
	for (i = 0; i < 5; i++) {
		
		for (j = 0; j < 8; j++) {
			
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
				} else if (i > 0 && i < 3) {
					
					if (invaders.state == 0) {
						
						src.x = 0;
						src.y = E_HEIGHT;

					} else {
						
						src.x = 30;
						src.y = E_HEIGHT;
					}

				//red
				} else {
					
					if (invaders.state == 0) {
					
						src.x = 0;
						src.y = E_HEIGHT * 2;
					
					} else {
					
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

//Draw the player
void draw_player() {

	SDL_Rect src;

	src.x = 0;
	src.y = 0;
	src.w = P_WIDTH;
	src.h = P_HEIGHT;

	SDL_BlitSurface(player_img, &src, screen, &player.hitbox);
}

//Draw both the ennemy and the players bullets if there alive
void draw_bullets(struct bullet_t b[], int max) {

	//Uint8 c = SDL_MapRGB(screen->format, 255, 255, 255);
	int i;


	for (i = 0; i < max; i++) {
	
		if (b[i].alive == 1) {
		
			SDL_FillRect(screen, &b[i].hitbox, 255);
		}
	}
}

//Draw a char
int draw_char(char c, int x, int y) {

	SDL_Rect src;
	SDL_Rect dest;
	int i,j;
	char *map[] = {"ABCDEFGHIJKLMNOPQRSTUVWXYZ",
			"abcdefghijklmnopqrstuvwxyz",
			"!@#$%^&*()_+{}|:\"<>?,.;'-=",
			"0123456789"};

	src.x = 0;
	src.y = 0;
	src.w = 20;
	src.h = 20;
	
	dest.x = x;
	dest.y = y;
	dest.w = 20;
	dest.h = 20;

	for (i = 0; i < 4; i++) {
	
		for(j = 0; j < strlen(map[i]); j++) {
			
			if (c == map[i][j]) {
			
				SDL_BlitSurface(cmap, &src, screen, &dest);
				return 0;
			}

			src.x += 20;
		}
	
		src.y += 20;//move down one line on the image file
		src.x = 0; //reset to start of line
	}

	return 0;
}

//Draw a string of chars
void draw_string(char s[], int x, int y) {

	int i;

	for (i = 0; i < strlen(s); i++) {
	
		draw_char(s[i],x,y);
		x += 20;
	}
}

//Draw Game Over message
void draw_game_over() {
		
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

//Set invader movment speed
void set_invaders_speed() {

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

			invaders.speed = 8;
			invaders.state_speed = 200;
			break;
		
		case 40:

			invaders.speed = 16;
			invaders.state_speed = 0;
			break;
	}
}

//Move positions of both ennemy and player bullets on screen
int move_bullets(struct bullet_t b[], int max, int speed) {
	
	int i;

	for(i = 0; i < max; i++) {
	
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

//Move invaders down one space once the reach the edge
void move_invaders_down() {

	int i,j;

	for (i = 0; i < 5; i++) {
		
		for (j = 0; j < 10; j++) {
		
			invaders.ennemy[i][j].hitbox.y += 15;
		}
	}
}

//Move invaders based on there current direction
int move_invaders(int speed) {
	
	set_invaders_speed();
	
	int i,j;

	switch (invaders.direction) {
		
		case left:
		
			for (i = 0; i < 10; i++) {
			
				for (j = 0; j < 5; j++) {
				
					if (invaders.ennemy[j][i].alive == 1) {
		
						if (invaders.ennemy[j][i].hitbox.x <= 0) {
						
							invaders.direction = right;
							move_invaders_down();
							return 0;
						}
						
						if (invaders.state_time + invaders.state_speed < SDL_GetTicks()) {
						
							invaders.state_time = SDL_GetTicks();
							
							if (invaders.state == 1) {
								
								invaders.state = 0;

							} else {
								
								invaders.state = 1;
							}
						}
						
						//move invader speed number of pixels
						invaders.ennemy[j][i].hitbox.x -= invaders.speed;
					}
				}
			}

			break;

		case right:
			
			for (i = 9; i >= 0; i--) {
			
				for (j = 0; j < 5; j++) {
				
					if (invaders.ennemy[j][i].alive == 1) {
					
						if (invaders.ennemy[j][i].hitbox.x + E_WIDTH >= WIDTH) {
					
							invaders.direction = left;
							move_invaders_down();
							return 0;
						}
		
						if (invaders.state_time + invaders.state_speed < SDL_GetTicks()) {
						
							invaders.state_time = SDL_GetTicks();

							if (invaders.state == 1) {
								
								invaders.state = 0;

							} else {
								
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

	return 0;
}

//Move player left or right
void move_player(enum direction_t direction) {

	if (direction == left) {
			
		if (player.hitbox.x > 0) {
			
			player.hitbox.x -= 10;
		}

	} else if (direction == right) {

		if (player.hitbox.x + player.hitbox.w < WIDTH){

			player.hitbox.x += 10;
		}
	}
}

//Detect any collision between any two non rotated rectangles
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

//Look for collisions based on player bullet and invader rectangles
void ennemy_hit_collision() {

	int i,j,k,c;
	
	for (i = 0; i < 5; i++) {
		
		for (j = 0; j < 10; j++) {
			
			if (invaders.ennemy[i][j].alive == 1) {
			
				for (k = 0; k < P_BULLETS; k++) {
			
					if (bullets[k].alive == 1) {
						
						c = collision(bullets[k].hitbox, invaders.ennemy[i][j].hitbox);
				
						if (c == 1) {
				
							invaders.ennemy[i][j].alive = 0;
							bullets[k].alive = 0;
							bullets[k].hitbox.x = 0;
							bullets[k].hitbox.y = 0;
							invaders.killed++;
							score.points += invaders.ennemy[i][j].points;
						}
					}
				}
			}
		}
	}
}

//Look for collisions based on ennemy bullet and player rectangles
void player_hit_collision() {

	int i,c;

	for(i = 0; i < E_BULLETS; i++) {
	
		if (e_bullets[i].alive == 1) {

			c = collision(e_bullets[i].hitbox, player.hitbox);

			if (c == 1) {
				
				if (player.lives >= 0) {
				
					player.lives--;
					pause_for(500);
				}
			}
		}
	}
}

//Look for collisions based on invader and player rectangles
int ennemy_player_collision() {

	int i,j,c;

	for(i = 0; i < 5; i++) {

		for(j = 0; j < 10; j++) {
		
			if (invaders.ennemy[i][j].alive == 1) {
					
				c = collision(player.hitbox, invaders.ennemy[i][j].hitbox);

				if (c == 1) {
				
					player.lives--;
					pause_for(500);
					init_invaders();
//					init_bases();
					return 1;
				}
			}
		}
	}

	return 0;
}

//Determine for game over event
void game_over_ai() {
	
	if (player.lives < 0) {
		
		state = game_over;
	}
}

//Shoot bullet/s from player
void player_shoot() {

	int i;
	
	for (i = 0; i < P_BULLETS; i++) {
				
		if (bullets[i].alive == 0) {
			
			//count number of shots fired
			score.shots++;

			bullets[i].hitbox.x = player.hitbox.x + (P_WIDTH / 2);
			//-5 at the end so the bullets ends closer to the top of the screen due to 30px speed
			bullets[i].hitbox.y = player.hitbox.y - (bullets[i].hitbox.h + 10);
			bullets[i].alive = 1;
			break;
		}
	}
}

//Determine game level
void calculate_level() {

	if (invaders.killed != 0 && invaders.killed % 40 == 0) {
		
		score.level++;
		init_invaders();
//		init_bases();
//		init_saucer();
		pause_for(500);
	}
}

//Determine when invaders should shoot
void ennemy_ai() {

	int i, j, k;

	for (i = 0; i < 10; i++) {
		
		for (j = 4; j >= 0; j--) {
			
			if (invaders.ennemy[j][i].alive == 1) {
				
				//player
				int mid_point = player.hitbox.x + (player.hitbox.w / 2);
				
				//ennemy
				int start = invaders.ennemy[j][i].hitbox.x;
				int end = invaders.ennemy[j][i].hitbox.x + invaders.ennemy[j][i].hitbox.w;

				if (mid_point > start && mid_point < end) {

					//fire bullet if available
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
				
				//alive ennemy found reversing up the ennemy grid dont check the rest of the colum
				break;
			}
		}
	}
}

//Determin when to pause game and how long for
void pause_game() {

	if (SDL_GetTicks() > pause_time + pause_len) {
	
		state = game;
	}
}

//Set amount of time to pause game for
void pause_for(unsigned int len) {

	state = pause;
	pause_time = SDL_GetTicks();
	pause_len = len;
}

//Load image files
int load_image(char filename[], SDL_Surface **surface, enum ck_t colour_key) {
	
	SDL_Surface *temp;
	
	//load image 
	temp = SDL_LoadBMP(filename);
	
	if (temp == NULL) {
	
		printf("Unable to load %s.\n", filename);
		return 1;
	}

	Uint32 colourkey;

	/* Set the image colorkey. */
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

//Main program
int main() {
	
	/* Initialize SDL’s video system and check for errors */
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {

		printf("Unable to initialize SDL: %s\n", SDL_GetError());
		return 1;
	}
	
	/* Make sure SDL_Quit gets called when the program exits! */
	atexit(SDL_Quit);
	
	/*set window title*/
	SDL_WM_SetCaption("Space Invaders", "P");
	
	/* Attempt to set a 800x600 8 bit color video mode */
	screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 8, SDL_DOUBLEBUF );
	
	if (screen == NULL) {
		
		printf("Unable to set video mode: %s\n", SDL_GetError());
		return 1;
	}

	//load images
	load_image("titlescreen.bmp", &title_screen, magenta);
	load_image("cmap.bmp", &cmap, magenta);
	load_image("invaders.bmp", &invadersmap, magenta);
	load_image("player.bmp", &player_img, magenta);
	load_image("gameover.bmp", &game_over_img, magenta);
	load_image("damage.bmp", &damage_img, lime);
	load_image("damagetop.bmp", &damage_top_img, lime);

	Uint32 next_game_tick = SDL_GetTicks();
	int sleep = 0;
	Uint8 *keystate = 0;
	int quit = 0;
	SDL_Event event;

	init_score();
	init_invaders();
	init_player();
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
								
								player_shoot();

							} else if (state == game_over) {
							
								init_invaders();
								init_score();
								init_player();
								state = game;
							}
						break;
						
						default:
						break;
					}
				break;
			}
		}
	
		draw_background();

		if (state == menu) {
			
			char s[] = "Press SPACEBAR to start";
			SDL_Rect src[60];
			
			int i;

			if (title_time + 2000 < SDL_GetTicks())  {
			
				src[0].x = 180;
				src[0].y = 40;
				src[0].w = 440;
				src[0].h = 230;
			
				SDL_FillRect(screen, &src[0], 248);
			
			} else {
			
				int y = 0;

				for (i = 0; i < 60; i++) {
				
					src[i].x = 0;
					src[i].y = y;
					src[i].w = SCREEN_WIDTH;
					src[i].h = 10;

					SDL_FillRect(screen, &src[i], 227);
				
					y += 10;							
				}
			
				for (i = 0; i < 60; i++) {

					SDL_FillRect(screen, &src[i], rand() % 255);

				}
			}
			
			draw_title_screen();
			draw_string(s, (SCREEN_WIDTH / 2) - (strlen(s) * 10), 500);

		} else if (state == game) {

			//move player
			if (keystate[SDLK_LEFT]) {
				
				move_player(left);
			}

			if (keystate[SDLK_RIGHT]) {
				
				move_player(right);
			}

			draw_hud();
			draw_player();
			draw_invaders();
			draw_bullets(bullets, P_BULLETS);
			draw_bullets(e_bullets, E_BULLETS);
			ennemy_hit_collision();
			player_hit_collision();
			ennemy_player_collision();
			move_invaders(invaders.speed);
			move_bullets(bullets, P_BULLETS, -30);
			move_bullets(e_bullets, E_BULLETS, 15);
			calculate_level();
			ennemy_ai();
			game_over_ai();
			pause_game();
		
		} else if (state == game_over) {
			
			draw_hud();
			draw_player();
			draw_invaders();
			draw_bullets(bullets, P_BULLETS);
			draw_bullets(e_bullets, E_BULLETS);
			draw_game_over();
		
		} else if (state == pause) {
			
			draw_hud();
			draw_player();
			draw_invaders();
			draw_bullets(bullets, P_BULLETS);
			draw_bullets(e_bullets, E_BULLETS);
			pause_game();
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
