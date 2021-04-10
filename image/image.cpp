#include <SDL2/SDL.h>

#include <stdio.h>

SDL_Window *SCREEN = NULL;
SDL_Surface *SCREEN_SURFACE = NULL;
SDL_Surface *HELLO_IMAGE = NULL;

bool init_screen(int, int);
bool load_media();
void finalize();

int main()
{
	if(!init_screen(500, 300))
	{
		printf("Exiting...");
		return -1;
	}
	if(!load_media())
	{
		printf("Exiting...");
		return -1;
	}
	else
	{
	    SDL_BlitSurface( HELLO_IMAGE, NULL, SCREEN_SURFACE, NULL);
		SDL_UpdateWindowSurface(SCREEN);

		SDL_Delay(2000);
	}

	finalize();
	
	return 0;
}

bool init_screen(int screen_width, int screen_height)
{
	if (SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf("Error in sdl init: %s", SDL_GetError());
		return false;
	}
	else
	{
	    SCREEN = SDL_CreateWindow("Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screen_width, screen_height, SDL_WINDOW_SHOWN);

		if (SCREEN == NULL)
		{
		    printf("Error in creating screen: %s", SDL_GetError());
			return false;
		}
		else
		{
		    SCREEN_SURFACE = SDL_GetWindowSurface(SCREEN);
		}
	}

	return true;
}

bool load_media()
{
	HELLO_IMAGE = SDL_LoadBMP("image/blue.bmp");

	if(HELLO_IMAGE == NULL)
	{
		printf("Error loading image: %s", SDL_GetError());
		return false;
	}

	return true;
}

void finalize()
{
    SDL_FreeSurface( HELLO_IMAGE );
    HELLO_IMAGE = NULL;

    SDL_DestroyWindow( SCREEN );
	SCREEN = NULL;

    SDL_Quit();
}
