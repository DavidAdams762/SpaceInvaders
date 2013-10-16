#ifndef _SPACE_H
#define _SPACE_H

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define WIDTH 800
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

struct score_t
{
  unsigned int shots;
  unsigned int points;
  unsigned int level;
  int passed;
};

struct ennemy_t
{
  SDL_Rect hitbox;
  enum colour_t colour;
  unsigned int alive;
  unsigned int points;
};

struct ennemies_t
{
  struct ennemy_t ennemy[5][10];
  enum direction_t direction;
  unsigned int killed;
  int speed;
  int state;
  int state_speed;
  int score;
  Uint32 state_time;
};

struct player_t
{
  SDL_Rect hitbox;
  int lives;
};

struct bullet_t
{
  SDL_Rect hitbox;
  unsigned int alive;
};


struct score_t calculate_level(struct ennemies_t ennemies, struct score_t score, enum state_t state, Uint32 pause_time, unsigned int pause_len);
int collision(SDL_Rect a, SDL_Rect b);

void display_background (SDL_Surface *screen);
void display_bullets(struct bullet_t b[], int max, SDL_Surface *screen);
int display_char(char c, int x, int y, SDL_Surface *cmap, SDL_Surface *screen);
void display_game_over(SDL_Surface *game_over_img, SDL_Surface *screen);
void display_hud(SDL_Surface *screen, struct score_t score, struct player_t player, SDL_Surface *cmap);
void display_ennemies(struct ennemies_t ennemies, SDL_Surface *ennemies_img, SDL_Surface *screen);
void display_player(SDL_Surface *player_img, SDL_Surface *screen, struct player_t player);
void display_title_screen(SDL_Surface *title_screen, SDL_Surface *screen);
void display_string(char s[], int x, int y, SDL_Surface *cmap, SDL_Surface *screen);
char *display_scores(struct score_t score, SDL_Surface *screen, SDL_Surface *cmap);

void ennemy_ai(struct ennemies_t ennemies, struct player_t player, struct bullet_t *e_bullets);
struct ennemies_t ennemy_hit_collision(struct ennemies_t ennemies, struct bullet_t *bullets, struct score_t score);
struct ennemies_t ennemy_player_collision(struct ennemies_t ennemies, struct player_t player, enum state_t state, Uint32 pause_time, unsigned int pause_len);

enum state_t game_over_ai(struct player_t player, enum state_t state);

void reset_bullets(struct bullet_t b[], int max);
struct ennemies_t reset_ennemies(struct ennemies_t ennemies);
struct player_t reset_player(struct player_t player);
struct score_t reset_score(struct score_t score);

int load_image(char filename[], SDL_Surface **surface, enum ck_t colour_key);

int move_bullets(struct bullet_t b[], int max, int speed);
struct ennemies_t move_ennemies(int speed, struct ennemies_t ennemies);
struct ennemies_t move_ennemies_down(struct ennemies_t ennemies);

struct player_t move_player(enum direction_t direction, struct player_t player);

void pause_game(Uint32 pause_time, unsigned int pause_len, enum state_t state);
struct player_t player_hit_collision(struct bullet_t *e_bullets, struct player_t player, enum state_t state, Uint32 pause_time, unsigned int pause_len);
void player_shoot(struct bullet_t *bullets, struct score_t score, struct player_t player);

struct ennemies_t set_ennemies_speed(struct ennemies_t ennemies);

struct score_t update_score(struct ennemies_t ennemies, struct score_t score);


int file_exists(const char * filename);
#endif
