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
	m_data->audio->PlayMusic("Music1", true);
	m_data->audio->SetMusicVolume(25.f);
}
void Menu::LoadUI(void)
{
	m_data->ui.buttonMap["playBtn"].setTexture(*m_data->gameData->m_assetManager->GetAsset<TextureAnimated>("playBtn"));
	m_data->ui.buttonMap["settingsBtn"].setTexture(*m_data->gameData->m_assetManager->GetAsset<TextureAnimated>("settingsBtn"));
	m_data->ui.buttonMap["leaveBtn"].setTexture(*m_data->gameData->m_assetManager->GetAsset<TextureAnimated>("leaveBtn"));
	m_data->ui.buttonMap["creditsBtn"].setTexture(*m_data->gameData->m_assetManager->GetAsset<TextureAnimated>("creditsBtn"));
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
	m_data->ui.logoCrea.setPosition({ 10, SCREEN_HEIGHT / 1.3 });
	m_data->ui.logoCrea.setScale({ 0.3f,0.3f });

	//Icons chara
	m_data->ui.charaAvaible.push_back("Perso1-1");
	m_data->ui.charaAvaible.push_back("Perso2-1");
	m_data->ui.charaAvaible.push_back("Perso3-1");
	m_data->ui.charaAvaible.push_back("Perso4-1");

	m_data->ui.iconsChara.setTexture(*m_data->gameData->m_assetManager->GetAsset<TextureAnimated>("Icone", AssetManager::AssetType::TEXTURE_ANIMATED));
	m_data->ui.iconsChara.SetAnimation("Perso1-1");
	m_data->ui.iconsChara.setOrigin({ 0.5f,0.5f });

	sf::FloatRect buttonRect = m_data->ui.buttonMap["playBtn"].getGlobalBounds();
	m_data->ui.buttonMap["playBtn"].setPosition({ SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 });
	m_data->ui.buttonMap["settingsBtn"]. setPosition({ SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + buttonRect.height});
	m_data->ui.buttonMap["leaveBtn"].setPosition({ SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 2 * buttonRect.height });
	m_data->ui.buttonMap["creditsBtn"].setPosition({ SCREEN_WIDTH - buttonRect.width / 2, SCREEN_HEIGHT / 2 + 2 * buttonRect.height });

	
	m_data->ui.buttonMap["playBtn"].setOrigin({ 0.5f,0.5f });
	m_data->ui.buttonMap["settingsBtn"].setOrigin({ 0.5f,0.5f });
	m_data->ui.buttonMap["creditsBtn"].setOrigin({ 0.5f,0.5f });
	m_data->ui.buttonMap["leaveBtn"].setOrigin({ 0.5f,0.5f });
	m_data->ui.buttonMap["moinsBtn"].setOrigin({ 0.5f,0.5f });
	m_data->ui.buttonMap["plusBtn"].setOrigin({ 0.5f,0.5f });

	m_data->ui.buttonMap["creditsBtn"].setScale({ 0.8f,0.8f });

	m_data->ui.playerCount.setFont(*m_data->gameData->m_assetManager->GetAsset<sf::Font>("MenuFont"));
	m_data->ui.playerCount.setCharacterSize(200u);
	m_data->ui.playerCount.setPosition({ SCREEN_WIDTH / 2 , SCREEN_HEIGHT / 2 });
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
				switch (m_data->state)
				{
					case MAIN_MENU:

						m_data->gameData->m_renderWindow->close();
						break;
					case OPTIONS:
					case PLAYER_NB_SELECTION:

						m_data->state = MAIN_MENU;
						break;
					case PLAYER_SELECTION:

						m_data->state = PLAYER_NB_SELECTION;
						break;
				}
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

						//std::cout << "jostick : " << _event.joystickMove.position << "btn : " << m_data->controlerBtn << std::endl;

						if (_event.joystickMove.position > 20)
						{
							ChangeSelection(1, _event.joystickMove.joystickId);
							m_data->inputDelay = 0.f;
						}
						else if (_event.joystickMove.position < -20)
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
			m_data->ui.buttonMap["playBtn"].CheckEvent(_event);
			m_data->ui.buttonMap["settingsBtn"].CheckEvent(_event);
			m_data->ui.buttonMap["leaveBtn"].CheckEvent(_event);
			m_data->ui.buttonMap["creditsBtn"].CheckEvent(_event);
			break;

		case OPTIONS:
			m_data->ui.buttonMap["playBtn"].CheckEvent(_event);
			m_data->ui.buttonMap["plusBtn"].CheckEvent(_event);
			m_data->ui.buttonMap["moinsBtn"].CheckEvent(_event);
			break;

		case PLAYER_NB_SELECTION:
			m_data->ui.buttonMap["playBtn"].CheckEvent(_event);
			m_data->ui.buttonMap["plusBtn"].CheckEvent(_event);
			m_data->ui.buttonMap["moinsBtn"].CheckEvent(_event);
			break;
	}
}

void Menu::Update(float _deltaTime)
{
	m_data->audio->UpdateMusicTransition(_deltaTime);
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
			m_data->ui.buttonMap["creditsBtn"].Update(_dt);
			break;

		case OPTIONS:
			m_data->ui.buttonMap["playBtn"].Update(_dt);
			m_data->ui.buttonMap["plusBtn"].Update(_dt);
			m_data->ui.buttonMap["moinsBtn"].Update(_dt);
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
			_renderWindow.draw(m_data->ui.buttonMap["creditsBtn"]);
			_renderWindow.draw(m_data->ui.logoGame);
			//Bandage fix
			//_renderWindow.draw(m_data->ui.logoCrea);

			if (m_data->ui.buttonMap["leaveBtn"].HasBeenClicked())
			{
				_renderWindow.close();
			}
			break;

		case OPTIONS:
			PrintOptions(_renderWindow);

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
			
			// CORRECTION : Ne changer la s�lection que si le joueur n'a pas encore valid� son choix
			if (m_data->charaSelected[_joystick] == false)
			{
				int totalCharacters = (int)m_data->ui.charaAvaible.size();
				int newSelection = m_data->currentCharaSelected[_joystick] + _value;
				
				// Wrap circulaire : si on d�passe � droite, revenir au d�but
				if (newSelection >= totalCharacters)
				{
					m_data->currentCharaSelected[_joystick] = 0;
				}
				// Wrap circulaire : si on d�passe � gauche, aller � la fin
				else if (newSelection < 0)
				{
					m_data->currentCharaSelected[_joystick] = totalCharacters - 1;
				}
				// Sinon, changer normalement
				else
				{
					m_data->currentCharaSelected[_joystick] = newSelection;
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

			switch (m_data->state)
			{
				case MAIN_MENU:

					m_data->audio->PlayMusicTransition("Music2", true, false, 5.f, TransitionType::FADED_MIX);

					m_data->state = PLAYER_NB_SELECTION;
					m_data->controlerBtn = PLAY_SELECTION;
					m_data->ui.buttonMap["plusBtn"].setScale(1, 1);
					m_data->ui.buttonMap["moinsBtn"].setScale(1, 1);
					m_data->ui.buttonMap["playBtn"].setPosition({ SCREEN_WIDTH / 2, SCREEN_HEIGHT / 1.5 });
					m_data->ui.buttonMap["moinsBtn"].setPosition({ SCREEN_WIDTH / 2 - 0.2f * SCREEN_WIDTH, SCREEN_HEIGHT / 2 });
					m_data->ui.buttonMap["plusBtn"].setPosition({ SCREEN_WIDTH / 2 + 0.2f * SCREEN_WIDTH, SCREEN_HEIGHT / 2 });
					m_data->ui.playerCount.setString(std::to_string(m_data->gameSettings.playerCount + 1));
					break;

				case PLAYER_NB_SELECTION:
					break;
	
				case PLAYER_SELECTION:
				{
					std::cout << "id = " << _id << " size of datalist = " << m_data->gameData->m_playerDataList.size() << std::endl;
					
					// CORRECTION : Convertir l'index de s�lection en PlayerSkin
					// m_currentCharaSelected[_id] contient l'index dans charaAvaible (0-3)
					// Correspondance :
					// 0 -> "Perso1-1" -> CHARACTER_1_1
					// 1 -> "Perso2-1" -> CHARACTER_2_1
					// 2 -> "Perso3-1" -> CHARACTER_3_1
					// 3 -> "Perso4-1" -> CHARACTER_4_1
					
					PlayerData::PlayerSkin selectedSkin;
					switch (m_data->currentCharaSelected[_id])
					{
						case 0:
							selectedSkin = PlayerData::CHARACTER_1_1;
							break;
						case 1:
							selectedSkin = PlayerData::CHARACTER_2_1;
							break;
						case 2:
							selectedSkin = PlayerData::CHARACTER_3_1;
							break;
						case 3:
							selectedSkin = PlayerData::CHARACTER_4_1;
							break;
						default:
							selectedSkin = PlayerData::CHARACTER_1_1; // Valeur par d�faut
							break;
					}
					
					m_data->gameData->m_playerDataList[_id].SetPlayerSkin(selectedSkin);
					m_data->charaSelected[_id] = true;

					// V�rifier si tous les joueurs ont s�lectionn� leur personnage
					int result = 0;
					for (auto selected : m_data->charaSelected)
					{
						if (selected)
						{
							result++;
						}
					}
					
					if (result == m_data->gameSettings.playerCount + 1)
					{
						std::cout << "All players have their skin, go to game\n";
						SceneBase::ChangeScene("Lo");
					}
					break;
				}
			}
			break;

		case SETTINGS:

			m_data->state = OPTIONS;
			m_data->controlerBtn = MORE;

			m_data->ui.buttonMap["plusBtn"].setScale(0.8f,0.8f);
			m_data->ui.buttonMap["moinsBtn"].setScale(0.8f,0.8f);

			m_data->ui.buttonMap["plusBtn"].setPosition(SCREEN_WIDTH / 1.2f, 2.1f / 4.f * SCREEN_HEIGHT);
			m_data->ui.buttonMap["moinsBtn"].setPosition(SCREEN_WIDTH / 1.5f, 2.1f / 4.f * SCREEN_HEIGHT);
			m_data->ui.buttonMap["playBtn"].setPosition(SCREEN_WIDTH / 2.f, 3.f / 4.f * SCREEN_HEIGHT);
			m_data->controlerBtn = PLAY_SELECTION;
			break;

		case LEAVE:
			m_data->gameData->m_renderWindow->close();
			break;

		case LESS:
			switch (m_data->state)
			{
				case PLAYER_NB_SELECTION:

					if (m_data->gameSettings.playerCount > MIN_PLAYERS)
					{
						m_data->gameSettings.playerCount -= 1;
						m_data->ui.playerCount.setString(std::to_string(m_data->gameSettings.playerCount + 1));
					}
					break;

				case OPTIONS:
				
					m_data->audio->AddMusicVolume(-10.f);
					m_data->audio->AddSoundVolume(-10.f);
					break;
			}
			break;

		case PLAY_SELECTION:
		{
			switch (m_data->state)
			{
				case PLAYER_NB_SELECTION:

					m_data->audio->PlayMusicTransition("Music1", true, true, 5.f, TransitionType::FADED_MIX);

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

				case OPTIONS:

					m_data->ui.playerCount.setCharacterSize(200u);
					m_data->ui.playerCount.setOrigin({ 0.6f,0.8f });

					m_data->state = MAIN_MENU;
					m_data->controlerBtn = PLAY;
					sf::FloatRect buttonRect = m_data->ui.buttonMap["playBtn"].getLocalBounds();
					m_data->ui.buttonMap["playBtn"].setPosition({ SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 });
					m_data->ui.buttonMap["settingsBtn"].setPosition({ SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + buttonRect.height });
					m_data->ui.buttonMap["leaveBtn"].setPosition({ SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 2 * buttonRect.height });
					break;
			}
			break;
		}		
		case MORE:

			switch (m_data->state)
			{
				case PLAYER_NB_SELECTION:

					if (m_data->gameSettings.playerCount < MAX_PLAYERS)
					{
						m_data->gameSettings.playerCount += 1;
						m_data->ui.playerCount.setString(std::to_string(m_data->gameSettings.playerCount + 1));
					}
					break;

				case OPTIONS:

					m_data->audio->AddMusicVolume(10.f);
					m_data->audio->AddSoundVolume(10.f);
					break;
			}
			break;
	}
}

void Menu::PrintIcons(sf::RenderWindow& _renderWindow)
{
	// Sauvegarder la couleur temporaire pour la restaurer apr�s
	sf::Color tempColor = m_data->ui.iconsChara.getColor();

	// Calcul du placement des ic�nes
	float border = 200.f;
	float iconSpacing = (SCREEN_WIDTH - 2 * border) / m_data->gameSettings.playerCount;

	for (int i = 0; i < m_data->gameSettings.playerCount + 1; i++)
	{
		// Positions des 3 ic�nes (gauche, centre, droite)
		sf::Vector2f iconPos[] =
		{
			{(float)(border + i * iconSpacing), SCREEN_HEIGHT / 2.f - SCREEN_HEIGHT / 4.f},
			{(float)(border + i * iconSpacing), SCREEN_HEIGHT / 2.f },
			{(float)(border + i * iconSpacing), SCREEN_HEIGHT / 2.f + SCREEN_HEIGHT / 4.f}
		};

		// Afficher le texte "Player : X"
		sf::Vector2f pos = { iconPos[0].x, iconPos[0].y - 70.f };
		m_data->ui.playerCount.setPosition(pos);

		char buffer[30];
		std::snprintf(buffer, 30, "Player : %d", i + 1);
		m_data->ui.playerCount.setString(buffer);
		m_data->ui.playerCount.setCharacterSize(60u);
		_renderWindow.draw(m_data->ui.playerCount);

		// Index de l'animation s�lectionn�e pour ce joueur
		int currentSelection = m_data->currentCharaSelected[i];
		int totalCharacters = (int)m_data->ui.charaAvaible.size();

		// Calculer les index des 3 ic�nes (avec wrap)
		int leftIndex = (currentSelection - 1 + totalCharacters) % totalCharacters;
		int centerIndex = currentSelection;
		int rightIndex = (currentSelection + 1) % totalCharacters;

		// Ic�ne de gauche (semi-transparente)
		tempColor.a = 100;
		m_data->ui.iconsChara.setColor(tempColor);
		m_data->ui.iconsChara.SetAnimation(m_data->ui.charaAvaible[leftIndex]);
		m_data->ui.iconsChara.setPosition(iconPos[0]);
		_renderWindow.draw(m_data->ui.iconsChara);

		// Ic�ne du centre (opaque ou sombre si s�lectionn�e)
		tempColor.a = 255;
		if (m_data->charaSelected[i] == true)
		{
			// Si le joueur a valid� son choix, afficher en noir
			m_data->ui.iconsChara.setColor(sf::Color(200, 200, 200, 255));
		}
		else
		{
			// Sinon afficher normalement
			m_data->ui.iconsChara.setColor(tempColor);
		}
		m_data->ui.iconsChara.SetAnimation(m_data->ui.charaAvaible[centerIndex]);
		m_data->ui.iconsChara.setPosition(iconPos[1]);
		_renderWindow.draw(m_data->ui.iconsChara);

		// Ic�ne de droite (semi-transparente)
		tempColor.a = 100;
		m_data->ui.iconsChara.setColor(tempColor);
		m_data->ui.iconsChara.SetAnimation(m_data->ui.charaAvaible[rightIndex]);
		m_data->ui.iconsChara.setPosition(iconPos[2]);
		_renderWindow.draw(m_data->ui.iconsChara);
	}

	// R�initialiser la couleur � blanc opaque
	m_data->ui.iconsChara.setColor(sf::Color(255, 255, 255, 255));
}

void Menu::PrintOptions(sf::RenderWindow& _renderWindow)
{
	m_data->ui.playerCount.setCharacterSize(50u);
	m_data->ui.playerCount.setOrigin({ 0.f,0.f });
	m_data->ui.playerCount.setPosition(0.f, 2.f / 4.f * SCREEN_HEIGHT);
	m_data->ui.playerCount.setCharacterSize(100u);
	char buffer[100];
	std::snprintf(buffer, 100, "Game volume : %0.0f", m_data->audio->GetMusicVolume());
	m_data->ui.playerCount.setString(buffer);
	_renderWindow.draw(m_data->ui.playerCount);

	m_data->ui.playerCount.setCharacterSize(150u);
	m_data->ui.playerCount.setOrigin({ 0.6f,0.8f });
	m_data->ui.playerCount.setPosition(SCREEN_WIDTH / 2.f, 1.f / 4.f * SCREEN_HEIGHT);
	m_data->ui.playerCount.setString("Settings");
	_renderWindow.draw(m_data->ui.playerCount);

	_renderWindow.draw(m_data->ui.buttonMap["plusBtn"]);
	_renderWindow.draw(m_data->ui.buttonMap["moinsBtn"]);
	_renderWindow.draw(m_data->ui.buttonMap["playBtn"]);
}