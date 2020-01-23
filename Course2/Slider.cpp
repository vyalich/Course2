#include "Slider.h"

void Slider::Init(int _x, int _y, int _w, int _h)
{
	x = _x, y = _y, w = _w, h = _h;
	s_x = 0, s_y = y + h / 3;
	s_w =20, s_h = h / 3;
	
	_state = NONE;
}

void Slider::Handle(SDL_Event* event)
{
	switch (event->type)
	{
	case SDL_MOUSEBUTTONDOWN:
		if (MouseOver(event->button.x, event->button.y))
			_state = DRAG;
		break;
	case SDL_MOUSEBUTTONUP:
		_state = NONE;
		break;
	case SDL_MOUSEMOTION:
		if (_state == DRAG)
		{
			s_x = event->button.x - x - s_w/2;
			if (s_x < 0)
				s_x = 0;
			else if (s_x > w - s_w)
				s_x = w - s_w;
			//SDL_Log("s_x = %d  x+w = %d", s_x, w - 1 - s_w);
		}
		break;
	}
	
}

void Slider::Render()
{
	SDL_SetRenderDrawColor(DRAW::render, 0x34, 0x10, 0xf0, 0xff);
	SDL_RenderDrawLine(DRAW::render, x, y + h / 2 - 1, x + w - 1, y + h / 2 - 1);
	SDL_RenderDrawLine(DRAW::render, x, y + h / 2, x + w - 1, y + h / 2);
	SDL_RenderDrawLine(DRAW::render, x, y + h / 2 + 1, x + w - 1, y + h / 2 + 1);
	SDL_Rect dst = { x + s_x, s_y, s_w, s_h };
	SDL_RenderFillRect(DRAW::render, &dst);
}