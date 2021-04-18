#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <SDL2/SDL_render.h>
#include <stdio.h>

bool init();
SDL_Texture* load_texture(const char*, SDL_Renderer*);

class WindowHandler
{
private:
	SDL_Window *window;
	SDL_Renderer *renderer;

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

		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
		if (renderer == NULL)
		{
		    printf("Failed to create renderer: %s", SDL_GetError());
			return;
		}

		SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);

		SDL_RenderClear(renderer);
	}

	SDL_Surface* getSurface()
	{
		return SDL_GetWindowSurface(window);
	}

	SDL_Texture* RenderTexture(const char *filepath)
	{
		SDL_Texture *texture = load_texture(filepath, renderer);

		SDL_RenderCopy(renderer, texture, NULL, NULL);

		SDL_RenderPresent(renderer);

		return texture;
	}

	~WindowHandler()
	{
		printf("Deleting window...\n");

		SDL_DestroyWindow(window);
		SDL_DestroyRenderer(renderer);
	}
};

int main()
{
	if(!init())
		return -1;

	WindowHandler *wh = new WindowHandler(300, 300);
	SDL_Texture *tex = wh->RenderTexture("images/background.png");
	
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
	SDL_DestroyTexture(tex);

	IMG_Quit();
	SDL_Quit();
	
	return 0;
}


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

SDL_Texture* load_texture(const char *filepath, SDL_Renderer *renderer)
{
	SDL_Texture *texture = NULL;
	SDL_Surface *surface = IMG_Load(filepath);

	if(surface == NULL)
	{
		printf("Error creating surface: %s", SDL_GetError());
	}
	else
	{
		texture = SDL_CreateTextureFromSurface(renderer, surface);

		if (texture == NULL)
		{
		    printf("Error optimizing: %s", SDL_GetError());
		}
		else
		{
			free(surface);
			return texture;
		}
	}

	return NULL;
}
