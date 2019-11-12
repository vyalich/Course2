#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <cstdlib>
#include <string>
#include "Window.h"

class App {
private:
	Window		m_MainWin;
	SDL_Event	event;

public:
	static std::string	version;

	App() {};
	int		OnExec();
	bool	Init();
	void	Handle(SDL_Event&);
	void	CleanUp();
};