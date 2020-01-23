#pragma once

#include <SDL.h>
#include "DRAW.h"
#include "Enums.h"

class Slider
{
private:
	int x, y;
	int w, h;
	int s_x, s_y;
	int s_w, s_h;
	char	_state;

public:

	void Init(int x, int y, int w, int h);

	void Handle(SDL_Event* event);

	void Render();

	bool MouseOver(int _x, int _y) { return (_x >= x + s_x && _x <= x + s_x + s_w && _y >= s_y && _y <= s_y + s_h); }

	double GetData() { return (double)s_x / (w - s_w); }

	void SetData(double n) { s_x = (w - s_w) * n; }
};

