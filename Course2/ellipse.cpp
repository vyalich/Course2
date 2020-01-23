#include "ellipse.h"

void Ellipse::Init(int _x, int _y)
{
	ID = 3;
	x = _x, y = _y;
	a = a_rel = b = b_rel = 0;

	_selected = true;
	_creating = true;

	tex_w = tex_h = 256;
	texture = DRAW::CreateTexture(tex_w, tex_h);
	title = u8"эллипс";
}

void Ellipse::Render(SDL_Renderer* render)
{
	SDL_Rect src = { 0, 0, 2 * a + 6, 2 * b + 6};
	SDL_FRect dst = { x - a - 3, y - b - 3, 2 * a + 6, 2 * b + 6};
	SDL_RenderCopyExF(DRAW::render, texture, &src, &dst, angle, 0, SDL_FLIP_NONE);
}

void Ellipse::DrawPixels(int X, int Y)
{
	if (f_a > 0 && last_y != 2 + (int)b + Y)
	{
		last_y = 2 + (int)b + Y;
		SDL_SetRenderDrawColor(DRAW::render, f_r, f_g, f_b, f_a);

		SDL_RenderDrawLine(DRAW::render, 2 + (int)a - X, 2 + (int)b + Y, 2 + (int)a + X, 2 + (int)b + Y);
		if(last_y > b + 2)
			SDL_RenderDrawLine(DRAW::render, 2 + (int)a - X, 2 + (int)b - Y, 2 + (int)a + X, 2 + (int)b - Y);
	}

	SDL_SetRenderDrawColor(DRAW::render, b_r, b_g, b_b, 0xff);

	SDL_RenderDrawPointF(DRAW::render, 2 + (int)a + X, 2 + (int)b + Y);
	SDL_RenderDrawPointF(DRAW::render, 2 + (int)a - X, 2 + (int)b + Y);
	SDL_RenderDrawPointF(DRAW::render, 2 + (int)a + X, 2 + (int)b - Y);
	SDL_RenderDrawPointF(DRAW::render, 2 + (int)a - X, 2 + (int)b - Y);
}

void Ellipse::Redraw()
{
	SDL_SetRenderTarget(DRAW::render, texture);
	SDL_SetRenderDrawColor(DRAW::render, 0, 0, 0, 0);
	SDL_RenderClear(DRAW::render);
	
	last_y = 3*b;

	int X = 0, Y = (int)(b-1);
	int a2 = (int)(a-1) * (int)(a-1), b2 = (int)(b-1) * (int)(b-1);

	int delta = 4 * b2 * ((X + 1) * (X + 1)) + a2 * ((2 * Y - 1) * (2 * Y - 1)) - 4 * a2 * b2; // Функция координат точки (x+1, y-1/2)

	while (a2 * (2 * Y - 1) > 2 * b2 * (X + 1)) // Первая часть дуги
	{
		DrawPixels(X, Y);
		if (delta < 0) // Переход по горизонтали
		{
			X++;
			delta += 4 * b2 * (2 * X + 3);
		}
		else // Переход по диагонали
		{
			X++;
			delta = delta - 8 * a2 * (Y - 1) + 4 * b2 * (2 * X + 3);
			Y--;
		}
	}

	delta = b2 * ((2 * X + 1) * (2 * X + 1)) + 4 * a2 * ((Y + 1) * (Y + 1)) - 4 * a2 * b2; // Функция координат точки (x+1/2, y-1)

	while (Y + 1 != 0) // Вторая часть дуги, если не выполняется условие первого цикла, значит выполняется a^2(2y - 1) <= 2b^2(x + 1)
	{
		DrawPixels(X, Y);
		if (delta < 0) // Переход по вертикали
		{
			Y--;
			delta += 4 * a2 * (2 * Y + 3);
		}
		else // Переход по диагонали
		{
			Y--;
			delta = delta - 8 * b2 * (X + 1) + 4 * a2 * (2 * Y + 3);
			X++;
		}
	}

	if (_selected)
		DrawUI();

	SDL_SetRenderTarget(DRAW::render, 0);
}

void Ellipse::Handle(SDL_Event* event)
{
	if (_creating)
	{
		x += event->motion.xrel / 2.0;
		y += event->motion.yrel / 2.0;
		a = abs(a_rel += event->motion.xrel / 2.0);
		b = abs(b_rel += event->motion.yrel / 2.0);

		ResizeTexture();
		Redraw();
	}

	int n;
	tie(b_r, b_g, b_b, n) = Palette::border.GetColor();
	tie(f_r, f_g, f_b, f_a) = Palette::fill.GetColor();

	double radian = angle * M_PI / 180;
	double Vx = (double)event->button.x - Canvas::canvas.x - (x);
	double Vy = (double)event->button.y - Canvas::canvas.y - (y);

	double Rx = Vx * cos(radian) + Vy * sin(radian);
	double Ry = Vx * sin(radian) - Vy * cos(radian);

	//координаты курсора на прямоугольнике без поворота
	X = x + Rx;
	Y = y - Ry;

	//вектор скалирования
	//double Relx = (double)event->button.x - x - w / 2;
	//double Rely = (double)event->button.y - y - h / 2;

	double XREL;
	if (Rx > 0)
		XREL = Rx - a;
	else
		XREL = Rx + a;

	double YREL;
	if (Ry < 0)
		YREL = -Ry - b;
	else
		YREL = -Ry + b;

	switch (event->type)
	{
	case SDL_MOUSEBUTTONDOWN:
		pn = 0;
		if (abs(x - a - X) < 3)
		{
			pn += LEFT;
			_state = SCALE;
		}
		else if (abs(x + a - X) < 3)
		{
			pn += RIGHT;
			_state = SCALE;
		}
		if (abs(y - b - Y) < 3)
		{
			pn += TOP;
			_state = SCALE;
		}
		else if (abs(y + b - Y) < 3)
		{
			pn += BOT;
			_state = SCALE;
		}

		if ((X <= x - a - 3 || X >= x + a + 3) && (Y <= y - b - 3 || Y >= y + b + 3))
			_state = ROTATE;

		if (X > x - a + 3 && X < x + a - 4 && Y > y - b + 3 && Y < y + b - 4)
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
				a -= XREL/2;
				if (a < 1)
				{
					a += XREL/2;
					x -= XREL;
				}
			}
			else if (pn & RIGHT)
			{
				a += XREL/2;
				if (a < 1)
					a = 1;
			}
			if (pn & TOP)
			{
				b -= YREL/2;
				if (b < 1)
				{
					b += YREL/2;
					y -= YREL;
				}
			}
			else if (pn & BOT)
			{
				b += YREL/2;
				if (b < 1)
					b = 1;
			}
			//x = cx - w / 2;
			//y = cy - h / 2;
			break;
		case ROTATE:
		{
			double Ax = (event->button.x - Canvas::canvas.x + event->motion.xrel) - (x);
			double Ay = (event->button.y - Canvas::canvas.y + event->motion.yrel) - (y);
			double Bx = event->button.x - Canvas::canvas.x - (x);
			double By = event->button.y - Canvas::canvas.y - (y);
			double cosalfa = (Ax * Bx + Ay * By) / sqrt(Ax * Ax + Ay * Ay) / sqrt(Bx * Bx + By * By);
			double alfa = acos(cosalfa) * 180 / M_PI;
			double m_def = Ax * By - Bx * Ay;
			if (m_def > 0)
				angle -= alfa;
			else if (m_def < 0)
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

void Ellipse::ResizeTexture()
{
	bool resize = false;
	if (a + 3 > tex_w / 2)
	{
		tex_w *= 2;
		resize = true;
	}
	else if (a + 3 < tex_w / 4)
	{
		tex_w /= 2;
		resize = true;
	}
	if (b + 3 > tex_h / 2)
	{
		tex_h *= 2;
		resize = true;
	}
	else if (b + 3 < tex_h / 4)
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

void Ellipse::Select()
{
	Palette::border.SetColor(b_r, b_g, b_b, 255);
	Palette::fill.SetColor(f_r, f_g, f_b, f_a);
	_selected = true;
	Redraw();
}

void Ellipse::UnSelect()
{
	_selected = false;
	Redraw();
}

void Ellipse::DrawUI()
{
	SDL_SetRenderDrawColor(DRAW::render, 0, 0, 0, SDL_ALPHA_OPAQUE);

	SDL_Rect dst = { 3, 3, 2*a, 2*b };
	SDL_RenderDrawRect(DRAW::render, &dst);

	dst = { 0, 0, 7, 7 };

	SDL_SetRenderDrawColor(DRAW::render, 0, 0, 0, 0xff);
	SDL_RenderFillRect(DRAW::render, &dst);
	dst.y = b;		SDL_RenderFillRect(DRAW::render, &dst);
	dst.y = 2*b-1;		SDL_RenderFillRect(DRAW::render, &dst);
	dst.x = a;		SDL_RenderFillRect(DRAW::render, &dst);
	dst.y = 0;			SDL_RenderFillRect(DRAW::render, &dst);
	dst.x = 2*a - 1;		SDL_RenderFillRect(DRAW::render, &dst);
	dst.y = b;		SDL_RenderFillRect(DRAW::render, &dst);
	dst.y = 2*b - 1;		SDL_RenderFillRect(DRAW::render, &dst);
}

bool Ellipse::MouseOver(int _x, int _y)
{
	double radian = angle * M_PI / 180;
	double Vx = _x - (x);
	double Vy = _y - (y);

	X = x + Vx * cos(radian) + Vy * sin(radian);
	Y = y - Vx * sin(radian) + Vy * cos(radian);
	return (X >= x - a - 6 && X <= x + a + 6 && Y >= y - b - 6 && Y <= y + b + 6);
}

void Ellipse::Write(ofstream& out)
{
	Dot::Write(out);
	out.write((char*)& a, sizeof(a));
	out.write((char*)& b, sizeof(b));
	out.write((char*)& f_r, sizeof(f_r));
	out.write((char*)& f_g, sizeof(f_g));
	out.write((char*)& f_b, sizeof(f_b));
	out.write((char*)& f_a, sizeof(f_a));
	out.write((char*)& angle, sizeof(angle));
	out.write((char*)& tex_w, sizeof(tex_w));
	out.write((char*)& tex_h, sizeof(tex_h));
}

void Ellipse::Read(ifstream& in)
{
	Dot::Read(in);
	ID = 3;
	title = u8"эллипс";
	in.read((char*)& a, sizeof(a));
	in.read((char*)& b, sizeof(b));
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

void Ellipse::WriteSVG(ofstream& out)
{
	out << "<ellipse cx=\"" << x << "\" cy=\"" << y << "\" rx=\"" << a << "\" ry=\"" << b << "\"";
	out << " stroke=\"rgb(" << (int)b_r << ", " << (int)b_g << ", " << (int)b_b << ")\"";
	out << " fill=\"rgb(" << (int)f_r << ", " << (int)f_g << ", " << (int)f_b << ")\"";
	out << " transform=\"rotate(" << angle << " " << x << " " << y << ")\" fill-opacity=\"" << (double)f_a / 255 << "\" />\n";
}