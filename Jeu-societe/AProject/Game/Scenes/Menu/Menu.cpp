#include "Menu.hpp"

//3 = 4 players
#define MAX_PLAYERS 3
//1 = 2 players
#define MIN_PLAYERS 1
//Delay to scroll in buttons with controler
#define INPUT_DELAY 0.5f



void Menu::Load(void)
{
	m_data = new SceneData;
	m_data->gameData = (GameData*)this->m_keptData;
	m_data->state = MAIN_MENU;
	m_data->gameData->m_assetManager->LoadManifest("Manifests/Menu.json", "Menu");
	m_data->audio = (AudioEngine*)m_data->gameData->m_audioEngine;	
	LoadUI();
}
void Menu::LoadUI(void)
{
	m_data->ui.buttonMap["playBtn"].setTexture(*m_data->gameData->m_assetManager->GetAsset<TextureAnimated>("playBtn"));
	m_data->ui.buttonMap["settingsBtn"].setTexture(*m_data->gameData->m_assetManager->GetAsset<TextureAnimated>("settingsBtn"));
	m_data->ui.buttonMap["leaveBtn"].setTexture(*m_data->gameData->m_assetManager->GetAsset<TextureAnimated>("leaveBtn"));
	m_data->ui.buttonMap["moinsBtn"].setTexture(*m_data->gameData->m_assetManager->GetAsset<TextureAnimated>("moinsBtn"));
	m_data->ui.buttonMap["plusBtn"].setTexture(*m_data->gameData->m_assetManager->GetAsset<TextureAnimated>("plusBtn"));
	//Background
	m_data->ui.background.setTexture(*m_data->gameData->m_assetManager->GetAsset<sf::Texture>("background"));
	//Game logo
	m_data->ui.logoGame.setTexture(*m_data->gameData->m_assetManager->GetAsset<sf::Texture>("LogoGame"));
	sf::Vector2u logoGameSize = m_data->gameData->m_assetManager->GetAsset<sf::Texture>("LogoGame")->getSize();
	m_data->ui.logoGame.setOrigin(sf::Vector2f( logoGameSize.x / 2.f, 1.f));
	m_data->ui.logoGame.setPosition({ SCREEN_WIDTH / 2, 0 });
	m_data->ui.logoGame.setScale({ 0.8f,0.8f });
	//Crea logo
	m_data->ui.logoCrea.setTexture(*m_data->gameData->m_assetManager->GetAsset<sf::Texture>("LogoCrea"));
	logoGameSize = m_data->gameData->m_assetManager->GetAsset<sf::Texture>("LogoCrea")->getSize();
	m_data->ui.logoCrea.setOrigin(sf::Vector2f( 1.f, logoGameSize.y / 2.f ));
	m_data->ui.logoCrea.setPosition({ 10, SCREEN_HEIGHT / 1.5 });
	m_data->ui.logoCrea.setScale({ 0.3f,0.3f });

	//Icons chara
	m_data->ui.charaAvaible.push_back("Perso1-1");
	m_data->ui.charaAvaible.push_back("Perso2-1");
	m_data->ui.charaAvaible.push_back("Perso3-1");
	m_data->ui.charaAvaible.push_back("Perso4-1");

	m_data->ui.iconsChara.setTexture(*m_data->gameData->m_assetManager->GetAsset<TextureAnimated>("Icone", AssetManager::AssetType::TEXTURE_ANIMATED));
	m_data->ui.iconsChara.SetAnimation("Perso1-1");
	m_data->ui.iconsChara.setOrigin({ 0.5f,0.5f });

	sf::FloatRect buttonRect = m_data->ui.buttonMap["playBtn"].getLocalBounds();
	m_data->ui.buttonMap["playBtn"].setPosition({ SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 });
	m_data->ui.buttonMap["settingsBtn"]. setPosition({ SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + buttonRect.height});
	m_data->ui.buttonMap["leaveBtn"].setPosition({ SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 2 * buttonRect.height });
	m_data->ui.buttonMap["moinsBtn"].setPosition({ SCREEN_WIDTH / 2 - 0.2f * SCREEN_WIDTH, SCREEN_HEIGHT / 2 });
	m_data->ui.buttonMap["plusBtn"].setPosition({ SCREEN_WIDTH / 2 + 0.2f * SCREEN_WIDTH, SCREEN_HEIGHT / 2 });
	
	
	m_data->ui.buttonMap["playBtn"].setOrigin({ 0.5f,0.5f });
	m_data->ui.buttonMap["settingsBtn"].setOrigin({ 0.5f,0.5f });
	m_data->ui.buttonMap["leaveBtn"].setOrigin({ 0.5f,0.5f });
	m_data->ui.buttonMap["moinsBtn"].setOrigin({ 0.5f,0.5f });
	m_data->ui.buttonMap["plusBtn"].setOrigin({ 0.5f,0.5f });


	m_data->ui.playerCount.setFont(*m_data->gameData->m_assetManager->GetAsset<sf::Font>("MenuFont"));
	m_data->ui.playerCount.setCharacterSize(200u);
	m_data->ui.playerCount.setPosition({ SCREEN_WIDTH / 2 , SCREEN_HEIGHT / 2 });
	sf::FloatRect rect = m_data->ui.playerCount.getGlobalBounds();
	//This font sucks bro
	m_data->ui.playerCount.setOrigin({0.6f,0.8f});
	m_data->ui.playerCount.setString(std::to_string(m_data->gameSettings.playerCount + 1));
}
void Menu::Unload(void)
{

}

void Menu::PollEvent(sf::Event& _event)
{
	//Update poll event button
	ButtonsPollEvent(_event);

	//Poll event of controlers
	switch (_event.type)
	{
		//If we press Enter, it's like press button on controler
		//Unusefull if we play only on controler
		case sf::Event::KeyPressed:
			if (_event.key.code == sf::Keyboard::Escape)
			{
				m_data->gameData->m_renderWindow->close();
			}
			else if (_event.key.code != sf::Keyboard::Enter)
			{
				break;
			}
		case sf::Event::JoystickButtonPressed:

			m_data->audio->PlaySound("uiSoundClick");
			PressSelection(_event.joystickButton.joystickId);
			//_event.joystick
			break;

		case sf::Event::JoystickMoved:

			//X Y joystick gauche
			//U V joystick droite
			//Z R pression des gachettes
			//La croix povX povY		
			if (m_data->inputDelay > INPUT_DELAY)
			{
				switch (_event.joystickMove.axis)
				{
					//All directions
					case sf::Joystick::Axis::X:
					case sf::Joystick::Axis::Y:
					case sf::Joystick::Axis::U:
					case sf::Joystick::Axis::V:

						std::cout << "jostick : " << _event.joystickMove.position << "btn : " << m_data->controlerBtn << std::endl;

						if (_event.joystickMove.position > 0)
						{
							ChangeSelection(1, _event.joystickMove.joystickId);
							m_data->inputDelay = 0.f;
						}
						else if (_event.joystickMove.position < 0)
						{
							ChangeSelection(-1, _event.joystickMove.joystickId);
							m_data->inputDelay = 0.f;
						}
						break;

					default:
						break;
				}
			}
			break;

		default:
			break;
	}
}
void Menu::ButtonsPollEvent(sf::Event& _event)
{
	switch (m_data->state)
	{
		case MAIN_MENU:
			m_data->ui.buttonMap["playBtn"].PollEvent(_event);
			m_data->ui.buttonMap["settingsBtn"].PollEvent(_event);
			m_data->ui.buttonMap["leaveBtn"].PollEvent(_event);
			break;

		case OPTIONS:
			break;

		case PLAYER_NB_SELECTION:
			m_data->ui.buttonMap["playBtn"].PollEvent(_event);
			m_data->ui.buttonMap["plusBtn"].PollEvent(_event);
			m_data->ui.buttonMap["moinsBtn"].PollEvent(_event);
			break;
	}
}

void Menu::Update(float _deltaTime)
{
	ButtonsUpdate(_deltaTime);
	
	//std::cout << "current chara = " << m_data->currentCharaSelected << std::endl;
	//std::cout << "current state = " << m_data->state << std::endl;
	//Update timer for delay between input
	m_data->inputDelay += _deltaTime;
}
void Menu::ButtonsUpdate(float _dt)
{
	switch (m_data->state)
	{
		case MAIN_MENU:
			m_data->ui.buttonMap["playBtn"].Update(_dt);
			m_data->ui.buttonMap["settingsBtn"].Update(_dt);
			m_data->ui.buttonMap["leaveBtn"].Update(_dt);
			break;

		case OPTIONS:
			break;

		case PLAYER_NB_SELECTION:
			m_data->ui.buttonMap["playBtn"].Update(_dt);
			m_data->ui.buttonMap["plusBtn"].Update(_dt);
			m_data->ui.buttonMap["moinsBtn"].Update(_dt);
			break;
	}
}

void Menu::Draw(sf::RenderWindow& _renderWindow)
{
	_renderWindow.draw(m_data->ui.background);
	DrawUI(_renderWindow);
}
void Menu::DrawUI(sf::RenderWindow& _renderWindow)
{
	sf::Vector2i mousePos = sf::Mouse::getPosition();
	switch (m_data->state)
	{
		case MAIN_MENU:
			_renderWindow.draw(m_data->ui.buttonMap["playBtn"]);
			_renderWindow.draw(m_data->ui.buttonMap["settingsBtn"]);
			_renderWindow.draw(m_data->ui.buttonMap["leaveBtn"]);
			_renderWindow.draw(m_data->ui.logoGame);
			_renderWindow.draw(m_data->ui.logoCrea);

			if (m_data->ui.buttonMap["leaveBtn"].IsClicked(mousePos.x, mousePos.y))
			{
				_renderWindow.close();
			}
			break;

		case OPTIONS:
			break;

		case PLAYER_NB_SELECTION:

			_renderWindow.draw(m_data->ui.buttonMap["playBtn"]);
			_renderWindow.draw(m_data->ui.buttonMap["moinsBtn"]);
			_renderWindow.draw(m_data->ui.playerCount);
			_renderWindow.draw(m_data->ui.buttonMap["plusBtn"]);

			break;
		case PLAYER_SELECTION:

			PrintIcons(_renderWindow);

			break;
	}
}

void Menu::ChangeSelection(int _value, int _joystick)
{
	sf::Vector2f mouseNewPos;

	m_data->audio->PlaySound("uiSoundON");


	switch (m_data->state)
	{
		case MAIN_MENU:
			
			if ((m_data->controlerBtn + _value) < PLAY)
			{
				m_data->controlerBtn = LEAVE;
				mouseNewPos = m_data->ui.buttonMap["leaveBtn"].getPosition();
			}
			else if ((m_data->controlerBtn + _value) > LEAVE)
			{
				m_data->controlerBtn = PLAY;
				mouseNewPos = m_data->ui.buttonMap["playBtn"].getPosition();
			}
			else
			{
				m_data->controlerBtn = (ControlerCurrentButton)(m_data->controlerBtn + _value);
				
				//Ok, racism
				switch (m_data->controlerBtn)
				{
					case PLAY:
						mouseNewPos = m_data->ui.buttonMap["playBtn"].getPosition();
						break;

					case LEAVE:
						mouseNewPos = m_data->ui.buttonMap["leaveBtn"].getPosition();
						break;
				
					case SETTINGS:
						mouseNewPos = m_data->ui.buttonMap["settingsBtn"].getPosition();
						break;
				}				
			}
			break;

		case OPTIONS:
			break;

		case PLAYER_NB_SELECTION:

			if ((m_data->controlerBtn + _value) > MORE)
			{
				m_data->controlerBtn = LESS;
				mouseNewPos = m_data->ui.buttonMap["moinsBtn"].getPosition();
			}
			else if ((m_data->controlerBtn + _value) < LESS)
			{
				m_data->controlerBtn = MORE;
				mouseNewPos = m_data->ui.buttonMap["plusBtn"].getPosition();
			}
			else
			{
				m_data->controlerBtn = (ControlerCurrentButton)(m_data->controlerBtn + _value);

				//Ok, racism
				switch (m_data->controlerBtn)
				{
					case MORE:
						mouseNewPos = m_data->ui.buttonMap["plusBtn"].getPosition();
						break;

					case LESS:
						mouseNewPos = m_data->ui.buttonMap["moinsBtn"].getPosition();
						break;

					case PLAY_SELECTION:
						mouseNewPos = m_data->ui.buttonMap["playBtn"].getPosition();
						break;
				}
			}
			break;

		case PLAYER_SELECTION:
			
			if (m_data->charaSelected[_joystick] == false)
			{
				if ((m_data->currentCharaSelected[_joystick] + _value) > m_data->ui.charaAvaible.size()-1)
				{
					m_data->currentCharaSelected[_joystick] = 0;
					//mouseNewPos = m_data->ui.buttonMap["moinsBtn"].getPosition();
				}
				else if ((m_data->controlerBtn + _value) < 0)
				{
					m_data->currentCharaSelected[_joystick] = m_data->ui.charaAvaible.size() - 1;
					//mouseNewPos = m_data->ui.buttonMap["plusBtn"].getPosition();
				}
				else
				{
					m_data->currentCharaSelected[_joystick] += _value;
				}
			}


			break;
	}
	//Set ON new button
	//std::cout << "mouse x = " << mouseNewPos.x << " y = " << mouseNewPos.y << std::endl;
	sf::Mouse::setPosition(sf::Vector2i(mouseNewPos), *m_data->gameData->m_renderWindow);
}

void Menu::PressSelection(int _id)
{
	m_data->audio->PlaySound("uiSoundClick");
	switch (m_data->controlerBtn)
	{
		case PLAY:
			//m_data->state = (MenuState)(m_data->state + 1);

			switch (m_data->state)
			{
				case MAIN_MENU:

					m_data->state = PLAYER_NB_SELECTION;
					m_data->controlerBtn = PLAY_SELECTION;
					m_data->ui.buttonMap["playBtn"].setPosition({ SCREEN_WIDTH / 2, SCREEN_HEIGHT / 1.5 });
					break;

				case PLAYER_NB_SELECTION:

					break;
				case PLAYER_SELECTION:

					std::cout << "id = " << _id << " size of datalist = " << m_data->gameData->m_playerDataList.size();
					m_data->gameData->m_playerDataList[_id].SetPlayerSkin((PlayerData::PlayerSkin)m_data->currentCharaSelected[_id]);
					m_data->charaSelected[_id] = true;

					//if (_id == m_data->currentPlayer)
					//{
					//}

					//if (_id == m_data->gameData->m_playerDataList.size() - 1)
					//{
					//	std::cout << "leave\n";
					//	SceneBase::ChangeScene("Board");
					//}
					int result = 0;
					for (auto selected : m_data->charaSelected)
					{
						result += (int)selected;
					}
					if (result == 4)
					{
						std::cout << "All players have their skin, go to game\n";
						SceneBase::ChangeScene("Board");

					}
					//if (_id == m_data->gameData->m_playerDataList.size() - 1)
					//{
					//	std::cout << "leave\n";
					//	SceneBase::ChangeScene("Board");
					//}

					//m_data->currentPlayer++;
					break;  
			}

			break;

		case SETTINGS:
			//Bruh i need render window to exit, here i cant
			break;

		case LEAVE:
			m_data->gameData->m_renderWindow->close();
			break;

		case LESS:
			if (m_data->gameSettings.playerCount > MIN_PLAYERS)
			{
				m_data->gameSettings.playerCount -= 1;
				m_data->ui.playerCount.setString(std::to_string(m_data->gameSettings.playerCount + 1));
			}
			break;

		case PLAY_SELECTION:
		{
			std::cout << "player count : " << m_data->gameSettings.playerCount;
			for (int i = 0; i < m_data->gameSettings.playerCount + 1; i++)
			{
				PlayerData newPlayer;
				newPlayer.m_joystickId = i;
				m_data->gameData->m_playerDataList.push_back(newPlayer);
				m_data->charaSelected.push_back(false);
				m_data->currentCharaSelected.push_back(0);
			}
			m_data->state = PLAYER_SELECTION;
			m_data->controlerBtn = PLAY;
			break;
		}		
		case MORE:
			if (m_data->gameSettings.playerCount < MAX_PLAYERS)
			{
				m_data->gameSettings.playerCount += 1;
				m_data->ui.playerCount.setString(std::to_string(m_data->gameSettings.playerCount + 1));
			}
			break;
	}
}

void Menu::PrintIcons(sf::RenderWindow& _renderWindow)
{
	sf::Color tempColor = m_data->ui.iconsChara.getColor();


	//ON S EN BRANLE, TOUT LE MONDE PEUT JOUER TOUT LES PERSO
	//Only one character available
	//if (m_data->ui.charaAvaible.size() == 1)
	//{
	//	//Center
	//	m_data->ui.iconsChara.SetAnimation(m_data->ui.charaAvaible[0]);
	//	m_data->ui.iconsChara.setPosition(iconPos[1]);
	//	_renderWindow.draw(m_data->ui.iconsChara);
	//}
	////2 characters available : one in center, one on left or right
	//else if(m_data->ui.charaAvaible.size() == 2)
	//{	
	//	//First chara
	//	if (m_data->currentCharaSelected == 0)
	//	{
	//		//Center
	//		m_data->ui.iconsChara.SetAnimation(m_data->ui.charaAvaible[0]);
	//		m_data->ui.iconsChara.setPosition(iconPos[1]);
	//		_renderWindow.draw(m_data->ui.iconsChara);

	//		//Right
	//		tempColor.a = 100;
	//		m_data->ui.iconsChara.setColor(tempColor);
	//		m_data->ui.iconsChara.SetAnimation(m_data->ui.charaAvaible[1]);
	//		m_data->ui.iconsChara.setPosition(iconPos[2]);
	//		_renderWindow.draw(m_data->ui.iconsChara);
	//	}
	//	//End chara
	//	else
	//	{
	//		//Center
	//		m_data->ui.iconsChara.SetAnimation(m_data->ui.charaAvaible[0]);
	//		m_data->ui.iconsChara.setPosition(iconPos[1]);
	//		_renderWindow.draw(m_data->ui.iconsChara);

	//		//Left
	//		tempColor.a = 100;
	//		m_data->ui.iconsChara.setColor(tempColor);
	//		m_data->ui.iconsChara.SetAnimation(m_data->ui.charaAvaible[1]);
	//		m_data->ui.iconsChara.setPosition(iconPos[0]);
	//		_renderWindow.draw(m_data->ui.iconsChara);
	//	}
	//}
	////Print 3 characters
	//else

	
	//Same usage of randcard printcard()
	//std::cout << "player count = " << m_data->gameSettings.playerCount << std::endl;
	//float iconSpacing = (SCREEN_WIDTH) / (m_data->gameSettings.playerCount + 1);
	float iconSpacing = (SCREEN_WIDTH - 2 * 100.f) / m_data->gameSettings.playerCount;


	//std::cout << " players : " << m_data->gameSettings.playerCount << std::endl;

	for (int i = 0; i < m_data->gameSettings.playerCount + 1; i++)
	{
		sf::Vector2f iconPos[] =
		{
			{(float)(100.f + i * iconSpacing), SCREEN_HEIGHT / 2.f - SCREEN_HEIGHT / 4.f},
			{(float)(100.f + i * iconSpacing), SCREEN_HEIGHT / 2.f },
			{(float)(100.f + i * iconSpacing), SCREEN_HEIGHT / 2.f + SCREEN_HEIGHT / 4.f}
		};


		//std::cout << "icon pos x = " << iconPos[1].x << std::endl;
		//std::cout << "i = " << i << std::endl;


		//First chara
		if (m_data->currentCharaSelected[i] == 0)
		{
			//Left
			tempColor.a = 100;
			m_data->ui.iconsChara.setColor(tempColor);
			m_data->ui.iconsChara.SetAnimation(m_data->ui.charaAvaible[m_data->ui.charaAvaible.size() - 1]);
			m_data->ui.iconsChara.setPosition(iconPos[0]);
			_renderWindow.draw(m_data->ui.iconsChara);
			//Center
			tempColor.a = 255;
			if (m_data->charaSelected[i] == true)
			{
				//m_data->ui.iconsChara.setColor(sf::Color(20, 20, 20, tempColor.a));
				m_data->ui.iconsChara.setColor(sf::Color(0, 0, 0, 255));

			}
			else
			{
				m_data->ui.iconsChara.setColor(tempColor);
			}
			m_data->ui.iconsChara.SetAnimation(m_data->ui.charaAvaible[m_data->currentCharaSelected[i]]);
			m_data->ui.iconsChara.setPosition(iconPos[1]);
			_renderWindow.draw(m_data->ui.iconsChara);
			//Right
			tempColor.a = 100;
			m_data->ui.iconsChara.setColor(tempColor);
			m_data->ui.iconsChara.SetAnimation(m_data->ui.charaAvaible[m_data->currentCharaSelected[i] + 1]);
			m_data->ui.iconsChara.setPosition(iconPos[2]);
			_renderWindow.draw(m_data->ui.iconsChara);
		}
		//End chara
		else if (m_data->currentCharaSelected[i] == m_data->ui.charaAvaible.size() - 1)
		{
			//Left
			tempColor.a = 100;
			m_data->ui.iconsChara.setColor(tempColor);
			m_data->ui.iconsChara.SetAnimation(m_data->ui.charaAvaible[m_data->currentCharaSelected[i] - 1]);
			m_data->ui.iconsChara.setPosition(iconPos[0]);
			_renderWindow.draw(m_data->ui.iconsChara);
			//Center
			tempColor.a = 255;
			if (m_data->charaSelected[i] == true)
			{
				//m_data->ui.iconsChara.setColor(sf::Color(20, 20, 20, tempColor.a));
				m_data->ui.iconsChara.setColor(sf::Color(0, 0, 0, 255));
			}
			else
			{
				m_data->ui.iconsChara.setColor(tempColor);
			}
			m_data->ui.iconsChara.setColor(tempColor);
			m_data->ui.iconsChara.SetAnimation(m_data->ui.charaAvaible[m_data->currentCharaSelected[i]]);
			m_data->ui.iconsChara.setPosition(iconPos[1]);
			_renderWindow.draw(m_data->ui.iconsChara);
			//Right
			tempColor.a = 100;
			m_data->ui.iconsChara.setColor(tempColor);
			m_data->ui.iconsChara.SetAnimation(m_data->ui.charaAvaible[0]);
			m_data->ui.iconsChara.setPosition(iconPos[2]);
			_renderWindow.draw(m_data->ui.iconsChara);
		}
		//Normal (3 chara print)
		else
		{
			//Left
			tempColor.a = 100;
			m_data->ui.iconsChara.setColor(tempColor);
			m_data->ui.iconsChara.SetAnimation(m_data->ui.charaAvaible[m_data->currentCharaSelected[i] - 1]);
			m_data->ui.iconsChara.setPosition(iconPos[0]);
			_renderWindow.draw(m_data->ui.iconsChara);
			//Center
			tempColor.a = 255;
			if (m_data->charaSelected[i] == true)
			{
				//m_data->ui.iconsChara.setColor(sf::Color(20, 20, 20, tempColor.a));
				m_data->ui.iconsChara.setColor(sf::Color(0, 0, 0, 255));

			}
			else
			{
				m_data->ui.iconsChara.setColor(tempColor);
			}
			m_data->ui.iconsChara.setColor(tempColor);
			m_data->ui.iconsChara.SetAnimation(m_data->ui.charaAvaible[m_data->currentCharaSelected[i]]);
			m_data->ui.iconsChara.setPosition(iconPos[1]);
			_renderWindow.draw(m_data->ui.iconsChara);
			//Right
			tempColor.a = 100;
			m_data->ui.iconsChara.setColor(tempColor);
			m_data->ui.iconsChara.SetAnimation(m_data->ui.charaAvaible[m_data->currentCharaSelected[i] + 1]);
			m_data->ui.iconsChara.setPosition(iconPos[2]);
			_renderWindow.draw(m_data->ui.iconsChara);
		}
	}


	m_data->ui.iconsChara.setColor(sf::Color(255,255,255,255));
}