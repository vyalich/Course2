#pragma once

#include <SDL.h>
#include "Slider.h"
#include <tuple>
#include "Enums.h"
#include <vector>

using namespace std;

class Palette
{
private:
	int x, y;
	int button_w, button_h;
	int panel_w, panel_h;
	bool	_alfa;
	Uint8	r, g, b, a;
	vector<Slider*>  s_colors;
	int colors;
	char _state;

public:

	static Palette	border;

	static Palette	fill;

	void Init(int x, int y, bool need_alfa);

	void Handle(SDL_Event* event);

	void Render();

	tuple<int, int, int, int> GetColor();

	void SetColor(int r, int g, int b, int a);

	bool MouseOver(int _x, int _y) { return (_x >= x && _x <= x + button_w && _y >= y && _y <= y + button_h); }

	bool MouseOverPanel(int _x, int _y) { return (_x >= 4 && _x <= 86 && _y >= y + button_h && _y <= y + button_h + panel_h); }

	bool IsOpened() { return _state; }

	void Close() { _state = CLOSED; }

};

