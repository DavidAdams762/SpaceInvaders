#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>


void interface(SDL_Surface *screen) {
    SDL_Surface *background[2];
    SDL_Surface *ship = NULL;
    SDL_Surface *shipshoot = NULL;
    SDL_Surface *tmp = NULL;
    SDL_Surface *shot = NULL;
    SDL_Surface *ennemy = NULL;
    SDL_Rect positionBackground;
    SDL_Rect positionShip;
    SDL_Rect positionShot;

    SDL_Rect *positionEnnemies;
    int continuer = 1;
    SDL_Event event;

    positionBackground.x = 0;
    positionBackground.y = 0;
    positionShip.x = (800 / 2) - 32;
    positionShip.y = 600 / 1.2;

    background[0] = IMG_Load("images/bg-1.png");
    background[1] = IMG_Load("images/bg-2.png");
    ship = IMG_Load("images/ship.png");
    tmp = ship;

    shipshoot = IMG_Load("images/shipshoot.png");
    ennemy = IMG_Load("images/enemies.png");

    SDL_BlitSurface(ship, NULL, screen, &positionShip);
    SDL_Flip(screen);

    SDL_EnableKeyRepeat(10, 10);

	while (continuer)
	{
	  Uint32 intervalBG = SDL_GetTicks();
	  SDL_BlitSurface(background[intervalBG%2], NULL, screen, &positionBackground);
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
	                    tmp = ship;
	                    ship = shipshoot;
	                    shot = IMG_Load("images/shot.png");
	                    positionShot.x = positionShip.x;
	                    positionShot.y = positionShip.y;
	                    while (positionShot.y < 800) {
	                        positionShot.y += 5;
	                    }
	                    break;
	                default:
	                    break;
	            }
	            break;
	    }
	        SDL_BlitSurface(ship, NULL, screen, &positionShip);
	        ship = tmp;
	        SDL_BlitSurface(shot, NULL, screen, &positionShip);
	        SDL_Flip(screen);
	    }


  SDL_FreeSurface(*background);
  SDL_FreeSurface(ship);
  SDL_FreeSurface(shot);                     //


}

int main(int argc, char *argv[])
{
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Surface *screen;
    screen = SDL_SetVideoMode(800, 600, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
    interface(screen);
    SDL_Quit();
  /*
  int xTest = 20;
  for (int i = 0; i <= 16; i++) {
	positionEnnemies[i].x = xTest;
	positionEnnemies[i].y = 0;
	if (i > 7 && positionEnnemies[i].x > 800) {
	  if (i == 8) {
		positionEnnemies[i].x = 20;
	  }
	  else {
		positionEnnemies[i].x = positionEnnemies[i-1].x + 100;
	  }
	  positionEnnemies[i].y += 100;
	}
	SDL_BlitSurface(ennemy, NULL, screen, &positionEnnemies[i]);
	xTest += 100;
  }
  */


  /* Position while */
       return EXIT_SUCCESS;
}
