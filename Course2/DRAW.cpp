#include "DRAW.h"

SDL_Renderer* DRAW::render;

SDL_Texture* DRAW::CreateTexture(int w, int h)
{
	//SDL_CreateRGBSurface(0, w, h, 32, 0, 0, 0, 0);
	SDL_Texture* res = SDL_CreateTexture(render, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, w, h);
	SDL_SetTextureBlendMode(res, SDL_BLENDMODE_BLEND);
	SDL_SetRenderTarget(render, res);
	SDL_SetRenderDrawColor(render, 0, 0, 0, 0);
	SDL_RenderClear(render);
	SDL_SetRenderTarget(render, 0);

	return res;
}

void DRAW::DrawText(SDL_Renderer* Render, TTF_Font* font, const char* title, int X, int Y, int W, int H, SDL_Color* color, SDL_Color* bg_color)
{
	//Render text surface
	SDL_Surface* textSurface;
	string n_title;

	if (title == "")
		n_title = u8" ";
	else
		n_title = title;

	textSurface = TTF_RenderUTF8_Shaded(font, title, *color, *bg_color);

	if (textSurface == 0)
	{
		SDL_Log("Unable to render text surface! SDL_ttf Error: %s", TTF_GetError());
	}
	else
	{
		//Create texture from surface pixels
		SDL_Texture* texture = SDL_CreateTextureFromSurface(Render, textSurface);

		if (texture == 0)
		{
			SDL_Log("Unable to create texture from rendered text! SDL Error: %s", SDL_GetError());
			return;
		}

		SDL_Rect dest = { X + W / 2 - textSurface->w / 2, Y + H / 2 - textSurface->h / 2, textSurface->w, textSurface->h };
		/*SDL_Texture* texTarget = SDL_CreateTexture(Render, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, textSurface->w, textSurface->h);

		//Now render to the texture
		SDL_SetRenderTarget(Render, texTarget);
		SDL_RenderClear(Render);
		SDL_RenderCopy(Render, texture, NULL, NULL);
		//Detach the texture
		SDL_SetRenderTarget(Render, NULL);

		SDL_RenderCopy(Render, texTarget, 0, &dest);

		SDL_FreeSurface(textSurface);
		SDL_DestroyTexture(texture);
		SDL_DestroyTexture(texTarget);
		texture = nullptr;
		textSurface = nullptr;*/

		SDL_RenderCopy(Render, texture, 0, &dest);
		SDL_DestroyTexture(texture);
		SDL_FreeSurface(textSurface);
	}
}