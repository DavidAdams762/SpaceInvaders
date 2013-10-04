#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "space.h"

struct score_t init_score(struct score_t score) {
	score.shots = 0;
	score.points = 0;
	score.level = 1;
	score.passed = 0;
	return score;
}

struct score_t update_score(struct invaders_t invaders, struct score_t score) {
    score.points = invaders.score;
    return score;
}

struct score_t calculate_level(struct invaders_t invaders, struct score_t score, enum state_t state, Uint32 pause_time, unsigned int pause_len) {
	if (invaders.killed != 0 && invaders.killed % 40 == 0) {
		score.level++;
		score.passed = 1;
		state = pause_for(500, state, pause_time, pause_len);
	}
	return score;
}

enum state_t game_over_ai(struct player_t player, enum state_t state) {
	if (player.lives < 0) {
		state = game_over;
	}
	return state;
}