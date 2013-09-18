#include <SDL/SDL.h>
#include <SDL/SDL_main.h>
#include "util.h"
#include "globals.h"
#include "input.h"
#include "player.h"
#include "enemy.h"
#include "bullet.h"
#include "collider.h"
#include "images.h"

player_t *player;

bool init()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) == -1)
		return false;

	init_globals();

	SDL_SetVideoMode(globals.window.width,
					 globals.window.height, 
					 globals.window.bpp, 
					 SDL_HWSURFACE);

	globals.screen = SDL_GetVideoSurface();

	if (globals.screen == NULL) {
		return false;
	}

	SDL_WM_SetCaption("Vaders", NULL);

	input_init();

	init_image_assets();
	push_image_asset("tank", load_image("assets/tank.png"));
	push_image_asset("bullet", load_image("assets/bullet.png"));
	push_image_asset("enemy1", load_image("assets/enemy.png"));

	return true;
}

void cleanup()
{
	input_cleanup();
	SDL_FreeSurface(globals.screen);
	free_image_assets();
	SDL_Quit();
}

void update()
{
	player_update(player);
	enemies_update();
	bullets_update();
	colliders_update();
}

void draw()
{
	player_draw(player);
	enemies_draw();
	bullets_draw();

	// colliders_debug_draw();
}

int main(int argc, char* argv[])
{
	SDL_Event osEvent;
	
	if (init() == false) {
		return EXIT_FAILURE;
	}

	player = player_make();

	colliders_init();
	bullets_init();
	enemies_init();
	// enemies_reset_formation();

	while (globals.bRun) {
		while (SDL_PollEvent(&osEvent)) {
			if (osEvent.type == SDL_QUIT) {
				globals.bRun = false;
			}
		}

		globals.time.sinceStart = SDL_GetTicks() - globals.time.start;
		globals.time.delta = SDL_GetTicks() - globals.time.last;
		globals.time.deltaf = (float)globals.time.delta / 1000.0f;
		globals.time.last = SDL_GetTicks();

		SDL_FillRect(globals.screen, NULL, SDL_MapRGB(globals.screen->format, 0, 0, 0));

		if (input_get_key_down(SDLK_ESCAPE)) {
			globals.bRun = false;
		}

		update();
		draw();

		input_update();
		SDL_Flip(globals.screen);
	}

	cleanup();
	return EXIT_SUCCESS;
}
