#include "Rect.h"

void Rect::Init(int _x, int _y)
{
	ID = 2;
	x = _x, y = _y;
	w = h = 1;
	angle = 0;
	tex_w = tex_h = 256;
	texture = DRAW::CreateTexture(tex_w, tex_h);
	title = u8"прямоугольник";
	_selected = true;
	_creating = true;
}

void Rect::Render(SDL_Renderer* render)
{
	SDL_Rect src = { 0, 0, w + 6, h + 6 };
	SDL_Rect dst = { x - 3, y - 3, w + 6, h + 6 };
	SDL_RenderCopyEx(DRAW::render, texture, &src, &dst, angle, 0, SDL_FLIP_NONE);

	/*SDL_SetRenderDrawColor(DRAW::render, 0xb0, 0xb0, 0xb0, 255);
	SDL_Rect dst2 = { x , y, w , h };
	SDL_RenderDrawRect(DRAW::render, &dst2);

	SDL_SetRenderDrawColor(DRAW::render, 0xff, 0, 0, 255);
	SDL_RenderDrawLine(DRAW::render, x + w / 2, y + h / 2, X, Y);*/

	//SDL_SetRenderDrawColor(DRAW::render, 0, 0, 0xff, 255);
	//SDL_RenderDrawLine(DRAW::render, x + w / 2, y + h / 2, WX, WY);

}

void Rect::Handle(SDL_Event* event)
{
	if (_creating)
	{
		w += event->motion.xrel;
		if (w < 1)
			w = 1;
		h += event->motion.yrel;
		if (h < 1)
			h = 1;

		int n;
		tie(b_r, b_g, b_b, n) = Palette::border.GetColor();
		tie(f_r, f_g, f_b, f_a) = Palette::fill.GetColor();

		ResizeTexture();
		Redraw();

		return;
	}

	int n;
	tie(b_r, b_g, b_b, n) = Palette::border.GetColor();
	tie(f_r, f_g, f_b, f_a) = Palette::fill.GetColor();

	double radian = angle * M_PI / 180;
	double Vx = (double)event->button.x - Canvas::canvas.x - (x + w / 2);
	double Vy = (double)event->button.y - Canvas::canvas.y - (y + h / 2);

	double Rx = Vx * cos(radian) + Vy * sin(radian);
	double Ry = Vx * sin(radian) - Vy * cos(radian);

	//координаты курсора на прямоугольнике без поворота
	X = x + w / 2 + Rx;
	Y = y + h / 2 - Ry;

	//вектор скалирования
	//double Relx = (double)event->button.x - x - w / 2;
	//double Rely = (double)event->button.y - y - h / 2;
	
	double XREL;
	if (Rx > 0)
		XREL = Rx - w / 2;
	else 
		XREL = Rx + w / 2;

	double YREL;
	if (Ry < 0)
		YREL = -Ry - h / 2;
	else 
		YREL = -Ry + h / 2;

	switch (event->type)
	{
	case SDL_MOUSEBUTTONDOWN:
		pn = 0;
		if (abs(x - X) < 3)
		{
			pn += LEFT;
			_state = SCALE;
		}
		else if (abs(x + w - X) < 3)
		{
			pn += RIGHT;
			_state = SCALE;
		}
		if (abs(y - Y) < 3)
		{
			pn += TOP;
			_state = SCALE;
		}
		else if (abs(y + h - Y) < 3)
		{
			pn += BOT;
			_state = SCALE;
		}

		if ((X <= x - 3 || X >= x + w + 3) && (Y <= y - 3 || Y >= y + h + 3))
			_state = ROTATE;

		if (X > x + 3 && X < x + w - 4 && Y > y + 3 && Y < y + h - 4)
			_state = DRAG;
		break;
	case SDL_MOUSEBUTTONUP:
		_state = NONE;
		break;
	case SDL_MOUSEMOTION:
		switch (_state) {
		case DRAG:
			x += event->motion.xrel;
			y += event->motion.yrel;
			break;
		case SCALE:
			
			if (pn & LEFT)
			{
				
				x += XREL/2;
				w -= XREL;
				if (w < 1)
				{
					w += XREL;
					x -= XREL/2;
				}
			}
			else if (pn & RIGHT)
			{
				w += XREL;
				if (w < 1)
					w = 1;
			}
			if (pn & TOP)
			{
				y += YREL/2;
				h -= YREL;
				if (h < 1)
				{
					h += YREL;
					y -= YREL/2;
				}
			}
			else if (pn & BOT)
			{
				h += YREL;
				if (h < 1)
					h = 1;
			}
			//x = cx - w / 2;
			//y = cy - h / 2;
			break;
		case ROTATE:
		{
			double Ax = (event->button.x - Canvas::canvas.x + event->motion.xrel) - (x + w / 2);
			double Ay = (event->button.y - Canvas::canvas.y + event->motion.yrel) - (y + h / 2);
			double Bx = event->button.x - Canvas::canvas.x - (x + w / 2);
			double By = event->button.y - Canvas::canvas.y - (y + h / 2);
			double cosalfa = (Ax * Bx + Ay * By) / sqrt(Ax * Ax + Ay * Ay) / sqrt(Bx * Bx + By * By);
			double alfa = acos(cosalfa)*180/M_PI;
			double m_def = Ax * By - Bx * Ay;
			if(m_def > 0)
				angle -= alfa;
			else if(m_def < 0)
				angle += alfa;
			if (alfa < -360)
				alfa += 360;
			if (alfa > 360)
				alfa -= 360;
			break;
		}
		}
		break;
	}

	ResizeTexture();

	Redraw();
}

void Rect::Redraw()
{
	SDL_SetRenderTarget(DRAW::render, texture);
	SDL_SetRenderDrawColor(DRAW::render, 0, 0, 0, 0);
	SDL_RenderClear(DRAW::render);

	SDL_Rect dest = { 3, 3, w, h };
	SDL_SetRenderDrawColor(DRAW::render, b_r, b_g, b_b, 0xff);
	SDL_RenderDrawRect(DRAW::render, &dest);

	if (f_a)
	{
		dest = { 3+1, 3+1, (int)w-2, (int)h-2 };
		SDL_SetRenderDrawColor(DRAW::render, f_r, f_g, f_b, f_a);
		SDL_RenderFillRect(DRAW::render, &dest);
	}

	if (_selected)
		DrawUI();

	SDL_SetRenderTarget(DRAW::render, 0);
}

void Rect::Select()
{
	_selected = true;
	Palette::border.SetColor(b_r, b_g, b_b, 255);
	Palette::fill.SetColor(f_r, f_g, f_b, f_a);
	Redraw();
}

void Rect::UnSelect()
{
	_selected = false;
	Redraw();
}

void Rect::DrawUI()
{
	SDL_SetRenderDrawColor(DRAW::render, 0, 0, 0, 0xff);
	SDL_Rect dst = { 0, 0, 7, 7 };	SDL_RenderFillRect(DRAW::render, &dst);
	dst.y = h / 2;					SDL_RenderFillRect(DRAW::render, &dst);
	dst.y = h - 1;					SDL_RenderFillRect(DRAW::render, &dst);
	dst.x = w / 2;					SDL_RenderFillRect(DRAW::render, &dst);
	dst.y = 0;						SDL_RenderFillRect(DRAW::render, &dst);
	dst.x = w - 1;					SDL_RenderFillRect(DRAW::render, &dst);
	dst.y = h / 2;					SDL_RenderFillRect(DRAW::render, &dst);
	dst.y = h - 1;					SDL_RenderFillRect(DRAW::render, &dst);
}

void Rect::ResizeTexture()
{
	bool resize = false;

	if (w + 6 > tex_w)
	{
		tex_w *= 2;
		resize = true;
	}
	else if (w + 6 < tex_w / 2)
	{
		tex_w /= 2;
		resize = true;
	}

	if (h + 6 > tex_h)
	{
		tex_h *= 2;
		resize = true;
	}
	else if (h + 6 < tex_h / 2)
	{
		tex_h /= 2;
		resize = true;
	}

	if (resize)
	{
		SDL_DestroyTexture(texture);
		texture = DRAW::CreateTexture(tex_w, tex_h);
	}
}

bool Rect::MouseOver(int _x, int _y)
{
	double radian = angle * M_PI / 180;
	double Vx = _x - (x + w / 2);
	double Vy = _y - (y + h / 2);

	X = x + w / 2 + Vx * cos(radian) + Vy * sin(radian);
	Y = y + h / 2 - Vx * sin(radian) + Vy * cos(radian);

	//SDL_Log("x = %f y = %f w = %f h = %f X = %f Y = %f _x = %d _y = %d _cx = %d _cy = %d", x, y, w, h, X, Y, _x, _y, _x - Canvas::canvas.x, _y - Canvas::canvas.y);
	return (X >= x - 6 && X <= x + w + 6 && Y >= y - 6 && Y <= y + h + 6);
}

void Rect::Normalize(double* _x, double* _y)
{
	double radian = angle * M_PI / 180;
	double Vx = *_x - (x + w / 2);
	double Vy = *_y - (y + h / 2);

	*_x = x + w / 2 + Vx * cos(radian) + Vy * sin(radian);
	*_y = y + h / 2 - Vx * sin(radian) + Vy * cos(radian);
}

void Rect::Write(ofstream& out)
{
	Dot::Write(out);
	out.write((char*)& w, sizeof(w));
	out.write((char*)& h, sizeof(h));
	out.write((char*)& f_r, sizeof(f_r));
	out.write((char*)& f_g, sizeof(f_g));
	out.write((char*)& f_b, sizeof(f_b));
	out.write((char*)& f_a, sizeof(f_a));
	out.write((char*)& angle, sizeof(angle));
	out.write((char*)& tex_w, sizeof(tex_w));
	out.write((char*)& tex_h, sizeof(tex_h));
}

void Rect::Read(ifstream& in)
{
	Dot::Read(in);
	ID = 2;
	title = u8"прямоугольник";
	in.read((char*)& w, sizeof(w));
	in.read((char*)& h, sizeof(h));
	in.read((char*)& f_r, sizeof(f_r));
	in.read((char*)& f_g, sizeof(f_g));
	in.read((char*)& f_b, sizeof(f_b));
	in.read((char*)& f_a, sizeof(f_a));
	in.read((char*)& angle, sizeof(angle));
	in.read((char*)& tex_w, sizeof(tex_w));
	in.read((char*)& tex_h, sizeof(tex_h));

	texture = DRAW::CreateTexture(tex_w, tex_h);
	Redraw();
}

void Rect::WriteSVG(ofstream& out)
{
	out << "<rect x=\"" << x << "\" y=\"" << y << "\" width=\"" << w << "\" height=\"" << h << "\"";
	out << " stroke=\"rgb(" << (int)b_r << ", " << (int)b_g << ", " << (int)b_b << ")\"";
	out << " fill=\"rgb(" << (int)f_r << ", " << (int)f_g << ", " << (int)f_b << ")\"";
	out << " transform=\"rotate(" << angle << " " << x + w/2 << " " << y + h/2 << ")\" fill-opacity=\"" << (double)f_a/255 << "\" />\n";
}