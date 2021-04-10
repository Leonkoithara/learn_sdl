#include <SDL2/SDL.h>

#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_surface.h>
#include <stdio.h>
#include <iostream>

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

SDL_Surface* load_media(std::string filepath)
{
	SDL_Surface *surface = SDL_LoadBMP(filepath.c_str());

	if(surface == NULL)
	{
		printf("Error creating surface: %s", SDL_GetError());
	}

	return surface;
}

void free_surfaces(SDL_Surface **surfaces)
{
	for (int i=0; i<4; i++)
	{
	    free(surfaces[i]);
	}
}

int main()
{
	if (SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf("Error in sdl init: %s", SDL_GetError());
		return -1;
	}

	enum character {RED, BLUE, GREEN, PINK};
	character c;
	SDL_Surface *surfaces[4];
	
	surfaces[0] = load_media("red.bmp");
	surfaces[1] = load_media("blue.bmp");
	surfaces[2] = load_media("green.bmp");
	surfaces[3] = load_media("pink.bmp");	
	
	WindowHandler *wh = new WindowHandler(300, 300);
	wh->blit_new_surface(surfaces[0]);
	
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
			else if( e.type == SDL_KEYDOWN)
			{
				switch ( e.key.keysym.sym )
				{
				case SDLK_UP:
					c = RED;
					break;
				case SDLK_DOWN:
					c = BLUE;
					break;
				case SDLK_LEFT:
					c = GREEN;
					break;
				case SDLK_RIGHT:
					c = PINK;
					break;
				default:
					c = BLUE;
				}

				wh->blit_new_surface(surfaces[c]);
			}
		}
	}
	
	delete wh;

	free_surfaces(surfaces);
	
	SDL_Quit();
	
	return 0;
}
