#include "Common.hpp"
#include "./Utilities/AssetManager.hpp"

#include "Game/Scenes/Board.hpp"


//Include scenes
#include "./Game/scenes/RockPaperScissors.hpp"
#include "./Game/Scenes/RussianRoulette/RussianRoulette.hpp"
#include "./Game/Scenes/RandCard/RandCard.hpp"
#include "Game/Scenes/ArmWrestling.hpp"
#include "Game/Scenes/Basket.hpp"
#include "Game/Scenes/FlagGame.hpp"
#include "Game/Scenes/Podium/Podium.hpp"


typedef struct MainData
{
	sf::RenderWindow renderWindow;
	AssetManager assetManager;

	sf::Clock clock;
	SceneHandler scenes;

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


	for (short i = 0; i < 4; i++)
	{
		PlayerData newPlayer;
		newPlayer.m_joystickId = i;
		mainData.gameData.m_playerDataList.push_back(newPlayer);
	}


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


	/*_mainData.scenes.AddScene(*(_mainData.armWrestlingScene = new ArmWrestling()), "ArmWrestling");
	_mainData.scenes.AddScene(*(_mainData.basketScene = new Basket()), "Basket");
	_mainData.scenes.AddScene(*(_mainData.flagGameScene = new FlagGame()), "FlagGame");
	_mainData.scenes.SelectScene("ArmWrestling",false);*/

	// GAME DATA
	_mainData.gameData.m_renderWindow = &_mainData.renderWindow;
	_mainData.gameData.m_assetManager = &_mainData.assetManager;
	

	//_mainData.scenes.AddScene(_mainData.sceneRussianRoulette, "RussianRoulette");
	//_mainData.scenes.AddScene(_mainData.randCard, "RandCard");

	_mainData.scenes.SetTransferedData(&_mainData.gameData);

	_mainData.scenes.AddScene<BaseGame>("Board");
	_mainData.scenes.AddScene<RockPaperScissors>("rockPaperSizor");

	_mainData.scenes.AddScene<Podium>("podium");

	_mainData.scenes.SelectScene("rockPaperSizor", true);
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

