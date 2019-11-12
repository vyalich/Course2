#include "Button.h"

bool Button::Init(SDL_Renderer* render, const char* path, int _x, int _y)
{
	icon = IMG_LoadTexture(render, path);
	if (icon == 0)
	{
		SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, u8"Невозможно загрузить изображение, ошибка: %s", IMG_GetError());
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
	if (_state == BTN_NONE)
	{
		_state = BTN_HOVER;
		SDL_SetTextureColorMod(icon, 0xd0, 0xd0, 0xd0);
	}
}

void Button::UnFocus()
{
	if (_state == BTN_HOVER)
	{
		_state = BTN_NONE;
		SDL_SetTextureColorMod(icon, 0xff, 0xff, 0xff);
	}
}

void Button::Press()
{
	SDL_SetTextureColorMod(icon, 0xa0, 0xa0, 0xa0);
	if(_state == BTN_HOVER)
		_state = BTN_PRESSED;
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