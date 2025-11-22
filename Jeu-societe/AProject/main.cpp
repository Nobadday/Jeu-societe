#include "Common.hpp"

#include "Game/Scenes/Board/Board.hpp"

// passage de données entre les scènes fait ici 
#include "./Game/scenes/RockPaperScissors/RockPaperScissors.hpp"
#include "Game/Scenes/ArmWrestling/ArmWrestling.hpp"
#include "Game/Scenes/Basket/Basket.hpp"
#include "./Game/Scenes/RussianRoulette/RussianRoulette.hpp"
#include "./Game/Scenes/RandCard/RandCard.hpp"

#include "Game/Scenes/FlagGame/FlagGame.hpp"
//Pue la merde (faut un bon menu systeme)
#include "Game/Scenes/Menu/Menu.hpp"




typedef struct MainData
{
	sf::RenderWindow renderWindow;
	AssetManager assetManager;
	AudioEngine audioEngine;


	sf::Clock clock;
	SceneHandler scenes;

	RussianRoulette sceneRussianRoulette;
	RandCard randCard;

	SceneBase* armWrestlingScene;
	SceneBase* basketScene;
	SceneBase* flagGameScene;

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

	mainData.gameData.m_playerDataList.resize(2);
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
	_mainData.renderWindow.create(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "SFML", sf::Style::Close);
	_mainData.renderWindow.setKeyRepeatEnabled(false);

	// GAME DATA
	_mainData.gameData.m_renderWindow = &_mainData.renderWindow;
	_mainData.gameData.m_assetManager = &_mainData.assetManager;
	_mainData.gameData.m_audioEngine = &_mainData.audioEngine;
	_mainData.audioEngine.SetAssetManager(_mainData.assetManager);

	_mainData.assetManager.LoadManifest("Manifests/Main.json", "main");

	_mainData.scenes.SetTransferedData(&_mainData.gameData);

	_mainData.scenes.AddScene<BaseGame>("Board");
	_mainData.scenes.AddScene<RockPaperScissors>("rockPaperSizor");
	_mainData.scenes.AddScene<ArmWrestling>("ArmWrestling");
	_mainData.scenes.AddScene<Basket>("Basket");
	_mainData.scenes.AddScene<FlagGame>("FlagGame");
	_mainData.scenes.AddScene<RandCard>("RandCard");
	_mainData.scenes.AddScene<RussianRoulette>("RuRoul");

	//_mainData.scenes.SelectScene("RandCard", true);


	_mainData.scenes.SetTransferedData(&_mainData.gameData);

	_mainData.scenes.AddScene<BaseGame>("Board");
	_mainData.scenes.AddScene<RockPaperScissors>("rockPaperSizor");
	_mainData.scenes.AddScene<ArmWrestling>("ArmWrestling");
	_mainData.scenes.AddScene<Basket>("Basket");
	_mainData.scenes.AddScene<FlagGame>("FlagGame");
	_mainData.scenes.AddScene<RandCard>("RandCard");
	_mainData.scenes.AddScene<RussianRoulette>("RuRoul");
	_mainData.scenes.AddScene<Menu>("Menu");

	//_mainData.scenes.SelectScene("Board", true);
	//_mainData.scenes.SelectScene("Menu", true);
	 _mainData.scenes.SelectScene("RuRoul", true);
	//_mainData.scenes.SelectScene("RandCard", true);

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

