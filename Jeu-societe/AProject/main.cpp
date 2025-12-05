#include "Common.hpp"

#include "Game/Scenes/Board/Board.hpp"
#include "Game/Scenes/Loading/LoadingScreen.hpp"


#include "./Game/scenes/RockPaperScissors/RockPaperScissors.hpp"
#include "Game/Scenes/ArmWrestling/ArmWrestling.hpp"
#include "Game/Scenes/Basket/Basket.hpp"


#include "./Game/Scenes/RussianRoulette/RussianRoulette.hpp"
#include "./Game/Scenes/RandCard/RandCard.hpp"
#include "Game/Scenes/FlagGame/FlagGame.hpp"

#include "./Game/Scenes/Podium/Podium.hpp"

typedef struct MainData
{
	sfMod::RenderWindow renderWindow;
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
	random::SetSeedPID();
	randmt::SetSeedPID();

	binds = new Binds();

	MainData mainData;


	mainData.gameData.m_playerDataList.resize(4);
	for (short i = 0; i < mainData.gameData.m_playerDataList.size(); i++)
	{
		mainData.gameData.m_playerDataList[i].SetJoystickID(i);
		mainData.gameData.m_playerDataList[i].SetPlayerSkin((PlayerData::PlayerSkin)(i % 8));
	}


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

	delete binds;
	binds = nullptr;
	
	return EXIT_SUCCESS;
}

void MainDataLoad(MainData& _mainData)
{
	_mainData.renderWindow.create(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "SFML", sf::Style::Default);

	_mainData.renderWindow.setKeyRepeatEnabled(false);

	// GAME DATA
	_mainData.gameData.m_renderWindow = &_mainData.renderWindow;
	_mainData.gameData.m_assetManager = &_mainData.assetManager;

	_mainData.assetManager.LoadManifest("Manifests/Main.json", "main");

	_mainData.scenes.SetTransferedData(&_mainData.gameData);

	// Ajouter les scènes
	_mainData.scenes.AddScene<LoadingScreen>("Lo");
	_mainData.scenes.AddScene<BaseGame>("Board");
	_mainData.scenes.AddScene<RockPaperScissors>("rockPaperSizor");
	_mainData.scenes.AddScene<ArmWrestling>("ArmWrestling");
	_mainData.scenes.AddScene<Basket>("Basket");
	_mainData.scenes.AddScene<FlagGame>("FlagGame");
	_mainData.scenes.AddScene<RandCard>("RandCard");
	_mainData.scenes.AddScene<RussianRoulette>("RuRoul");
	_mainData.scenes.AddScene<Podium>("Podium");

	// Sélectionner le LoadingScreen metre false pour tout scene au debut 
	_mainData.scenes.SelectScene("ArmWrestling", false);

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

	_mainData.scenes.Update(deltaTime);
}

void Draw(MainData& _mainData)
{
	_mainData.renderWindow.clear(sf::Color::Black);
	
	_mainData.scenes.Draw(_mainData.renderWindow);

	_mainData.renderWindow.display();
}

