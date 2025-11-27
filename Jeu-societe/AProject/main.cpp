#include "Common.hpp"
#include "./Animation/Graphics.hpp"
#include "./Ui/Button.hpp"

typedef struct MainData
{
	sfMod::RenderWindow renderWindow;
	sf::Clock clock;
	SceneHandler scenes;

	TextureAnimated ta;
	Button btn;

	GameData gameData;
} MainData;


int main(void);

void MainDataLoad(MainData& _mainData);

void PollEvent(MainData& _mainData);
void Update(MainData& _mainData);
void Draw(MainData& _mainData);


int main(void)
{
	random::SetSeedPID();
	randmt::SetSeedPID();

	MainData mainData;
	MainDataLoad(mainData);
	
	mainData.renderWindow.setIcon("./Assets/Images/Placeholder.png");
	mainData.renderWindow.SetFullscreenPrefered(true);


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
	return EXIT_SUCCESS;
}

void MainDataLoad(MainData& _mainData)
{
	_mainData.renderWindow.create(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "SFML", sf::Style::Default);
	_mainData.renderWindow.setKeyRepeatEnabled(false);
	
	_mainData.scenes.SetTransferedData(&_mainData.gameData);

	// GAME DATA
	_mainData.gameData.m_renderWindow = &_mainData.renderWindow;
	

	_mainData.ta.LoadFromFile("./Assets/ButtonPlaceholder.anim", TextureAnimated::ANIMATION_ANIM);
	_mainData.btn.setTexture(_mainData.ta);
	_mainData.btn.setPosition(200.0f, 200.0f);

	_mainData.clock.restart();
}


void PollEvent(MainData& _mainData)
{
	sf::Event event;

	while (_mainData.renderWindow.pollEvent(event))
	{
		_mainData.btn.CheckEvent(event);
		switch (event.type)
		{
			case sf::Event::Closed:
				_mainData.renderWindow.close();
				return;
				break;

			case sf::Event::KeyPressed:
				_mainData.btn.Click();
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
	float dtFixed = deltaTime / (1.0f / 60.0f);


	_mainData.scenes.Update(deltaTime);

	_mainData.btn.Update(deltaTime);
	if (_mainData.btn.HasBeenClicked())
	{
		printf("kys%d\n", random::RandomInt(0,69));
	}
}

void Draw(MainData& _mainData)
{
	_mainData.renderWindow.clear(sf::Color::Black);
	
	_mainData.scenes.Draw(_mainData.renderWindow);
	
	_mainData.renderWindow.draw(_mainData.btn);

	_mainData.renderWindow.display();
}

