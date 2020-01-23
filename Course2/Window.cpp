#include "Window.h"
#include "App.h"

int	Window::Init()
{
	//default resolution
	winH = 600; winW = 800;

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
		                      winW, winH, SDL_WINDOW_SHOWN );
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

	font = TTF_OpenFont("a_AvanteBs_ExtraBold.ttf", 22);

	DRAW::render = render;

	SDL_SetRenderDrawBlendMode(DRAW::render, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(render, 0xFF, 0xFF, 0xFF, 0x00);

	for (int i = 0; i < 4; i++)
	{
		Button* tmp = new Button;
		string str = "./img/button-*.jpg";
		str[str.find("*")] = i + 49;
		tmp->Init(render, str.c_str(), winW - 138 + (i % 2) * 69, 5 + i / 2 * 69);
		file_control.push_back(tmp);
	}

	m_ShapeBar.Init(render, "./img/shapebar-*.jpg", 0, 0, 6, VERTICAL, 5);
	canvas.Init(render);
	Palette::border.Init(5, 419, false);
	Palette::fill.Init(39, 419, true);


	workspace = { 94, 0, winW - 94 - 139, winH};

	Canvas::canvas = { winW, winH, 0, 0 };

	mb_down = false;

	return 0;
}

int Window::Handle(SDL_Event* event)
{
	int mx = 0, my = 0;
	SDL_GetMouseState(&mx, &my);

	switch (event->type)
	{
	case SDL_KEYDOWN:
		if (event->key.keysym.sym == SDLK_q && event->key.keysym.mod == KMOD_LCTRL)
		{
			//SDL_Event exitEvent = { SDL_QUIT };
			//SDL_PushEvent(&exitEvent);
			return 1;
		}
		if (event->key.keysym.sym == SDLK_DOWN)
		{
			m_ShapeBar.Activate0();
			SDL_Log("cur: %d size: %d", Canvas::_current, Canvas::v_elements.size() - 1);
			if (Canvas::_current < (int)(Canvas::v_elements.size() - 1))
			{
				//SDL_Log("%d", Canvas::_current);
				if (Canvas::_current >= 0)
					Canvas::v_elements[Canvas::_current]->UnSelect();
				Canvas::_current++;
				Canvas::v_elements[Canvas::_current]->Select();
				//SDL_Log("%d", Canvas::_current);
			}
			
		}
		if (event->key.keysym.sym == SDLK_UP)
		{
			m_ShapeBar.Activate0();
			if (Canvas::_current > 0)
			{
				Canvas::v_elements[Canvas::_current]->UnSelect();
				Canvas::_current--;
				Canvas::v_elements[Canvas::_current]->Select();
			}
			//SDL_Log("%d", Canvas::_current);
		}
		if (event->key.keysym.sym == SDLK_DELETE && Canvas::_current >= 0)
		{
			Canvas::v_elements[Canvas::_current]->Clear();
			Canvas::v_elements.erase(Canvas::v_elements.begin() + Canvas::_current);
			Canvas::_current = -1;
		}
		//SDL_Log(u8"нажата клавиша %d", event->key.keysym.sym);
		break;

	case SDL_KEYUP:
		//SDL_Log(u8"отпущена клавиша %d", event->key.keysym.sym);
		break;
	case SDL_TEXTEDITING:
		//SDL_Log(u8"Выделение текста. Выделенный текст '%s', начало в %d и длина выделения %d", event->edit.text, event->edit.start, event->edit.length);
		break;
	case SDL_TEXTINPUT:
		//SDL_Log(u8"Ввод текста с клавиатуры. Текст '%s'", event->text.text);
		break;
	case SDL_MOUSEMOTION:
		if (Canvas::_current >= 0  && mb_down)
		{
			Canvas::v_elements[Canvas::_current]->Handle(event);
		}
		break;
	case SDL_MOUSEBUTTONDOWN:
		mb_down = true;
		if (InWorkspace(event->button.x, event->button.y))
		{
			if (Bar::_active)
			{
				Dot* tmp = 0;
				switch (Bar::_active)
				{
				case 1:
					tmp = new Dot;
					break;
				case 2:
					tmp = new Line;
					break;
				case 3:
					tmp = new Rect;
					break;
				case 4:
					tmp = new Ellipse;
					break;
				case 5:
					tmp = new Dot;	//for text
					break;
				default:
					break;
				}
				tmp->Init(event->button.x - Canvas::canvas.x, event->button.y - Canvas::canvas.y);
				Canvas::v_elements.push_back(tmp);
				Canvas::_current = Canvas::v_elements.size() - 1;
			}
			else if (Canvas::_current >= 0 && Canvas::v_elements[Canvas::_current]->MouseOver(mx - Canvas::canvas.x, my - Canvas::canvas.y))
			{
				Canvas::v_elements[Canvas::_current]->Handle(event);
			}
		}
		break;
	case SDL_MOUSEBUTTONUP:
		mb_down = false;
		if (Canvas::_current >= 0)
		{
			if (Bar::_active)
			{
				Canvas::v_elements[Canvas::_current]->StopCreate();
				Canvas::v_elements[Canvas::_current]->UnSelect();
				Canvas::_current = -1;
			}
			else
				Canvas::v_elements[Canvas::_current]->Handle(event);
		}
		

		break;
	case SDL_MOUSEWHEEL:
		//SDL_Log(u8"Колесо мыши X=%d, Y=%d", event->wheel.x, event->wheel.y);
		break;
	case SDL_QUIT:
		//SDL_Log(u8"Пользователь запросил выход");
		return 1;
	case SDL_WINDOWEVENT:
		switch (event->window.event)
		{
		case SDL_WINDOWEVENT_RESIZED:
			//SDL_Log(u8"Окно %d изменило размер %dx%d", event->window.windowID, event->window.data1, event->window.data2);
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
			//SDL_Log(u8"Размер окна %d изменился на %dx%d", event->window.windowID, event->window.data1, event->window.data2);
			break;
		case SDL_WINDOWEVENT_CLOSE:
		{
			//SDL_Log(u8"Окно %d закрыто", event->window.windowID);
			return 1;
		}
		default:
			//SDL_Log(u8"В окне %d произошло неизвестное событие %d", event->window.windowID, event->window.event);
			break;
		}
		break;
	default:
		//SDL_Log(u8"Неизвестное событие %d", event->type);
		break;
	}
	if (m_ShapeBar.MouseOver(event->button.x, event->button.y))
	{
		m_ShapeBar.Handle(event);
		if (Bar::_active && Canvas::_current >= 0)
		{
			Canvas::v_elements[Canvas::_current]->UnSelect();
			Canvas::_current = -1;
		}
	}
	else
		m_ShapeBar.Unfocus();

	Palette::border.Handle(event);
	if (Palette::border.IsOpened())
		Palette::fill.Close();
	Palette::fill.Handle(event);
	if (Palette::fill.IsOpened())
		Palette::border.Close();
	HandleFileControl(event);

	return 0;
}

void Window::Render()
{
	SDL_SetRenderDrawColor(render, 0xa0, 0xa0, 0xa0, 0xff);
	SDL_RenderClear(render);

	canvas.Render(render);

	SDL_RenderSetViewport(render, &Canvas::canvas);

	for (int i = 0; i < Canvas::v_elements.size(); i++)
	{
		//SDL_Log("print %d", i);
		if(i != Canvas::_current)
			Canvas::v_elements[i]->Render(render);
	}

	if(Canvas::_current >= 0)
		Canvas::v_elements[Canvas::_current]->Render(render);

	SDL_RenderSetViewport(render, 0);

	SDL_Rect dst = { 0, 0, 74, winH };
	SDL_SetRenderDrawColor(DRAW::render, 0x30, 0x30, 0x30, 255);
	SDL_RenderFillRect(DRAW::render, &dst);
	
	SDL_Rect dst2 = { winW - 144, 0, 143, winH };
	SDL_RenderFillRect(DRAW::render, &dst2);

	m_ShapeBar.Render(render);
	Palette::border.Render();
	Palette::fill.Render();
	for (int i = 0; i < 4; i++)
	{
		file_control[i]->Render(render);
	}

	SDL_RenderPresent(render);
}

void Window::HandleFileControl(SDL_Event* event)
{
	for (int i = 0; i < 4; i++)
	{
		if (file_control[i]->MouseOver(event->button.x, event->button.y))
		{
			file_control[i]->Focus();
			switch (event->type)
			{
			case SDL_MOUSEBUTTONDOWN:
				file_control[i]->Press();
				SDL_Log("pressed %d", i);
				break;
			case SDL_MOUSEBUTTONUP:
				SDL_Log("released %d", i);
				file_control[i]->Release();
				switch (i) {
				case 0:
					//create new canvas and clear old
					Create();
					break;
				case 1:
					//open bin file
					LoadMVG();
					break;
				case 2:
					//save bin file
					SaveMVG();
					break;
				case 3:
					//save svg file
					SaveSVG();
					break;
				}
				break;
			}
		}
		else
			file_control[i]->UnFocus();
	}

	
}

void Window::CleanUp()
{
	for (int i = 0; i < Canvas::v_elements.size(); i++)
		Canvas::v_elements[i]->Clear();
	canvas.Clear();
	SDL_DestroyRenderer(render);
	SDL_DestroyWindow(window);
}

void Window::InputText(const char* hint)
{

	bool done = false;
	bool first = true;
	SDL_Rect rect = { (winW - 217) / 2 + 74 - 100, 300, 200, 40 };
	SDL_Event event;

	SDL_StartTextInput();
	buf.clear();
	buf = hint;

	SDL_SetRenderDrawColor(render, 0xff, 0xff, 0xff, 0xff);
	SDL_RenderFillRect(render, &rect);

	SDL_Color color = { 0x0, 0x0, 0x0, 0xff };
	SDL_Color bgcolor = { 0xff, 0xff, 0xff, 0xff };
	DRAW::DrawText(render, font, buf.c_str(), rect.x, rect.y, rect.w, rect.h, &color, &bgcolor);

	SDL_RenderPresent(render);

	while (!done)
	{

		if (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_KEYDOWN:
				if (event.key.keysym.sym == SDLK_RETURN)
				{
					done = true;
				}
				else if (event.key.keysym.sym == SDLK_BACKSPACE && buf.length() > 0)
				{
					buf.pop_back();
				}
				break;
			case SDL_TEXTINPUT:
				if (first)
				{
					buf.clear();
					first = false;
				}
				buf += event.text.text;

				break;
			}
		}
		SDL_SetRenderDrawColor(render, 0xff, 0xff, 0xff, 0xff);
		SDL_RenderFillRect(render, &rect);

		SDL_Color color = { 0x0, 0x0, 0x0, 0xff };
		SDL_Color bgcolor = { 0xff, 0xff, 0xff, 0xff };
		DRAW::DrawText(render, font, buf.c_str(), rect.x, rect.y, rect.w, rect.h, &color, &bgcolor);

		SDL_RenderPresent(render);
	}

	SDL_StopTextInput();

	//return buf.c_str();
}

void Window::Create()
{
	canvas.Clear();

	InputText(u8"Введите ширину");
	Canvas::canvas.w = stoi(buf);

	InputText(u8"Введите высоту");
	Canvas::canvas.h = stoi(buf);

	Canvas::canvas.x = (winW - 217) / 2 + 74 - Canvas::canvas.w / 2;
	Canvas::canvas.y = winH / 2 - Canvas::canvas.h / 2;
}

void Window::SaveMVG()
{
	nfdchar_t* savePath = NULL;
	nfdresult_t result = NFD_SaveDialog("mvg", NULL, &savePath);
	if (result == NFD_OKAY)
	{
		SDL_Log("Success!");
		string path = savePath;
		free(savePath);
		path += ".mvg";
		SDL_Log(path.c_str());
		
		ofstream out(path.c_str(), ios_base::binary);

		//offsets
		out.write((char*)& Canvas::canvas.x, sizeof(Canvas::canvas.x));
		out.write((char*)& Canvas::canvas.y, sizeof(Canvas::canvas.y));
		out.write((char*)& Canvas::canvas.w, sizeof(Canvas::canvas.w));
		out.write((char*)& Canvas::canvas.h, sizeof(Canvas::canvas.h));
		SDL_Log("cax = %d cay = %d caw = %d cah = %d", Canvas::canvas.x, Canvas::canvas.y, Canvas::canvas.w, Canvas::canvas.h);

		//count of obj
		int n = Canvas::v_elements.size();
		out.write((char*)&n, sizeof(n));
		SDL_Log("count = %d", n);

		//objects
		for (int i = 0; i < n; i++)
		{
			Canvas::v_elements[i]->Write(out);
		}

		out.close();
	}
	else if (result == NFD_CANCEL)
	{
		SDL_Log("User pressed cancel.");
		return;
	}
	else
	{
		SDL_Log("Error: %s\n", NFD_GetError());
	}
}

void Window::LoadMVG()
{
	nfdchar_t* outPath = NULL;
	nfdresult_t result = NFD_OpenDialog("mvg", NULL, &outPath);
	if (result == NFD_OKAY)
	{
		canvas.Clear();
		SDL_Log("Success!");
		string path = outPath;
		free(outPath);
		SDL_Log(path.c_str());

		ifstream in(path.c_str(), ios_base::binary);

		//offsets
		in.read((char*)& Canvas::canvas.x, sizeof(Canvas::canvas.x));
		in.read((char*)& Canvas::canvas.y, sizeof(Canvas::canvas.y));
		in.read((char*)& Canvas::canvas.w, sizeof(Canvas::canvas.w));
		in.read((char*)& Canvas::canvas.h, sizeof(Canvas::canvas.h));
		SDL_Log("cax = %d cay = %d caw = %d cah = %d", Canvas::canvas.x, Canvas::canvas.y, Canvas::canvas.w, Canvas::canvas.h);

		//count of obj
		int n;
		in.read((char*)& n, sizeof(n));
		SDL_Log("count = %d", n);

		//objects
		for (int i = 0; i < n; i++)
		{
			int m;
			in.read((char*)& m, sizeof(m));
			SDL_Log("i = %d ID = %d", i, m);
			switch (m)
			{
			case 0:
			{
				Dot* tmp = new Dot;
				tmp->Read(in);
				Canvas::v_elements.push_back(tmp);
				break;
			}
			case 1:
			{
				Dot* tmp = new Line;
				tmp->Read(in);
				Canvas::v_elements.push_back(tmp);
				break;
			}
			case 2:
			{
				Dot* tmp = new Rect;
				tmp->Read(in);
				Canvas::v_elements.push_back(tmp);
				break;
			}
			case 3:
			{
				Dot* tmp = new Ellipse;
				tmp->Read(in);
				Canvas::v_elements.push_back(tmp);
				break;
			}
			}
			
		}

		in.close();
	}
	else if (result == NFD_CANCEL)
	{
		SDL_Log("User pressed cancel.");
		return;
	}
	else
	{
		SDL_Log("Error: %s\n", NFD_GetError());
	}
}

void Window::SaveSVG()
{
	nfdchar_t* savePath = NULL;
	nfdresult_t result = NFD_SaveDialog("svg", NULL, &savePath);
	if (result == NFD_OKAY)
	{
		SDL_Log("Success!");
		string path = savePath;
		free(savePath);
		path += ".svg";
		SDL_Log(path.c_str());

		ofstream out(path.c_str());

		//offsets
		out << "<?xml version=\"1.0\"?>\n";
		out << "<svg width=\"" << Canvas::canvas.w << "\" height=\"" << Canvas::canvas.h << "\" viewBox=\"0 0 " << Canvas::canvas.w << " " << Canvas::canvas.h << "\" xmlns=\"http://www.w3.org/2000/svg\">\n";

		//objects
		for (int i = 0; i < Canvas::v_elements.size(); i++)
		{
			Canvas::v_elements[i]->WriteSVG(out);
		}

		out << "</svg>";

		out.close();
	}
	else if (result == NFD_CANCEL)
	{
		SDL_Log("User pressed cancel.");
		return;
	}
	else
	{
		SDL_Log("Error: %s\n", NFD_GetError());
	}
}