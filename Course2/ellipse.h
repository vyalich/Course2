#pragma once

#include "Rect.h"
#include "Canvas.h"
#include "DRAW.h"

class Ellipse: public Dot
{
protected:

	double			a, b, a_rel, b_rel;
	int			last_y;
	double			X, Y;
	char			pn;
	double			angle;
	Uint8			f_r, f_g, f_b, f_a;
	SDL_Texture*	texture;
	int				tex_w, tex_h;

public:
	virtual void Init(int _x, int _y);

	virtual void Render(SDL_Renderer* render);

	virtual void Handle(SDL_Event* event);

	void DrawPixels(int X, int Y);

	void Select();

	void UnSelect();

	void Redraw();

	void DrawUI();

	void ResizeTexture();

	bool MouseOver(int _x, int _y);

	virtual void Write(ofstream& out);

	virtual void Read(ifstream& in);

	virtual void Clear() { SDL_DestroyTexture(texture); };

	virtual void WriteSVG(ofstream& out);
};

