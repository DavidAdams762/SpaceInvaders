#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>

void pause();

int main(int argc, char *argv[])
{
  SDL_Surface *screen = NULL;
  SDL_Surface *background = NULL;
  SDL_Rect positionBackground;

  positionBackground.x = 0;
  positionBackground.y = 0;

  SDL_Init(SDL_INIT_VIDEO);

  screen = SDL_SetVideoMode(800, 600, 32, SDL_HWSURFACE);
  SDL_WM_SetCaption("Load images", NULL);
  background = SDL_LoadBMP("images/background.bmp");
  SDL_BlitSurface(background, NULL, screen, &positionBackground);

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
