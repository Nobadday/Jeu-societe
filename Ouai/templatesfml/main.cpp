#include "Common.hpp"
#include "GameState.hpp"

#pragma region Struct
typedef struct MainData
{
	sf::RenderWindow* renderWindow;
	sf::Clock* clock;
}MainData;

#pragma endregion

Binds* binds = new Binds();

void Load(MainData* const _mainData);
void PollEvent(sf::RenderWindow* const _renderWindow);
void Update(MainData* const _mainData);
void Draw(sf::RenderWindow* const _renderWindow);
void CleanUp(MainData* const _mainData);
void LoadMainData(MainData* const _mainData);
void CleanUpMainData(MainData* const _mainData);
sf::RenderWindow* CreateRenderWindow(usi _frameRate, bool _reapetInput, bool _vsync);
void KeyPressed(sf::Event::KeyEvent _key, sf::RenderWindow* _renderWindow);
void KeyReleased(sf::Event::KeyEvent _key, sf::RenderWindow* _renderWindow);
void MousePressed(sf::Event::MouseButtonEvent _mouse, sf::RenderWindow* _renderWindow);
void MouseReleased(sf::Event::MouseButtonEvent _mouse, sf::RenderWindow* _renderWindow);
void MouseMoved(sf::Vector2i _mouse, sf::RenderWindow* _renderWindow);

MainData mainData = { 0 };

int main(void)
{

	Load(&mainData);
	if (mainData.renderWindow == NULL)
	{
		return EXIT_FAILURE;
	}
	while (mainData.renderWindow->isOpen())
	{
		PollEvent(mainData.renderWindow);
		Update(&mainData);
		Draw(mainData.renderWindow);
	}
	CleanUp(&mainData);
	return EXIT_SUCCESS;
}

void Load(MainData* const _mainData)
{
	srand(_getpid());
	LoadMainData(_mainData);
	SetGameState(MENU);
}

void PollEvent(sf::RenderWindow* const _renderWindow)
{
	sf::Event event;
	while (_renderWindow->pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			_renderWindow->close();
		}
		else if (event.type == sf::Event::KeyPressed)
		{
			KeyPressed(event.key, _renderWindow);
		}
		else if (event.type == sf::Event::KeyReleased)
		{
			KeyReleased(event.key, _renderWindow);
		}
		else if (event.type == sf::Event::MouseButtonPressed)
		{
			MousePressed(event.mouseButton, _renderWindow);
		}
		else if (event.type == sf::Event::MouseButtonReleased)
		{
			MouseReleased(event.mouseButton, _renderWindow);
		}
		else if (event.type == sf::Event::MouseMoved)
		{
			sf::Vector2i mousePos(event.mouseMove.x, event.mouseMove.y);
			MouseMoved(mousePos, _renderWindow);
		}
	}
}

void Update(MainData* const _mainData)
{
	float dt = _mainData->clock->restart().asSeconds();
	Binds::UpdateBind(dt);
	switch (GetGameState())
	{
	case MENU:
		Menu::Update(dt);
		break;
	case GAME:
		Game::Update(dt);
		break;
	case GAME_OVER:
		GameOver::Update(dt);
		break;
	default:
		break;
	}
}

void Draw(sf::RenderWindow* const _renderWindow)
{
	_renderWindow->clear(sf::Color::Black);
	switch (GetGameState())
	{
	case MENU:
		Menu::Draw(_renderWindow);
		break;
	case GAME:
		Game::Draw(_renderWindow);
		break;
	case GAME_OVER:
		GameOver::Draw(_renderWindow);
		break;
	}

	_renderWindow->display();
}

void CleanUp(MainData* const _mainData)
{
	CleanUpMainData(_mainData);
}

void LoadMainData(MainData* const _mainData)
{
	_mainData->clock = new sf::Clock();
	_mainData->renderWindow = CreateRenderWindow(60, true, true);

}

void CleanUpMainData(MainData* const _mainData)
{
	delete binds;
	binds = NULL;
	delete _mainData->clock;
	delete _mainData->renderWindow;
	CleanupGameState();
}

sf::RenderWindow* CreateRenderWindow(usi _frameRate, bool _reapetInput, bool _vsync)
{
	sf::Vector2u size = { SCREEN_WIDTH, SCREEN_HEIGHT };
	sf::VideoMode videoMode = sf::VideoMode(size.x,size.y, BPP);
	sf::RenderWindow* renderWindow = new sf::RenderWindow(videoMode, "Cursed Soul", sf::Style::Default);
	renderWindow->setFramerateLimit(_frameRate);
	renderWindow->setKeyRepeatEnabled(_reapetInput);
	renderWindow->setVerticalSyncEnabled(_vsync);

	return renderWindow;
}

void KeyPressed(sf::Event::KeyEvent _key, sf::RenderWindow* _renderWindow)
{
	switch (GetGameState())
	{
	case MENU:
		Menu::KeyPressed(_key, _renderWindow);
		break;
	case GAME:
		Game::KeyPressed(_key, _renderWindow);
		break;
	case GAME_OVER:
		GameOver::KeyPressed(_key, _renderWindow);
		break;
	default:
		break;
	}
}
void KeyReleased(sf::Event::KeyEvent _key, sf::RenderWindow* _renderWindow)
{
	switch (GetGameState())
	{
	case MENU:
		Menu::KeyReleased(_key, _renderWindow);
		break;
	case GAME:
		Game::KeyReleased(_key, _renderWindow);
		break;
	case GAME_OVER:
		GameOver::KeyReleased(_key, _renderWindow);
		break;
	default:
		break;
	}
}

void MousePressed(sf::Event::MouseButtonEvent _mouse, sf::RenderWindow* _renderWindow)
{
	switch (_mouse.button)
	{
	default:
		break;
	}
}

void MouseReleased(sf::Event::MouseButtonEvent _mouse, sf::RenderWindow* _renderWindow)
{
	switch (_mouse.button)
	{
	default:
		break;
	}
}

void MouseMoved(sf::Vector2i _mouse, sf::RenderWindow* _renderWindow)
{

}
