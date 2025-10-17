#include "Common.hpp"


typedef struct MainData
{
	sf::RenderWindow renderWindow;
	sf::Clock clock;
	SceneHandler scenes;
	SceneBase* armWrestlingScene;

	GameData gameData;
} MainData;

Binds* binds = nullptr;


int main(void);

void MainDataLoad(MainData& _mainData);

void PollEvent(MainData& _mainData);
void Update(MainData& _mainData);
void Draw(MainData& _mainData);


int main(void)
{
	StringFormat::Load();
	binds = new Binds();
	random::SetSeedPID();
	
	MainData mainData;
	MainDataLoad(mainData);

	while (mainData.renderWindow.isOpen())
	{
		PollEvent(mainData);
		Update(mainData);
		Draw(mainData);

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::RShift))
		{
			printf("-- MANUAL FREEZE : [    /  ON] --\n");
			while (sf::Keyboard::isKeyPressed(sf::Keyboard::RShift));
			printf("-- MANUAL FREEZE : [OFF /    ] --\n");
		}
	}

	StringFormat::Unload();
	delete binds;
	binds = nullptr;
	return EXIT_SUCCESS;
}

void MainDataLoad(MainData& _mainData)
{
	_mainData.renderWindow.create(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "SFML", sf::Style::Close);
	_mainData.renderWindow.setKeyRepeatEnabled(false);

	_mainData.scenes.AddScene(*(_mainData.armWrestlingScene = new ArmWrestling()), "ArmWrestling");
	_mainData.scenes.SetTransferedData(&_mainData.gameData);

	// GAME DATA
	_mainData.gameData.m_renderWindow = &_mainData.renderWindow;
	


	//

	_mainData.clock.restart();
}


void PollEvent(MainData& _mainData)
{
	sf::Event event;
	
	while (_mainData.renderWindow.pollEvent(event))
	{
		switch (event.type)
		{
			case sf::Event::Closed:
				_mainData.renderWindow.close();
				return;
				break;

			default:
				_mainData.scenes.PollEvent(event);
				break;

		}
	}
}

void Update(MainData& _mainData)
{
	float deltaTime = _mainData.clock.restart().asSeconds();
	//float dtFixed = deltaTime / (1.0f / 60.0f);
	_mainData.scenes.Update(deltaTime);
}

void Draw(MainData& _mainData)
{
	_mainData.renderWindow.clear(sf::Color::Black);
	
	_mainData.scenes.Draw(_mainData.renderWindow);

	_mainData.renderWindow.display();
}