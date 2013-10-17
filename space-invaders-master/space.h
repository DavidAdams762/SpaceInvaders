/*
** space.h for  in /Users/Ophelie/Sites/SpaceInvaders/space-invaders-master
** 
** Made by Ophelie Volga
** Login   <volga_o@etna-alternance.net>
** 
** Started on  Thu Oct 17 21:49:37 2013 Ophelie Volga
** Last update Thu Oct 17 21:50:05 2013 Ophelie Volga
*/
#ifndef _SPACE_H
#define _SPACE_H

#define WIDTH 800
#define HEIGHT 600
#define BULLET_W 5
#define BULLET_H 15
#define PLAYER_W 30
#define PLAYER_H 10
#define PLAYER_B 1
#define ENNEMY_W 30
#define ENNEMY_H 30
#define ENNEMY_B 3

enum direction_t {left, right, stationary};
enum state_t {menu, options, game, game_over, pause};

struct score_t
{
  int shots;
  int points;
  int level;
  int passed;
};

struct ennemy_t
{
  SDL_Rect hitbox;
  int alive;
  int points;
};

struct ennemies_t
{
  struct ennemy_t ennemy[5][10];
  enum direction_t direction;
  int killed;
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

struct bullets_t
{
  SDL_Rect hitbox;
  int alive;
};


struct score_t check_level(struct ennemies_t ennemies, struct score_t score, enum state_t state, Uint32 pause_time, int pause_len);
int collision(SDL_Rect a, SDL_Rect b);

void display_background (SDL_Surface *screen);
void display_bullets(struct bullets_t b[], int max, SDL_Surface *screen);
int display_char(char c, int x, int y, SDL_Surface *font_img, SDL_Surface *screen);
void display_ennemies(struct ennemies_t ennemies, SDL_Surface *ennemies_img, SDL_Surface *screen);
void display_game_over(SDL_Surface *game_over_img, SDL_Surface *screen);
void display_player(SDL_Surface *player_img, SDL_Surface *screen, struct player_t player);
char *display_scores(struct score_t score, SDL_Surface *screen, SDL_Surface *font_img);
void display_status(SDL_Surface *screen, struct score_t score, struct player_t player, SDL_Surface *font_img);
void display_string(char s[], int x, int y, SDL_Surface *font_img, SDL_Surface *screen);
void display_title_screen(SDL_Surface *title_screen, SDL_Surface *screen);

void ennemy_moving(struct ennemies_t ennemies, struct player_t player, struct bullets_t *ennemy_bullets);
struct ennemies_t ennemy_hit_collision(struct ennemies_t ennemies, struct bullets_t *bullets, struct score_t score);
struct ennemies_t ennemy_player_collision(struct ennemies_t ennemies, struct player_t player, enum state_t state, Uint32 pause_time, int pause_len);

int file_exists(const char * filename);

void reset_bullets(struct bullets_t b[], int max);
struct ennemies_t reset_ennemies(struct ennemies_t ennemies);
struct player_t reset_player(struct player_t player);
struct score_t reset_score(struct score_t score);

int move_bullets(struct bullets_t b[], int max, int speed);
struct ennemies_t move_ennemies(int speed, struct ennemies_t ennemies);
struct ennemies_t move_ennemies_down(struct ennemies_t ennemies);

struct player_t move_player(enum direction_t direction, struct player_t player);

void pause_game(Uint32 pause_time, int pause_len, enum state_t state);
struct player_t player_hit_collision(struct bullets_t *ennemy_bullets, struct player_t player, enum state_t state, Uint32 pause_time, int pause_len);
void player_shoot(struct bullets_t *bullets, struct score_t score, struct player_t player);

struct ennemies_t set_ennemies_speed(struct ennemies_t ennemies);
enum state_t state_game_over(struct player_t player, enum state_t state);

struct score_t update_score(struct ennemies_t ennemies, struct score_t score);

void create_file_scores(char *file);
char *get_scores(char *file, int score);
void change_scores(char *file, char *tmp);
char *get_format_scores(char *);
#endif
