/*This source code copyrighted by Lazy Foo' Productions (2004-2019)
and may not be redistributed without written permission.*/

//Using SDL and standard IO
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <string>
#include <iostream>

int main( int argc, char* args[] )
{
	SDL_Init(SDL_INIT_EVERYTHING);
	TTF_Init();
	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags)){
		std::cout << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError();
		return 10;
	}
	Mix_Init(MIX_INIT_MP3);

	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Texture* texture;
	SDL_Surface* surf;
	TTF_Font* font;

	window = SDL_CreateWindow("TEST", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);

	surf = IMG_Load("texture.png");
	if (surf == NULL){
		std::cout << "Failed to load texture image!";
		return 100;
	}
	texture = SDL_CreateTextureFromSurface(renderer,surf);
	SDL_FreeSurface(surf);

	bool quit = false;
	SDL_Event e;
	while (!quit){
		while (SDL_PollEvent(&e) != 0){
			if (e.type == SDL_QUIT){
				quit = true;
			}
		}
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, texture, NULL, NULL);
		SDL_RenderPresent(renderer);
	}

	SDL_DestroyTexture(texture);
	texture = NULL;
	
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	window = NULL;
	renderer = NULL;

	Mix_Quit();
	IMG_Quit();
	TTF_Quit();
	SDL_Quit();

	return 0;
}