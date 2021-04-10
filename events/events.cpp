#include <SDL2/SDL.h>

#include <stdio.h>

class WindowHandler
{
private:	
	SDL_Window *window;
	SDL_Surface *surface;

public:
	WindowHandler(int w, int h)
	{
		printf("Initialising window...\n");
		window = SDL_CreateWindow("Event handled", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, w, h, SDL_WINDOW_SHOWN);

		if(window == NULL)
		{
			printf("Error creating window: %s", SDL_GetError());
			return;
		}
		else
		{
			surface = SDL_GetWindowSurface(window);
		}
	}

	bool blit_new_surface(SDL_Surface *blit_surface)
	{
		SDL_BlitSurface(blit_surface, NULL, surface, NULL);

		SDL_UpdateWindowSurface(window);
	}	

	~WindowHandler()
	{
		printf("Deleting window...\n");

		SDL_DestroyWindow(window);
	}
};

int main()
{
	if (SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf("Error in sdl init: %s", SDL_GetError());
		return -1;
	}

	WindowHandler *wh = new WindowHandler(700, 700);

	SDL_Surface *background = SDL_LoadBMP("background.bmp");
	wh->blit_new_surface(background);

		
	bool quit_app = false;
	SDL_Event e;

	while (!quit_app)
	{
		while (SDL_PollEvent(&e))
		{
		    if (e.type == SDL_QUIT)
			{
			    quit_app = true;
			}
		}
	}
	
	delete wh;
	SDL_FreeSurface(background);
	SDL_Quit();
	
	return 0;
}
