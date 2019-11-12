#include "Window.h"
#include "App.h"

int	Window::Init()
{
	//default resolution
	winH = 480; winW = 640;

	if (SDL_GetNumVideoDisplays() < 1)
		return -1;
	
	//getting display resolution
	SDL_DisplayMode current;
	if (SDL_GetCurrentDisplayMode(0, &current) < 0)
	{
		SDL_Log("Could not get display mode for video display #0: %s", SDL_GetError());
	}
	else
	{
		winW = current.w*0.8; winH = current.h*0.8;
	}

	std::string title = "My VGE " + App::version;

	//create main window
	window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
		                      winW, winH, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
	if (window == 0)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, u8"Ошибка", u8"Невозможно создать окно. Посмотрите лог для подробной информации.", NULL);
		SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, u8"Невозможно создать окно, ошибка: %s", SDL_GetError());
		return -2;
	}

	//create renderer for window
	render = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (render == NULL)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, u8"Ошибка", u8"Невозможно создать рендерер. Посмотрите лог для подробной информации.", NULL);
		SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, u8"Невозможно создать рендерер, ошибка: %s", SDL_GetError());
		return -3;
	}
	SDL_SetRenderDrawColor(render, 0xFF, 0xFF, 0xFF, 0x00);

	m_ShapeBar.Init(render, "./img/shapebar-*.jpg", 10, 10, 6, VERTICAL, 15);

	return 0;
}

int Window::Handle(SDL_Event* event)
{
	int mx = 0, my = 0;
	switch (event->type)
	{
	case SDL_KEYDOWN:
		if (event->key.keysym.sym == SDLK_q && event->key.keysym.mod == KMOD_LCTRL)
		{
			//SDL_Event exitEvent = { SDL_QUIT };
			//SDL_PushEvent(&exitEvent);
			return 1;
		}
		SDL_Log(u8"нажата клавиша %d", event->key.keysym.sym);
		break;

	case SDL_KEYUP:
		SDL_Log(u8"отпущена клавиша %d", event->key.keysym.sym);
		break;
	case SDL_TEXTEDITING:
		SDL_Log(u8"Выделение текста. Выделенный текст '%s', начало в %d и длина выделения %d", event->edit.text, event->edit.start, event->edit.length);
		break;
	case SDL_TEXTINPUT:
		SDL_Log(u8"Ввод текста с клавиатуры. Текст '%s'", event->text.text);
		break;
	case SDL_FINGERMOTION:
		SDL_Log(u8"Сенсор: %i, x: %i, y: %i", event->tfinger.fingerId, event->tfinger.x, event->tfinger.y);
		break;
	case SDL_FINGERDOWN:
		SDL_Log(u8"Сенсор: %lld нажат - x: %i, y: %i",event->tfinger.fingerId, event->tfinger.x, event->tfinger.y);
		break;
	case SDL_FINGERUP:
		SDL_Log(u8"Сенсор: %lld отпущен - x: %i, y: %i", event->tfinger.fingerId, event->tfinger.x, event->tfinger.y);
		break;
	case SDL_MULTIGESTURE:
		SDL_Log(u8"Жест: x = %f, y = %f, угол = %f, длина = %f", event->mgesture.x, event->mgesture.y, event->mgesture.dTheta, event->mgesture.dDist);
		SDL_Log(u8"Жест: количество касаний = %i", event->mgesture.numFingers);
		break;
	case SDL_DOLLARGESTURE:
		SDL_Log(u8"Жест %lld выполнен, ошибка: %f", event->dgesture.gestureId, event->dgesture.error);
		break;
	case SDL_DOLLARRECORD:
		SDL_Log(u8"Записан жест: %lld", event->dgesture.gestureId);
		break;
	case SDL_MOUSEMOTION:
		//SDL_Log(u8"Движение мышью. X=%d, Y=%d, Относительный X=%d, Относительный Y=%d", event->motion.x, event->motion.y, event->motion.xrel, event->motion.yrel);
		SDL_GetMouseState(&mx, &my);
		break;
	case SDL_MOUSEBUTTONDOWN:
		SDL_Log(u8"Нажата кнопка мыши %u", event->button.button);
		SDL_GetMouseState(&mx, &my);
		break;
	case SDL_MOUSEBUTTONUP:
		SDL_Log(u8"Отпущена кнопка мыши %u", event->button.button);
		SDL_GetMouseState(&mx, &my);
		break;
	case SDL_MOUSEWHEEL:
		SDL_Log(u8"Колесо мыши X=%d, Y=%d", event->wheel.x, event->wheel.y);
		SDL_GetMouseState(&mx, &my);
		break;
	case SDL_QUIT:
		SDL_Log(u8"Пользователь запросил выход");
		return 1;
	case SDL_WINDOWEVENT:
		switch (event->window.event)
		{
		case SDL_WINDOWEVENT_SHOWN:
			SDL_Log(u8"Окно %d показано", event->window.windowID);
			break;
		case SDL_WINDOWEVENT_HIDDEN:
			SDL_Log(u8"Окно %d спрятано", event->window.windowID);
			break;
		case SDL_WINDOWEVENT_EXPOSED:
			SDL_Log(u8"Окно %d подверглось действию", event->window.windowID);
			break;
		case SDL_WINDOWEVENT_MOVED:
			SDL_Log(u8"Окно %d передвинуто %d,%d", event->window.windowID, event->window.data1, event->window.data2);
			break;
		case SDL_WINDOWEVENT_RESIZED:
			SDL_Log(u8"Окно %d изменило размер %dx%d", event->window.windowID, event->window.data1, event->window.data2);
			if (event->window.data1 < 640)
				winW = 640;
			else
				winW = event->window.data1;
			if (event->window.data2 < 480)
				winH = 480;
			else
				winH = event->window.data2;
			SDL_SetWindowSize(window, winW, winH);
			break;
		case SDL_WINDOWEVENT_SIZE_CHANGED:
			SDL_Log(u8"Размер окна %d изменился на %dx%d", event->window.windowID, event->window.data1, event->window.data2);
			break;
		case SDL_WINDOWEVENT_MINIMIZED:
			SDL_Log(u8"Окно %d свернуто", event->window.windowID);
			break;
		case SDL_WINDOWEVENT_MAXIMIZED:
			SDL_Log(u8"Окно %d на весь экран", event->window.windowID);
			break;
		case SDL_WINDOWEVENT_RESTORED:
			SDL_Log(u8"Окно %d развернуто", event->window.windowID);
			break;
		case SDL_WINDOWEVENT_ENTER:
			SDL_Log(u8"Мышь попала в окно %d", event->window.windowID);
			break;
		case SDL_WINDOWEVENT_LEAVE:
			SDL_Log(u8"Мышь покинула окно %d", event->window.windowID);
			break;
		case SDL_WINDOWEVENT_FOCUS_GAINED:
			SDL_Log(u8"Окно %d получило фокус клавиатуры", event->window.windowID);
			break;
		case SDL_WINDOWEVENT_FOCUS_LOST:
			SDL_Log(u8"Окно %d потеряло фокус клавиатуры", event->window.windowID);
			break;
		case SDL_WINDOWEVENT_CLOSE:
		{
			SDL_Log(u8"Окно %d закрыто", event->window.windowID);
			return 1;
		}
		default:
			SDL_Log(u8"В окне %d произошло неизвестное событие %d", event->window.windowID, event->window.event);
			break;
		}
		break;
	default:
		SDL_Log(u8"Неизвестное событие %d", event->type);
		break;
	}
	if(m_ShapeBar.MouseOver(mx, my))
		m_ShapeBar.Handle(event);
	return 0;
}

void Window::Render()
{
	SDL_SetRenderDrawColor(render, 0xff, 0xff, 0xff, 0x0);
	SDL_RenderClear(render);

	m_ShapeBar.Render(render);

	int mx, my;
	SDL_GetMouseState(&mx, &my);
	SDL_SetRenderDrawColor(render, 0x0f, 0x0f, 0x0f, 0xa0);
	SDL_RenderDrawLine(render, mx, 0, mx, winH - 1);
	SDL_RenderDrawLine(render, 0, my, winW-1, my);

	SDL_RenderPresent(render);
}

void Window::CleanUp()
{
	SDL_DestroyRenderer(render);
	SDL_DestroyWindow(window);
}