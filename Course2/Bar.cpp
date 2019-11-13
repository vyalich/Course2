#include "Bar.h"

int Bar::Init(SDL_Renderer* render, const char* path, int n_x, int n_y, int num, int n_orient, int padding)
{
	_focused =-1;
	_active = 0;
	pos.x = n_x; pos.y = n_y;
	size = num;
	std::string src = path;
	Button* tmp;
	int dx = n_x+padding, dy = n_y+padding;
	int W, H;
	int n = src.find("*");
	for (int i = 0; i < size; i++)
	{
		src[n] = i+49;
		SDL_Log("%s", src.c_str());
		tmp = new Button;
		if (tmp->Init(render, src.c_str(), dx, dy) < 0)
			return -1;
		tmp->GetSize(&W, &H);
		if (n_orient == VERTICAL)
			dy += H + padding;
		else if (n_orient == HORIZONTAL)
			dx += W + padding;
		v_Buttons.push_back(tmp);
	}
	if (n_orient == VERTICAL)
		dy -= (H + padding);
	else if (n_orient == HORIZONTAL)
		dx -= (W + padding);
	pos.w = dx + W - pos.x + padding;
	pos.h = dy + H - pos.y + padding;

	v_Buttons[0]->Press();
	v_Buttons[0]->Release();

	return 0;
}

void Bar::Render(SDL_Renderer* render)
{
	SDL_SetRenderDrawColor(render, 0x0, 0x0, 0x0, 0xf0);
	SDL_RenderFillRect(render, &pos);

	for (int i = 0; i < size; i++)
	{
		v_Buttons[i]->Render(render);
	}
}

void Bar::Handle(SDL_Event* event)
{
	int mx, my;
	int index;
	SDL_GetMouseState(&mx, &my);
	if (orient == VERTICAL)
		index = (my - pos.y-1) * size / pos.h;
	else if (orient == HORIZONTAL)
		index = (mx - pos.w-1) * size / pos.w;

	switch (event->type)
	{
	case SDL_MOUSEMOTION:
		if (v_Buttons[index]->MouseOver(mx, my))
		{
			if (index != _focused && index != _active)
			{
				if (_focused >= 0 && _focused != _active)
					v_Buttons[_focused]->UnFocus();
				v_Buttons[index]->Focus();
				_focused = index;
			}
		}
		else
		{
			if (_focused != _active && _focused >= 0)
			{
				SDL_Log("UnFocus: %d, active: %d", _focused, _active);
				v_Buttons[_focused]->UnFocus();
				_focused = -1;
			}
		}
		break;
	case SDL_MOUSEBUTTONDOWN:
		if (event->button.button == SDL_BUTTON_LEFT)
		{
			if (_focused >= 0)
			{
				v_Buttons[index]->Press();
				SDL_Log("%d pressed", index);
			}
		}
		break;
	case SDL_MOUSEBUTTONUP:
		if (event->button.button == SDL_BUTTON_LEFT)
		{
			SDL_Log("focus: %d", _focused);

			if (_focused >= 0 && v_Buttons[_focused]->Release())
			{
				//v_Buttons[_focused]->Release();
				v_Buttons[_active]->UnFocus();
				_active = _focused;
				SDL_Log("%d released", index);
				SDL_Log("active: %d", _active);
			}
		}
		break;
	}
}