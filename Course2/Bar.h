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
	

	std::vector<Button*> v_Buttons;
	
public:
	static int	_active;

	Bar() {}
	
	int		Init(SDL_Renderer* render, const char* path, int x, int y, int num, int pos, int padding);
	void	Render(SDL_Renderer* render);
	void	Handle(SDL_Event* event);
	void	CleanUp();
	void	Activate0() { v_Buttons[_active]->Release(); v_Buttons[_active]->UnFocus(); v_Buttons[0]->Press(); _active = 0; }
	void	Unfocus() { if(_focused >= 0 && _focused != _active) v_Buttons[_focused]->UnFocus(); _focused = -1; }
	bool	MouseOver(int mx, int my) { return (pos.x <= mx && mx < pos.x + pos.w && pos.y <= my
												&& my < pos.y + pos.h); }


};

