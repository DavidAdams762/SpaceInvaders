#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

// void pause();

int main(int argc, char *argv[])
{
  SDL_Surface *screen = NULL;
  SDL_Surface *background = NULL;
  SDL_Surface *ship = NULL;
  SDL_Surface *shot = NULL;
  SDL_Rect positionBackground;
  SDL_Rect positionShip;
  SDL_Rect positionShot;
  int continuer = 1;
  SDL_Event event;

  positionBackground.x = 0;
  positionBackground.y = 0;
  positionShip.x = (800 / 2) - 32;
  positionShip.y = 600 / 1.2;

  SDL_Init(SDL_INIT_VIDEO);

  screen = SDL_SetVideoMode(800, 600, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
  SDL_WM_SetCaption("Load images", NULL);
  background = SDL_LoadBMP("images/background.bmp");
  SDL_BlitSurface(background, NULL, screen, &positionBackground);

  ship = IMG_Load("images/ship.png");
  SDL_BlitSurface(ship, NULL, screen, &positionShip);

  SDL_Flip(screen);

  SDL_EnableKeyRepeat(10, 10);

  while (continuer)
    {
      SDL_WaitEvent(&event);
      switch(event.type)
        {
	        case SDL_QUIT:
                continuer = 0;
                break;
            case SDL_KEYDOWN:
                switch(event.key.keysym.sym)
                {
                    case SDLK_ESCAPE:
                        continuer = 0;
                        break;
                    case SDLK_RIGHT:
                        if (positionShip.x < 735)
                        {
                            positionShip.x += 5;
                        }
                        break;
                    case SDLK_LEFT:
                        if (positionShip.x > 0)
                        {
                            positionShip.x -= 5;
                        }
                        break;
                    case SDLK_SPACE:
                        shot = IMG_Load("images/shot.png");
                        positionShot.x = positionShip.x;
                        positionShot.y = positionShip.y;
                        while (positionShot.y < 800) {
                            positionShot.y += 5;
                        }
                        break;
                }
                break;
        }
        SDL_BlitSurface(background, NULL, screen, &positionBackground);
        SDL_BlitSurface(ship, NULL, screen, &positionShip);
        SDL_BlitSurface(shot, NULL, screen, &positionShip);
        SDL_Flip(screen);
    }

  SDL_FreeSurface(background);
  SDL_FreeSurface(ship);
  SDL_FreeSurface(shot);
  SDL_Quit();

  return EXIT_SUCCESS;
}

/*void pause()
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
                break;
            case SDL_KEYDOWN:
                switch(event.key.keysym.sym)
                {
                    case SDLK_ESCAPE:
                        continuer = 0;
                        break;
                }
                break;
        }
    }
}*/
