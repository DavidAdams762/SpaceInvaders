#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>

void pause();

int main(int argc, char *argv[])
{
  SDL_Surface *screen = NULL;

  SDL_Init(SDL_INIT_VIDEO);

  screen = SDL_SetVideoMode(800, 600, 32, SDL_HWSURFACE);
  if (screen == NULL)
    {
      fprintf(stderr, "Impossible de charger le mode vidéo : %s\n", SDL_GetError());
      exit(EXIT_FAILURE);
    }

  SDL_WM_SetCaption("Space Invaders", NULL);

  SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));

  SDL_Flip(screen);

  pause();

  SDL_Quit();

  return EXIT_SUCCESS;
}

void pause()
{
  int continuer = 1;
  SDL_Event event;

  while (continuer)
    {
      SDL_WaitEvent(&event);
      switch(event.type)
        {
	case SDL_QUIT:
	  continuer = 0;
        }
    }
}
