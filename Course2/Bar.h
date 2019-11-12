#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <vector>
#include <string>
#include "Button.h"
#include "Enums.h"

class Bar
{
private:
	SDL_Rect	pos;
	int			orient;
	int			size;
	int			_focused;
	int			_active;

	std::vector<Button*> v_Buttons;
	
public:
	Bar() {}
	
	int		Init(SDL_Renderer* render, const char* path, int x, int y, int num, int pos, int padding);
	void	Render(SDL_Renderer* render);
	void	Handle(SDL_Event* event);
	void	CleanUp();
	bool	MouseOver(int mx, int my) { return (pos.x <= mx && mx <= pos.x + pos.w && pos.y <= my
												&& my <= pos.y + pos.h); }


};

