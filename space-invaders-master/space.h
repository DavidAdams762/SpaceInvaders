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
	int score;
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

/*void calculate_level(struct invaders_t invaders, struct score_t score, enum state_t state, Uint32 pause_time, unsigned int pause_len);
int collision(SDL_Rect a, SDL_Rect b);

void draw_background (SDL_Surface *screen);
void draw_bullets(struct bullet_t b[], int max, SDL_Surface *screen);
int draw_char(char c, int x, int y, SDL_Surface *cmap, SDL_Surface *screen);
void draw_game_over(SDL_Surface *game_over_img, SDL_Surface *screen);
void draw_hud(SDL_Surface *screen, struct score_t score, struct player_t player, SDL_Surface *cmap);
void draw_invaders(struct invaders_t invaders, SDL_Surface *invadersmap, SDL_Surface *screen);
void draw_player(SDL_Surface *player_img, SDL_Surface *screen, struct player_t player);
void draw_title_screen(SDL_Surface *title_screen, SDL_Surface *screen);
void draw_string(char s[], int x, int y, SDL_Surface *cmap, SDL_Surface *screen);

void ennemy_ai(struct invaders_t invaders, struct player_t player, struct bullet_t *e_bullets);
void ennemy_hit_collision(struct invaders_t invaders, struct bullet_t *bullets, struct score_t score);
int ennemy_player_collision(struct invaders_t invaders, struct player_t player, enum state_t state, Uint32 pause_time, unsigned int pause_len);

void game_over_ai(struct player_t player, enum state_t state);

void init_invaders(struct invaders_t invaders);
void init_player(struct player_t player);
void init_score(struct score_t score);

int load_image(char filename[], SDL_Surface **surface, enum ck_t colour_key);

int move_bullets(struct bullet_t b[], int max, int speed);
int move_invaders(int speed, struct invaders_t invaders);
void move_invaders_down(struct invaders_t invaders);

void move_player(enum direction_t direction, struct player_t player);

void pause_for(unsigned int len, enum state_t state, Uint32 pause_time, unsigned int pause_len);
void pause_game(Uint32 pause_time, unsigned int pause_len, enum state_t state);
void player_hit_collision(struct bullet_t *e_bullets, struct player_t player, enum state_t state, Uint32 pause_time, unsigned int pause_len);
void player_shoot(struct bullet_t *bullets, struct score_t score, struct player_t player);

void set_invaders_speed(struct invaders_t invaders);

struct score_t update_score(struct invaders_t invaders, struct score_t score);*/

#endif