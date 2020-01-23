#pragma once

#include "Dot.h"
#include "Enums.h"
#include "Canvas.h"

class Line : public Dot
{
protected:
	double	x2, y2;

public:
	virtual void Init(int _x, int _y);

	virtual void Render(SDL_Renderer* render);

	virtual void Handle(SDL_Event* event);

	virtual void Clear() {};

	virtual bool MouseOver(int _x, int _y);

	virtual void Write(ofstream& out);

	virtual void Read(ifstream& in);

	virtual void WriteSVG(ofstream& out);
};

