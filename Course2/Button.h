#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include "Enums.h"


class Button
{
private:
	SDL_Rect		pos;
	int				_state;
	bool			_clicked;
	SDL_Texture*	icon;

public:
	Button() {}
	~Button() {}

	bool Init(SDL_Renderer* render, const char* path, int x, int y);
	void GetSize(int* _w, int* _h) { *_w = pos.w; *_h = pos.h; }
	void Render(SDL_Renderer* render);
	void CleanUP() { SDL_DestroyTexture(icon); }
	bool MouseOver(int mx, int my) { return (pos.x <= mx && mx <= pos.x + pos.w && pos.y <= my && my <= pos.y + pos.h); }
	void Focus();
	void UnFocus();
	void Press();
	bool Release();
	bool IsFocused() { return (_state == BTN_HOVER); }
	bool IsPressed() { return (_state == BTN_PRESSED); }

};

