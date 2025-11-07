#include "./Common.hpp"

#include "./Game/Scenes/Board.hpp"

#include "./Game/scenes/RockPaperScissors.hpp"

#include "./Game/Scenes/RussianRoulette/RussianRoulette.hpp"
#include "./Game/Scenes/RandCard/RandCard.hpp"

#include "./Game/Scenes/ArmWrestling.hpp"
#include "./Game/Scenes/Basket.hpp"
#include "./Game/Scenes/FlagGame.hpp"


typedef struct MainData
{
	sf::RenderWindow renderWindow;
	sf::Clock clock;
	SceneHandler scenes;

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

	return EXIT_SUCCESS;
}

void MainDataLoad(MainData& _mainData)
{
	_mainData.renderWindow.create(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "SFML", sf::Style::Close);
	_mainData.renderWindow.setKeyRepeatEnabled(false);

	_mainData.scenes.SetTransferedData(&_mainData.gameData);

	// GAME DATA
	_mainData.gameData.m_renderWindow = &_mainData.renderWindow;
	
	_mainData.scenes.AddScene<RandCard>("RandCard");
	_mainData.scenes.AddScene<BaseGame>("Board");
	_mainData.scenes.AddScene<RussianRoulette>("RussianRoulette");
	_mainData.scenes.AddScene<FlagGame>("FlagGame");
	_mainData.scenes.AddScene<ArmWrestling>("ArmWrestling");
	_mainData.scenes.AddScene<Basket>("Basket");
	_mainData.scenes.AddScene<RockPaperScissors>("RockPaperScissors");

	

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

