#ifndef _SPACE_H
#define _SPACE_H

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
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

SDL_Surface *screen;
SDL_Surface *title_screen;
SDL_Surface *cmap;
SDL_Surface *invadersmap;
SDL_Surface *player_img;
SDL_Surface *damage_img;
SDL_Surface *damage_top_img;
SDL_Surface *game_over_img;
struct invaders_t invaders;

int load_image(char filename[], SDL_Surface **surface, enum ck_t colour_key);
void init_invaders();
void draw_invaders(struct invaders_t invaders, SDL_Surface *invadersmap, SDL_Surface *screen);
void set_invaders_speed(struct invaders_t invaders);
void move_invaders_down();
int move_invaders(int speed, struct invaders_t invaders);

#endif