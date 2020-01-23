#include "Palette.h"

Palette Palette::border;
Palette Palette::fill;


void Palette::Init(int _x, int _y, bool need_alfa)
{
	x = _x, y = _y;
	button_w = 30, button_h = 30;
	panel_w = 64, panel_h = 90;
	_alfa = need_alfa;
	colors = 3;
	r = 0, g = 0, b = 0, a = 255;
	if (need_alfa)
	{
		panel_h += 30;
		colors++;
		a = 0;
	}
	
	Slider* tmp;
	for (int i = 0; i < colors; i++)
	{
		tmp = new Slider;
		tmp->Init(10, y + button_h + 30 * i, panel_w - 10, 30);
		s_colors.push_back(tmp);
	}
	_state = CLOSED;
}

void Palette::Handle(SDL_Event* event)
{
	if (MouseOver(event->button.x, event->button.y))
	{
		if (event->type == SDL_MOUSEBUTTONDOWN && event->button.button == SDL_BUTTON_LEFT)
			_state = !_state;
	}
	if (_state && MouseOverPanel(event->button.x, event->button.y))
	{
		for (int i = 0; i < colors; i++)
		{
			s_colors[i]->Handle(event);
		}
	}

	r = s_colors[0]->GetData() * 255;
	g = s_colors[1]->GetData() * 255;
	b = s_colors[2]->GetData() * 255;
	if(_alfa)
		a = s_colors[3]->GetData() * 255;

}

void Palette::Render()
{
	SDL_Rect dst = { x, y, button_w, button_h };
	SDL_SetRenderDrawColor(DRAW::render, 0x50, 0x50, 0x50, 255);
	SDL_RenderFillRect(DRAW::render, &dst);
	if (a > 0)
	{
		dst = { x + 5, y + 5, button_w - 10, button_h - 10 };
		SDL_SetRenderDrawColor(DRAW::render, r, g, b, a);
		SDL_RenderFillRect(DRAW::render, &dst);
	}
	else
	{
		dst = { x + 5, y + 5, button_w - 10, button_h - 10 };
		SDL_SetRenderDrawColor(DRAW::render, 255, 255, 255, 255);
		SDL_RenderFillRect(DRAW::render, &dst);
		SDL_SetRenderDrawColor(DRAW::render, 255, 0, 0, 255);
		SDL_RenderDrawLine(DRAW::render, x + 5, y + 5, x + button_w - 6, y + button_h - 6);
		SDL_RenderDrawLine(DRAW::render, x + 6, y + 5, x + button_w - 6, y + button_h - 7);
		SDL_RenderDrawLine(DRAW::render, x + 5, y + 6, x + button_w - 7, y + button_h - 6);
	}

	if (_state == OPENED)
	{
		dst = { 5, y + button_h, panel_w, panel_h };
		SDL_SetRenderDrawColor(DRAW::render, 0x50, 0x50, 0x50, 255);
		SDL_RenderFillRect(DRAW::render, &dst);
		SDL_SetRenderDrawColor(DRAW::render, 0x0, 0x0, 0x0, 255);
		SDL_RenderDrawRect(DRAW::render, &dst);
		for (int i = 0; i < colors; i++)
		{
			s_colors[i]->Render();
		}
	}
}

tuple<int, int, int, int> Palette::GetColor()
{
	return make_tuple(r, g, b, a);
}

void Palette::SetColor(int r, int g, int b, int a)
{
	s_colors[0]->SetData((double)r / 255);
	s_colors[1]->SetData((double)g / 255);
	s_colors[2]->SetData((double)b / 255);
	if (_alfa)
		s_colors[3]->SetData((double)a / 255);
}