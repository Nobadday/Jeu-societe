#include "Menu.hpp"

//3 = 4 players
#define MAX_PLAYERS 3
//1 = 2 players
#define MIN_PLAYERS 1
//Delay to scroll in buttons with controler
#define INPUT_DELAY 0.2f

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
	m_data->ui.background.setTexture(*m_data->gameData->m_assetManager->GetAsset<sf::Texture>("background"));


	sf::FloatRect buttonRect = m_data->ui.buttonMap["playBtn"].getLocalBounds();
	m_data->ui.buttonMap["playBtn"].setPosition({ SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 });
	m_data->ui.buttonMap["settingsBtn"].setPosition({ SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + buttonRect.height});
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
	//Font sucks
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
			if (_event.key.code != sf::Keyboard::Enter)
			{
				break;
			}
		case sf::Event::JoystickButtonPressed:

			PressSelection();
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
				case sf::Joystick::Axis::X:
				case sf::Joystick::Axis::U:

					ChangeSelection(_event.joystickMove.position / 100.f);
					m_data->inputDelay = 0.f;
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

	//Update timer for 
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
	
			if (m_data->ui.buttonMap["playBtn"].HasBeenClicked())
			{
				m_data->controlerBtn = PLAY;
				PressSelection();
			}
			else if (m_data->ui.buttonMap["settingsBtn"].HasBeenClicked())
			{
				m_data->controlerBtn = SETTINGS;
				PressSelection();
			}
			break;

		case OPTIONS:
			PressSelection();
			break;

		case PLAYER_NB_SELECTION:
			m_data->ui.buttonMap["playBtn"].Update(_dt);
			m_data->ui.buttonMap["plusBtn"].Update(_dt);
			m_data->ui.buttonMap["moinsBtn"].Update(_dt);


			if (m_data->ui.buttonMap["plusBtn"].HasBeenClicked())
			{
				if (m_data->gameSettings.playerCount < MAX_PLAYERS)
				{
					m_data->controlerBtn = MORE;
					PressSelection();
				}
			}
			else if (m_data->ui.buttonMap["moinsBtn"].HasBeenClicked())
			{
				if (m_data->gameSettings.playerCount > MIN_PLAYERS)
				{
					m_data->controlerBtn = LESS;
					PressSelection();
				}
			}
			else if (m_data->ui.buttonMap["playBtn"].HasBeenClicked())
			{
				m_data->controlerBtn = PLAY_SELECTION;
				PressSelection();
			}
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
	}
}

void Menu::ChangeSelection(int _value)
{
	m_data->audio->PlaySound("uiSoundON");
	//Reset old button
	int i = 0;
	for (auto& button : m_data->ui.buttonMap)
	{
		if (i == _value)
		{
			button.second.SetState(Button::STATE_IDLE);
		}
		i++;
	}

	switch (m_data->state)
	{
		case MAIN_MENU:
			
			if ((m_data->controlerBtn + _value) < PLAY)
			{
				m_data->controlerBtn = LEAVE;
			}
			else if ((m_data->controlerBtn + _value) > LEAVE)
			{
				m_data->controlerBtn = PLAY;
			}
			else
			{
				m_data->controlerBtn = (ControlerCurrentButton)(m_data->controlerBtn + _value);
			}
			break;

		case OPTIONS:
			break;

		case PLAYER_NB_SELECTION:

			if ((m_data->controlerBtn + _value) < MORE)
			{
				m_data->controlerBtn = LESS;
			}
			else if ((m_data->controlerBtn + _value) > LESS)
			{
				m_data->controlerBtn = MORE;
			}
			else
			{
				m_data->controlerBtn = (ControlerCurrentButton)(m_data->controlerBtn + _value);
			}
			break;
	}
	//Set ON new button
	i = 0;
	for (auto& button : m_data->ui.buttonMap)
	{
		if (i == _value)
		{
			button.second.SetState(Button::STATE_ON);
		}
		i++;
	}
}

void Menu::PressSelection(void)
{
	switch (m_data->controlerBtn)
	{
		case PLAY:


			m_data->state = (MenuState)(m_data->state + 1);
			//if (m_data->state == )



			switch (m_data->state)
			{
			case MAIN_MENU:

				m_data->state = PLAYER_NB_SELECTION;


				break;
			case PLAYER_NB_SELECTION:

				break;
			case PLAYER_SELECTION:


				SceneBase::ChangeScene("Board");
				break;



			}




			m_data->state = PLAYER_NB_SELECTION;
			m_data->ui.buttonMap["playBtn"].setPosition({ SCREEN_WIDTH / 2, SCREEN_HEIGHT / 1.5 });
			break;

		case SETTINGS:
			//Bruh i need render window to exit, here i cant
			break;

		case LEAVE:
			//Later
			break;

		case LESS:
			m_data->gameSettings.playerCount -= 1;
			m_data->ui.playerCount.setString(std::to_string(m_data->gameSettings.playerCount + 1));
			break;

		case PLAY_SELECTION:
		{
			PlayerData newPlayer;
			newPlayer.m_playerSkin = PlayerData::PlayerSkin::CHARACTER_1_1;
			newPlayer.m_joystickId = 0;
			m_data->gameData->m_playerDataList.push_back(newPlayer);

			break;
		}		
		case MORE:
			m_data->gameSettings.playerCount += 1;
			m_data->ui.playerCount.setString(std::to_string(m_data->gameSettings.playerCount + 1));
			break;
	}
}