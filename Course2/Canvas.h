#pragma once

#include <vector>
#include <SDL.h>
#include <SDL_ttf.h>
#include "Dot.h"


using namespace std;

class Canvas
{
public:
	static vector<Dot*> v_elements;

	static int _current;

	static SDL_Rect canvas;

	int Init(SDL_Renderer* render);

	void Resize(SDL_Renderer* render);

	int Handle(SDL_Event* event);

	void Render(SDL_Renderer* render);

	void Clear();
};

