#include "ArmWrestling.hpp"

#define MAX 20

#define PLAY_TIME 10
#define END_TIME 5

void ArmWrestling::Load(void)
{
	m_gameData = (GameData*)m_keptData;

	m_gameData->m_assetManager->LoadManifest("Manifests/ArmWrestling.json", "AW");
	m_background.setTexture(*m_gameData->m_assetManager->GetAsset<sf::Texture>("MinigameBackground", AssetManager::AssetType::TEXTURE));

	m_playerArms.SetTexture(*m_gameData->m_assetManager->GetAsset<TextureAtlas>("playerArms", AssetManager::AssetType::TEXTURE_ATLAS), "IDLE");
	m_playerArms.setOrigin(0.5f, 1);
	m_playerArms.setPosition(SCREEN_WIDTH / 2, SCREEN_HEIGHT);
	m_playerArms.setScale(1.2f, 1.2f);


	m_bar.SetSize(sf::Vector2f(SCREEN_WIDTH / 7, SCREEN_HEIGHT / 18));
	m_bar.setOrigin(m_bar.GetLocalBounds().getSize().x / 2, m_bar.GetLocalBounds().getSize().y / 2);
	m_bar.setPosition(SCREEN_WIDTH / 2, SCREEN_HEIGHT * 0.15f);


	m_bar.SetBarCompletion(m_currentValue, MAX, 0);
	m_bar.SetAvoidOverflow(true);
	
	m_bar.SetColorFiller(sf::Color(5, 215, 223));
	m_bar.SetColorBG(sf::Color(94, 6, 113));
	//m_bar.SetTextureBG(m_gameData->m_assetManager->GetAsset<sf::Texture>("BackgroundBar", AssetManager::AssetType::TEXTURE), true);
	//m_bar.SetTextureFiller(m_gameData->m_assetManager->GetAsset<sf::Texture>("ForegroundBar", AssetManager::AssetType::TEXTURE), true);


	m_currentValue = MAX / 2;

	m_timerText.setFont(*m_gameData->m_assetManager->GetAsset<sf::Font>("ArmWrestlingFont", AssetManager::AssetType::FONT));
	m_timerText.SetOutline(2, sf::Color::Black);
	m_timerText.setCharacterSize(75u); 
	m_timerText.setOrigin(sf::Vector2f(0.5f, 0.5f));
	m_timerText.setPosition(SCREEN_WIDTH / 2, SCREEN_HEIGHT * 0.05f);

	m_timerText.setOrigin(sf::Vector2f(0.5f, 0.5f));

	m_timer.SetTimeTarget(PLAY_TIME, true);

	if (m_gameData->m_gonnaPlayIndex.size() == 0)
	{
		m_gameData->m_gonnaPlayIndex.push_back(0);
		m_gameData->m_gonnaPlayIndex.push_back(1);
	}

	m_state = GAME;
}


void ArmWrestling::Unload(void)
{
	m_gameData = NULL;
}


void ArmWrestling::PollEvent(sf::Event& _event)
{
	switch (_event.type)
	{
		case sf::Event::JoystickButtonPressed:
			{
				if (!m_timer.IsFinished() && m_state == GAME)
				{
					int playerID = this->m_gameData->GetPlayerIDFromJoystick(_event.joystickButton.joystickId);

					std::cout << playerID << std::endl;
					if (this->m_gameData->IsPlayerParticipating(playerID))
					{
						switch (_event.joystickButton.button)
						{
						case 0:
							if (playerID == m_gameData->m_gonnaPlayIndex[0])
							{
								m_currentValue++;
							}

							if (playerID == m_gameData->m_gonnaPlayIndex[1])
							{
								m_currentValue--;
							}
							break;

							default:
								break;
						}
					}
				}
			}
		break;
	}
}


void ArmWrestling::Update(float _deltaTime)
{
	m_timer.Update(_deltaTime);
	switch (m_state)
	{
		case ArmWrestling::GAME:
			char buffer[50];
			std::snprintf(buffer, 50, "%02.2f", m_timer.GetRemainingTime());
			m_timerText.setString(buffer);


			m_bar.SetBarCompletion(m_currentValue, MAX);	
			break;

		case ArmWrestling::END:
			break;
		
	default:
		break;
	}




	if (m_timer.IsFinished())
	{
		switch (m_state)
		{
		case ArmWrestling::GAME:
			{
				m_timer.SetTimeTarget(END_TIME, true);
				m_state = END;

				std::string string;

					if (m_currentValue > MAX / 2)
					{
						string = "Player" + std::to_string(m_gameData->m_gonnaPlayIndex[0] + 1) + " WIN";
						m_playerArms.SetTextureFrame("LEFt_WIN");
						m_gameData->AddPlayerWin(m_gameData->m_gonnaPlayIndex[0]);
						m_gameData->AddPlayerWin(m_gameData->m_gonnaPlayIndex[1]);
					}
					else if (m_currentValue < MAX / 2)
					{
						string = "Player" + std::to_string(m_gameData->m_gonnaPlayIndex[1] + 1) + " WIN";
						m_playerArms.SetTextureFrame("RIGHT_WIN");
						m_gameData->AddPlayerWin(m_gameData->m_gonnaPlayIndex[1]);
						m_gameData->AddPlayerWin(m_gameData->m_gonnaPlayIndex[0]);
					}
					else
					{
						string = "Tie, no winner";
					}

					m_timerText.setString(string);
			}
				break;

			case ArmWrestling::END:

				//ChangeScene("Board");
				break;

		default:
			break;
		}
	}
	else if (m_currentValue == 20)
	{
		m_timer.End();
		std::string string = "Player " + std::to_string(m_gameData->m_gonnaPlayIndex[0] + 1) + " WIN";
		m_playerArms.SetTextureFrame("LEFt_WIN");
		m_gameData->AddPlayerWin(m_gameData->m_gonnaPlayIndex[0]);
		m_gameData->AddPlayerWin(m_gameData->m_gonnaPlayIndex[1]);
		m_timerText.setString(string);
		m_state = END;
	}
	else if (m_currentValue == 0)
	{
		m_timer.End();
		std::string string = "Player " + std::to_string(m_gameData->m_gonnaPlayIndex[1] + 1) + " WIN";
		m_playerArms.SetTextureFrame("RIGHT_WIN");
		m_gameData->AddPlayerWin(m_gameData->m_gonnaPlayIndex[1]);
		m_gameData->AddPlayerWin(m_gameData->m_gonnaPlayIndex[0]);
		m_timerText.setString(string);
		m_state = END;
	}

}

void ArmWrestling::Draw(sf::RenderWindow& _renderWindow)
{
	_renderWindow.draw(m_background);
	_renderWindow.draw(m_playerArms);

	switch (m_state)
	{
		case ArmWrestling::GAME:
			_renderWindow.draw(m_bar);
		break;

		case ArmWrestling::END:
			break;

		default:
			break;
	}
	
	_renderWindow.draw(m_timerText);

}