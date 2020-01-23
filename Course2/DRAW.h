#pragma once

#include <SDL.h>
#include <SDL_ttf.h>
#include <string>

using namespace std;

class DRAW
{
public:

	static SDL_Renderer* render;

	static SDL_Texture* CreateTexture(int w, int h);

	static void DrawText(SDL_Renderer* Render, TTF_Font* Font, const char* title, int X, int Y, int W, int H, SDL_Color* color, SDL_Color* bg_color);
};

