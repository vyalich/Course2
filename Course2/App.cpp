#include "App.h"

std::string App::version = u8"v0.01a";

int App::OnExec()
{
	//basic init
	if (!Init())
		return -1;
	
	//main cycle
	
	while(m_MainWin.Handle(&event) == 0)
	{
		m_MainWin.Render();
		SDL_WaitEvent(&event);
	}

	//free memory
	CleanUp();

	return 0;
}

bool App::Init() {
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, u8"Ошибка", u8"Невозможно инициализировать SDL2. Посмотрите лог для подробной информации.", NULL);
		SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, u8"Невозможно инициализировать SDL 2, ошибка: %s", SDL_GetError());
		return false;
	}

	atexit(SDL_Quit);

	if (TTF_Init() < 0)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, u8"Ошибка", u8"Невозможно инициализировать TTF. Посмотрите лог для подробной информации.", NULL);
		SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, u8"Невозможно инициализировать TTF, ошибка: %s", TTF_GetError());
		return false;
	}

	atexit(TTF_Quit);

	if (IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG) < 0)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, u8"Ошибка", u8"Невозможно инициализировать Image. Посмотрите лог для подробной информации.", NULL);
		SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, u8"Невозможно инициализировать Image, ошибка: %s", IMG_GetError());
		return false;
	}

	if (m_MainWin.Init() < 0)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, u8"Ошибка", u8"Невозможно инициализировать главное окно. Посмотрите лог для подробной информации.", NULL);
		SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, u8"Невозможно инициализировать главное окно, ошибка: %s", SDL_GetError());
		return false;
	}

	return true;
}

void App::CleanUp()
{
	m_MainWin.CleanUp();
	TTF_Quit();
	SDL_Quit();
}

int main(int argc, char** argv)
{
	App app;
	return app.OnExec();
}