#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include "Button.h"
#include "Bar.h"

class Window {
private:
	SDL_Window*			window;
	SDL_Renderer*		render;
	TTF_Font*			font;
	Bar					m_ShapeBar;
	int					winW, winH;

public:
	Window() {}
	~Window() {}
	
	int		Init();
	int		Handle(SDL_Event*);
	void	Render();
	void	CleanUp();
};

