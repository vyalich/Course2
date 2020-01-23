#pragma once

#include <SDL.h>
#include "Dot.h"
#include "Enums.h"
#include "Canvas.h"

class Rect: public Dot
{
protected:
	Uint8	f_r, f_g, f_b, f_a;
	double	X, Y;
	double	w, h;
	double	angle;
	char	pn;

	SDL_Texture* texture;
	int			 tex_w, tex_h;

public:
	virtual void Init(int _x, int _y);

	virtual void Render(SDL_Renderer* render);

	virtual void Handle(SDL_Event* event);

	virtual void Clear() { SDL_DestroyTexture(texture); }

	bool MouseOver(int x, int y);

	void Select();

	void UnSelect();

	void Redraw();

	void DrawUI();

	void ResizeTexture();

	void Normalize(double* _x, double* _y);

	virtual void Write(ofstream& out);

	virtual void Read(ifstream& in);

	virtual void WriteSVG(ofstream& out);
};

