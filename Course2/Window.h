#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <tuple>
#include <stdexcept>
#include <nfd.h>
#include <fstream>
#include "Button.h"
#include "Bar.h"
#include "Canvas.h"
#include "ellipse.h"
#include "Rect.h"
#include "Line.h"
#include "DRAW.h"
#include "Palette.h"

using namespace std;

class Window {
private:
	SDL_Window*			window;
	SDL_Renderer*		render;
	TTF_Font*			font;
	Bar					m_ShapeBar;
	vector<Button*>		file_control;
	Canvas				canvas;
	SDL_Rect			workspace;
	int					winW, winH;
	bool				mb_down;
	string				buf;

public:
	Window() {}
	~Window() {}
	

	int		Init();
	int		Handle(SDL_Event*);
	void	Render();
	void	CleanUp();
	bool	InWorkspace(int _x, int _y) { return (_x >= workspace.x && _x <= workspace.x + workspace.w - 1 && _y >= workspace.y && _y <= workspace.y + workspace.h - 1); }
	void	HandleFileControl(SDL_Event* event);
	void	Create();
	void	SaveMVG();
	void	LoadMVG();
	void    SaveSVG();
	void	InputText(const char* hint);
};

