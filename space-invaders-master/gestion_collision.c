#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "space.h"

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

struct invaders_t ennemy_player_collision(struct invaders_t invaders, struct player_t player, enum state_t state, Uint32 pause_time, unsigned int pause_len) {

	int i,j,c;

	for(i = 0; i < 5; i++) {

		for(j = 0; j < 10; j++) {

			if (invaders.ennemy[i][j].alive == 1) {

				c = collision(player.hitbox, invaders.ennemy[i][j].hitbox);

				if (c == 1) {

					player.lives--;
					state = pause_for(500, state, pause_time, pause_len);
					invaders = init_invaders(invaders);
					return invaders;
				}
			}
		}
	}

	return invaders;
}