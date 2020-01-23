#include "Line.h"

void Line::Init(int _x, int _y)
{
	ID = 1;
	x = x2 = _x, y = y2 = _y;
	b_r = 0x0; b_g = 0x0; b_b = 0x0;
	title = u8"линия";
}

void Line::Render(SDL_Renderer* render)
{
	
	SDL_SetRenderDrawColor(render, b_r, b_g, b_b, 0xff);
	SDL_RenderDrawLineF(render, x, y, x2, y2);
	if (_selected)
	{
		SDL_SetRenderDrawColor(DRAW::render, 0, 0, 0, 0xff);
		SDL_Rect frame = { x - 2, y - 2, 5, 5 };
		SDL_RenderFillRect(DRAW::render, &frame);
		frame = { (int)x2 - 2, (int)y2 - 2, 5, 5 };
		SDL_RenderFillRect(DRAW::render, &frame);
	}
}

void Line::Handle(SDL_Event* event)
{
	if (_creating && event->type == SDL_MOUSEMOTION)
	{
		x2 += event->motion.xrel;
		y2 += event->motion.yrel;

		int n;
		tie(b_r, b_g, b_b, n) = Palette::border.GetColor();
		return;
	}

	int n;
	tie(b_r, b_g, b_b, n) = Palette::border.GetColor();

	switch (event->type)
	{
	case SDL_MOUSEBUTTONDOWN:
		//SDL_Log("-");
		_state = DRAG;
		if (abs(event->button.x - Canvas::canvas.x - x) <= 2 && abs(event->button.y - Canvas::canvas.y - y) <= 2)
		{
			//SDL_Log("<-");
			_state = DRAG_PN1;
		}
		else if (abs(event->button.x - Canvas::canvas.x - x2) <= 2 && abs(event->button.y - Canvas::canvas.y - y2) <= 2)
		{
			//SDL_Log("->");
			_state = DRAG_PN2;
		}
		break;
	case SDL_MOUSEBUTTONUP:
		//SDL_Log("><");
		_state = NONE;
		break;
	case SDL_MOUSEMOTION:
		switch (_state) {
		case DRAG:
			x += event->motion.xrel;
			y += event->motion.yrel;
			x2 += event->motion.xrel;
			y2 += event->motion.yrel;
			break;
		case DRAG_PN1:
			x += event->motion.xrel;
			y += event->motion.yrel;
			break;
		case DRAG_PN2:
			x2 += event->motion.xrel;
			y2 += event->motion.yrel;
			break;
		}
		break;
	}
	
}

bool  Line::MouseOver(int _x, int _y)
{
	double s = (x2 - x) * (_y - y) - (_x - x) * (y2 - y);
	double ab = sqrt((x2 - x) * (x2 - x) + (y2 - y) * (y2 - y));
	return abs(s / ab) < 3;
}

void Line::Write(ofstream& out)
{
	Dot::Write(out);
	out.write((char*)& x2, sizeof(x2));
	out.write((char*)& y2, sizeof(y2));
}

void Line::Read(ifstream& in)
{
	Dot::Read(in);
	ID = 1;
	title = u8"линия";
	in.read((char*)& x2, sizeof(x2));
	in.read((char*)& y2, sizeof(y2));
}

void Line::WriteSVG(ofstream& out)
{
	out << "<line x1=\"" << x << "\" y1=\"" << y << "\" x2=\"" << x2 << "\" y2=\"" << y2 <<"\"";
	out << " stroke=\"rgb(" << (int)b_r << ", " << (int)b_g << ", " << (int)b_b << ")\"/>\n";
}