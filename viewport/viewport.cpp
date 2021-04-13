#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <SDL2/SDL_render.h>
#include <stdio.h>

typedef struct
{
	int r;
	int g;
	int b;
	int a;
} color;
typedef struct
{
	int x1;
	int y1;
	int x2;
	int y2;
} line;

bool init();
SDL_Texture* load_texture(const char*, SDL_Renderer*);

class WindowHandler
{
private:
	SDL_Window *window;
	SDL_Renderer *renderer;

	color empty_screen;

	int screen_height;
	int screen_width;

public:
	WindowHandler(int w, int h)
	{
		printf("Initialising window...\n");
		window = SDL_CreateWindow("Event handled", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, w, h, SDL_WINDOW_SHOWN);
		empty_screen = {0, 0, 0, 0};		
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

		SDL_SetRenderDrawColor(renderer, empty_screen.r, empty_screen.g, empty_screen.b, empty_screen.a);

		SDL_RenderClear(renderer);
	}

	SDL_Surface* getSurface()
	{
		return SDL_GetWindowSurface(window);
	}

	void UpdateRenderedBuffer()
	{
		SDL_RenderPresent(renderer);
	}

	SDL_Texture* RenderTexture(const char *filepath, SDL_Rect *viewport)
	{
		SDL_Texture *texture = load_texture(filepath, renderer);

		SDL_RenderSetViewport(renderer, viewport);
		SDL_RenderCopy(renderer, texture, NULL, NULL);

		SDL_Rect default_view = {0, 0, 500, 500};
		SDL_RenderSetViewport(renderer, &default_view);
		return texture;
	}

	void DrawLines(line *lines, int n, color c)
	{
		SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a);

		for (int i=0; i<n; i++)
		{
		    SDL_RenderDrawLine(renderer, lines[i].x1, lines[i].y1, lines[i].x2, lines[i].y2);
		}

		SDL_SetRenderDrawColor(renderer, empty_screen.r, empty_screen.g, empty_screen.b, empty_screen.a);
	}

	void DrawRectangle(SDL_Rect *rect, color c, bool solid)
	{
		SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a);

		if(solid)
			SDL_RenderFillRect(renderer, rect);
		else 
		    SDL_RenderDrawRect(renderer, rect);

		SDL_SetRenderDrawColor(renderer, empty_screen.a, empty_screen.g, empty_screen.b, empty_screen.a);
	}

	~WindowHandler()
	{
		printf("Deleting window...\n");

		SDL_DestroyWindow(window);
		SDL_DestroyRenderer(renderer);
	}
};

void free_texture(SDL_Texture **textures)
{
	for (int i=0; i<4; i++)
	{
	    SDL_DestroyTexture(textures[i]);
	}
}

int main()
{
	if(!init())
		return -1;

	SDL_Rect rect1 = {0, 0, 100, 100};
	SDL_Rect rect2 = {100, 0, 100, 100};
	SDL_Rect rect3 = {0, 100, 100, 100};
	SDL_Rect rect4 = {100, 100, 100, 100};
	
	WindowHandler *wh = new WindowHandler(200, 200);

	SDL_Texture *textures[4];
	textures[0] = wh->RenderTexture("viewport/red.png", &rect1);
	textures[1] = wh->RenderTexture("viewport/green.png", &rect2);
	textures[2] = wh->RenderTexture("viewport/blue.png", &rect3);
	textures[3] = wh->RenderTexture("viewport/pink.png", &rect4);

	wh->UpdateRenderedBuffer();
		
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
	free_texture(textures);

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
