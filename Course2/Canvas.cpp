#include "Canvas.h"

vector<Dot*> Canvas::v_elements;

int Canvas::_current = -1;

SDL_Rect Canvas::canvas;

int Canvas::Init(SDL_Renderer* render)
{
	v_elements.clear();

	int w, h;
	SDL_GetRendererOutputSize(render, &w, &h);

	whole_off = { w - 121, 0, 120, h };
	slider_off = { w - 12, 0, 10, 20 };

	return 0;
}

int Canvas::Handle(SDL_Event* event)
{
	return 0;
}

void Canvas::Render(SDL_Renderer* render)
{
	SDL_SetRenderDrawColor(render, 0xff, 0xff, 0xff, 0xff);
	SDL_RenderFillRect(render, &Canvas::canvas);
}

void Canvas::Clear() 
{
	_current = -1;
	for (int i = 0; i < v_elements.size(); i ++)
	{
		v_elements[i]->Clear();
		delete v_elements[i];
	}
	v_elements.clear();
}
