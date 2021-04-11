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

	SDL_Texture* RenderTexture(const char *filepath)
	{
		SDL_Texture *texture = load_texture(filepath, renderer);

		SDL_RenderCopy(renderer, texture, NULL, NULL);

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

int main()
{
	if(!init())
		return -1;
	
	color c1 = {255, 0, 0, 255};
	SDL_Rect rect1 = {125, 125, 250, 250};
	color c2 = {0, 255, 0, 255};
	SDL_Rect rect2 = {85, 85, 320, 320};

	color cl = {0, 0, 255, 255};
	line lines[2];
	lines[0] = {250, 0, 250, 500};
	lines[1] = {0, 250, 500, 250};
	
	WindowHandler *wh = new WindowHandler(500, 500);
	wh->DrawRectangle(&rect1, c1, true);
	wh->DrawRectangle(&rect2, c2, false);
	wh->DrawLines(lines, 2, cl);

	wh->UpdateRenderedBuffer();


	/* Draw a triangle
	   line lines[3];
	   lines[0] = {150, 20, 20, 280};
	   lines[1] = {20, 280, 280, 280};
	   lines[2] = {280, 280, 150, 20};
	   wh->DrawLines(lines, 3, c);
	*/        
		
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
