#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "space.h"

struct score_t reset_score(struct score_t score) {
	score.shots = 0;
	score.points = 0;
	score.level = 1;
	score.passed = 0;
	return score;
}

struct score_t update_score(struct ennemies_t ennemies, struct score_t score) {
    score.points = ennemies.score;
    return score;
}

struct score_t calculate_level(struct ennemies_t ennemies, struct score_t score, enum state_t state, Uint32 pause_time, unsigned int pause_len) {
	if (ennemies.killed != 0 && ennemies.killed % 40 == 0) {
		score.level++;
		score.passed = 1;
	}
	return score;
}

enum state_t game_over_ai(struct player_t player, enum state_t state) {
	if (player.lives < 0) {
		state = game_over;
	}
	return state;
}