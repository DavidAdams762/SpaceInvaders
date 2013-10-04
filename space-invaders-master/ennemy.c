#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "space.h"

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