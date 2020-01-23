#pragma once
#include <SDL.h>
#include <string>
#include <tuple>
#include <fstream>
#include "DRAW.h"
#include "Palette.h"

using namespace std;

class Dot
{
protected:
	string	title;

	int ID;

	bool	_selected;
	bool	_creating;
	char	_state;

	double	x, y;
	Uint8	b_r, b_g, b_b;

public:
	
	virtual void Init(int _x, int _y);

	virtual void Render(SDL_Renderer* render);

	virtual void Handle(SDL_Event* event);

	virtual void Clear() {};

	virtual void Select() { _selected = true; Palette::border.SetColor(b_r, b_g, b_b, 255);	}

	virtual void UnSelect() { _selected = false; }

	void StopCreate() { _creating = false; }

	bool IsCreating() { return _creating; }
	
	virtual bool MouseOver(int _x, int _y) { return (_x >= x - 3 && _x <= x + 3 && _y >= y - 3 && _y <= y + 3); }

	virtual void Write(ofstream &out);

	virtual void WriteSVG(ofstream& out);

	virtual void Read(ifstream& in);
};

