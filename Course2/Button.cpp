#include "Button.h"

bool Button::Init(SDL_Renderer* render, const char* path, int _x, int _y)
{
	icon = IMG_LoadTexture(render, path);
	if (icon == 0)
	{
		SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, u8"���������� ��������� �����������, ������: %s", IMG_GetError());
		return false;
	}
	pos.x = _x; pos.y = _y;
	SDL_QueryTexture(icon, 0, 0, &pos.w, &pos.h);
	_state = BTN_NONE;
	SDL_SetTextureColorMod(icon, 0xff, 0xff, 0xff);

	return true;
}

void Button::Render(SDL_Renderer* render)
{
	SDL_RenderCopy(render, icon, 0, &pos);
}

void Button::Focus()
{
	_state = BTN_HOVER;
	SDL_SetTextureColorMod(icon, 0x70, 0x70, 0x70);	//75%
}

void Button::UnFocus()
{
	_state = BTN_NONE;
	SDL_SetTextureColorMod(icon, 0xff, 0xff, 0xff);	//100%
}

void Button::Press()
{
	_state = BTN_PRESSED;
	SDL_SetTextureColorMod(icon, 0x30, 0x30, 0x30);	//30%
}

bool Button::Release()
{
	if (_state == BTN_PRESSED)
	{	
		_state = BTN_ACTIVE;
		return true;
	}
	return false;
}