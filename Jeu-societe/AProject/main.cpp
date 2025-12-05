#include "Common.hpp"

#include "Game/Scenes/Board/Board.hpp"


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
	_mainData.renderWindow.create(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Cute & Cursed", sf::Style::Default);
	_mainData.renderWindow.SetFullscreenPrefered(true);
	_mainData.renderWindow.setKeyRepeatEnabled(false);

	// GAME DATA
	_mainData.gameData.m_renderWindow = &_mainData.renderWindow;
	_mainData.gameData.m_assetManager = &_mainData.assetManager;


	_mainData.assetManager.LoadManifest("Manifests/Main.json", "main");

	_mainData.scenes.SetTransferedData(&_mainData.gameData);

	_mainData.scenes.AddScene<BaseGame>("Board");
	_mainData.scenes.AddScene<RockPaperScissors>("rockPaperSizor");
	_mainData.scenes.AddScene<ArmWrestling>("ArmWrestling");
	_mainData.scenes.AddScene<Basket>("Basket");
	_mainData.scenes.AddScene<FlagGame>("FlagGame");
	_mainData.scenes.AddScene<RandCard>("RandCard");
	_mainData.scenes.AddScene<RussianRoulette>("RuRoul");
	_mainData.scenes.AddScene<Podium>("Podium");

	_mainData.scenes.SelectScene("ArmWrestling", true);

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
			//case sf::Event::JoystickDisconnected:
			//	std::cout << " disconnected (joystick ID " << event.joystickConnect.joystickId << std::endl;
			//	for (short i = 0; i < _mainData.gameData.m_playerDataList.size(); i++)
			//	{
			//		if (_mainData.gameData.m_playerDataList[i].GetJoystickId() == event.joystickConnect.joystickId)
			//		{
			//			std::cout << "Joueur " << i + 1 << " disconnected (joystick ID " << event.joystickConnect.joystickId << "), changement de son id a -1" << std::endl;
			//			//_mainData.gameData.m_playerDataList[i].SetJoystickID(-1);
			//		}
			//		else if (_mainData.gameData.m_playerDataList[i].GetJoystickId() > event.joystickConnect.joystickId)
			//		{
			//			//std::cout << "changement ID pour joueur " << i << " de Id: " << _mainData.gameData.m_playerDataList[i].GetJoystickId() << " a ID :" << _mainData.gameData.m_playerDataList[i].GetJoystickId() -1 << std::endl;
			//			//_mainData.gameData.m_playerDataList[i].SetJoystickID(_mainData.gameData.m_playerDataList[i].GetJoystickId() - 1);
			//		}
			//	}

			//	std::cout << std::endl << std::endl;
			//	for (short i = 0; i < _mainData.gameData.m_playerDataList.size(); i++)
			//	{
			//		std::cout << "Player " << i + 1 << ": id Manette : " << _mainData.gameData.m_playerDataList[i].GetJoystickId() << std::endl;
			//	}
			//	std::cout << std::endl;


			//	break;

			//case sf::Event::JoystickConnected:
			//{
			//	bool isAlreadyConnected = false;

			//	std::cout << "Manette connectee avec ID : " << event.joystickConnect.joystickId << std::endl;
			//	for (short i = 0; i < _mainData.gameData.m_playerDataList.size(); i++)
			//	{
			//		if (_mainData.gameData.m_playerDataList[i].GetJoystickId() == event.joystickConnect.joystickId)
			//		{
			//			std::cout << "Mannette deja pairee avec un joueur" << std::endl;
			//			isAlreadyConnected = true;
			//		}
			//	}

			//	for (short i = 0; i < _mainData.gameData.m_playerDataList.size(); i++)
			//	{
			//		if (!isAlreadyConnected)
			//		{
			//			if (_mainData.gameData.m_playerDataList[i].GetJoystickId() == -1)
			//			{
			//				std::cout << "Reassignation de la manette avec ID : " << event.joystickConnect.joystickId << " au joueur " << i << std::endl;
			//				_mainData.gameData.m_playerDataList[i].SetJoystickID(event.joystickConnect.joystickId);
			//				isAlreadyConnected = true;
			//			}
			//		}
			//	}

			//	std::cout << std::endl << std::endl;
			//	for (short i = 0; i < _mainData.gameData.m_playerDataList.size(); i++)
			//	{
			//		std::cout << "Player " << i + 1 << ": id Manette : " << _mainData.gameData.m_playerDataList[i].GetJoystickId() << std::endl;
			//	}
			//	std::cout << std::endl;
			//}
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

	//

	//for (short i = 0; i < _mainData.gameData.m_playerDataList.size(); i++)
	//{
	//	if (sf::Joystick::isConnected(_mainData.gameData.m_playerDataList[i].GetJoystickId()))
	//	{
	//		std::cout << "Joystick id : " << _mainData.gameData.m_playerDataList[i].GetJoystickId() << " is connected" <<  std::endl;
	//	}
	//	else
	//	{
	//		std::cout << "Joystick id : " << _mainData.gameData.m_playerDataList[i].GetJoystickId() << " is not connected" << std::endl;
	//	}
	//}
	//float dtFixed = deltaTime / (1.0f / 60.0f);
	_mainData.scenes.Update(deltaTime);
}

void Draw(MainData& _mainData)
{
	_mainData.renderWindow.clear(sf::Color::Black);
	
	_mainData.scenes.Draw(_mainData.renderWindow);

	_mainData.renderWindow.display();
}

