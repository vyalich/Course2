#include "Dot.h"
#include "Canvas.h"

void Dot::Init(int _x, int _y)
{
	ID = 0;
	_selected = true;
	_creating = true;
	x = _x, y = _y;
	b_r = 0x0; b_g = 0x0; b_b = 0x0;
	int n;
	tie(b_r, b_g, b_b, n) = Palette::border.GetColor();
	title = u8"точка";
}

void Dot::Render(SDL_Renderer* render)
{
	SDL_SetRenderDrawColor(DRAW::render, b_r, b_g, b_b, 0xff);
	SDL_RenderDrawPoint(DRAW::render, x, y);
	SDL_RenderDrawPoint(DRAW::render, x, y-1);
	SDL_RenderDrawPoint(DRAW::render, x, y+1);
	SDL_RenderDrawPoint(DRAW::render, x+1, y);
	SDL_RenderDrawPoint(DRAW::render, x-1, y);

	if (_selected)
	{
		SDL_SetRenderDrawColor(DRAW::render, 0, 0, 0, 0xff);
		SDL_Rect frame = { x - 3, y - 3, 7, 7 };
		SDL_RenderDrawRect(DRAW::render, &frame);
	}
}

void Dot::Handle(SDL_Event* event)
{
	int n;
	tie(b_r, b_g, b_b, n) = Palette::border.GetColor();

	if (event->type == SDL_MOUSEMOTION && event->button.x >= Canvas::canvas.x && event->button.x <= Canvas::canvas.x + Canvas::canvas.w &&
		event->button.y >= Canvas::canvas.y && event->button.y <= Canvas::canvas.y + Canvas::canvas.h)
	{
		x += event->motion.xrel;
		y += event->motion.yrel;
	}
}

void Dot::Write(ofstream& out)
{
	out.write((char*)& ID, sizeof(ID));
	out.write((char*)& x, sizeof(x));
	out.write((char*)& y, sizeof(y));
	out.write((char*)& b_r, sizeof(b_r));
	out.write((char*)& b_g, sizeof(b_g));
	out.write((char*)& b_b, sizeof(b_b));
}

void Dot::Read(ifstream& in)
{
	ID = 0;
	title = u8"точка";
	_selected = false;
	in.read((char*)& x, sizeof(x));
	in.read((char*)& y, sizeof(y));
	in.read((char*)& b_r, sizeof(b_r));
	in.read((char*)& b_g, sizeof(b_g));
	in.read((char*)& b_b, sizeof(b_b));
}

void Dot::WriteSVG(ofstream& out)
{
	out << "<circle cx=\"" << x << "\" cy=\"" << y << "\" r=\"" << 1 << "\" ";
	out << "fill=\"rgb(" << (int)b_r << ", " << (int)b_g << ", " << (int)b_b << ")\"/>\n";
}