#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <stdio.h>

class WindowHandler
{
private:
	SDL_Window *window;
	SDL_Surface *surface;

	int screen_height;
	int screen_width;

public:
	WindowHandler(int w, int h)
	{
		printf("Initialising window...\n");
		window = SDL_CreateWindow("Event handled", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, w, h, SDL_WINDOW_SHOWN);
		screen_height = h;
		screen_width = w;

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

	SDL_PixelFormat* getSurfaceFormat()
	{
		return surface->format;
	}

	bool blit_new_surface(SDL_Surface *blit_surface)
	{
		SDL_BlitSurface(blit_surface, NULL, surface, NULL);

		SDL_UpdateWindowSurface(window);
	}
	bool blit_stretched(SDL_Surface *blit_surface)
	{
		SDL_Rect rect;
		rect.x = 0;
		rect.y = 0;
		rect.h = screen_height;
		rect.w = screen_width;

		SDL_BlitScaled(blit_surface, NULL, surface, &rect);

		SDL_UpdateWindowSurface(window);
	}

	~WindowHandler()
	{
		printf("Deleting window...\n");

		SDL_DestroyWindow(window);
	}
};

bool init()
{
	if (SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf("Error in sdl init: %s", SDL_GetError());
		return false;
	}

	int flags = IMG_INIT_PNG;                   // bitwise Or all the flags you need
	int init_ = IMG_Init(flags);
	if (!(init_ & flags))
	{
		printf("SDL Image could not initialise: %s", IMG_GetError());
		return false;
	}

	return true;
}

SDL_Surface* load_bmpmedia(const char *filepath, SDL_PixelFormat *format)
{
	SDL_Surface *optimized = NULL;
	SDL_Surface *surface = SDL_LoadBMP(filepath);

	if(surface == NULL)
	{
		printf("Error creating surface: %s", SDL_GetError());
	}
	else
	{
		optimized = SDL_ConvertSurface(surface, format, 0);

		if (optimized == NULL)
		{
		    printf("Error optimizing: %s", SDL_GetError());
		}
		else
		{
			free(surface);
			return optimized;
		}
	}

	return surface;
}

SDL_Surface* load_imgmedia(const char *filepath, SDL_PixelFormat *format)
{
	SDL_Surface *optimized = NULL;
	SDL_Surface *surface = IMG_Load(filepath);

	if(surface == NULL)
	{
		printf("Error creating surface: %s", SDL_GetError());
	}
	else
	{
		optimized = SDL_ConvertSurface(surface, format, 0);

		if (optimized == NULL)
		{
		    printf("Error optimizing: %s", SDL_GetError());
		}
		else
		{
			free(surface);
			return optimized;
		}
	}

	return surface;
}

int main()
{
	if(!init())
		return -1;

	WindowHandler *wh = new WindowHandler(300, 300);
	SDL_Surface *background = load_imgmedia("images/background.png", wh->getSurfaceFormat());
	wh->blit_stretched(background);
	
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

	SDL_Quit();
	
	return 0;
}
