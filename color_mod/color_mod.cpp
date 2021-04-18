#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <stdio.h>

typedef struct
{
	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char a;
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

class Texture
{
private:
	SDL_Texture *texture;

	int height;
	int width;

public:
	Texture()
	{
		texture = NULL;
		height = 0;
		width = 0;
	}

	int get_width()
	{
		return width;
	}
	int get_height()
	{
		return height;
	}
	SDL_Texture* get_texture()
	{
		return texture;
	}

	void ColorModulation(color c)
	{
		SDL_SetTextureColorMod(texture, c.r, c.g, c.b);
	}

// Doing it like this so we can reassign new texture to the object if needed
	bool LoadFromFile(const char *filepath, SDL_Renderer *renderer)
	{
		if(texture != NULL)
			FreeTexture();

		SDL_Surface *surface = IMG_Load(filepath);
		if(surface == NULL)
		{
			printf("Failed to load image: %s", IMG_GetError());
			return false;
		}

		SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, 0, 0xFF, 0xFF));

		texture = SDL_CreateTextureFromSurface(renderer, surface);
		if(texture == NULL)
		{
			printf("Failed to create texture: %s", SDL_GetError());
			return false;
		}
		width = surface->w;
		height = surface->h;

		SDL_FreeSurface(surface);

		return true;
	}
	


	void FreeTexture()
	{
		if(texture == NULL)
		{
			SDL_DestroyTexture(texture);
			texture = NULL;
			width = 0;
			height = 0;
		}
	}

	~Texture()
	{
		FreeTexture();
	}
};

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

	void render(Texture *texture, int x, int y, SDL_Rect *clip, color c)
	{
		SDL_Rect render_quad = {x, y, texture->get_width(), texture->get_height()};

		if(clip)
		{
			render_quad.w = clip->w;
			render_quad.h = clip->h;
		}

		texture->ColorModulation(c);
		SDL_RenderCopy(renderer, texture->get_texture(), clip, &render_quad);
	}

	Texture* RenderSpritesheet(const char *filepath, int *x, int *y, SDL_Rect *clip, int n, color c)
	{
		Texture *spritesheet = new Texture();
		
		if(!spritesheet->LoadFromFile(filepath, renderer))
		{
			return NULL;
		}
		if(clip)
		{
			for (int i=0; i<n; i++)
			{
				render(spritesheet, x[i], y[i], clip+i, c);
			}
		}
		else
		{
			render(spritesheet, *x, *y, NULL, c);
		}

		return spritesheet;
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

void free_texture(Texture **textures, int n)
{
	for (int i=0; i<n; i++)
	{
	    delete textures[i];
	}
}

int main()
{
	if(!init())
		return -1;

	int x=0, y=0;
	WindowHandler *wh = new WindowHandler(700, 600);

	Texture *blue = wh->RenderSpritesheet("images/square.png", &x, &y, NULL, 0, {255,128,255});

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
	delete blue;

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
