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
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, u8"������", u8"���������� ���������������� SDL2. ���������� ��� ��� ��������� ����������.", NULL);
		SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, u8"���������� ���������������� SDL 2, ������: %s", SDL_GetError());
		return false;
	}

	atexit(SDL_Quit);

	if (TTF_Init() < 0)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, u8"������", u8"���������� ���������������� TTF. ���������� ��� ��� ��������� ����������.", NULL);
		SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, u8"���������� ���������������� TTF, ������: %s", TTF_GetError());
		return false;
	}

	atexit(TTF_Quit);

	if (IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG) < 0)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, u8"������", u8"���������� ���������������� Image. ���������� ��� ��� ��������� ����������.", NULL);
		SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, u8"���������� ���������������� Image, ������: %s", IMG_GetError());
		return false;
	}

	if (m_MainWin.Init() < 0)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, u8"������", u8"���������� ���������������� ������� ����. ���������� ��� ��� ��������� ����������.", NULL);
		SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, u8"���������� ���������������� ������� ����, ������: %s", SDL_GetError());
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