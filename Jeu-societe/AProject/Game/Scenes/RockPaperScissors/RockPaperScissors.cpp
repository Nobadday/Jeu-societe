#include "RockPaperScissors.hpp"


#define PLAY_TIME 5
#define PAUSE_TIME 2

void RockPaperScissors::Load()
{
	m_data = new SceneData;

	m_data->gameData = (GameData*)this->m_keptData;

	m_data->gameData->m_assetManager->LoadManifest("Manifests/RockPaperScissor.json", "rockpaperScissor");

	for (short i = 0; i < 6; i++)
	{
	switch (i)
		{
		case 0:
			m_data->spriteTab[i].setTexture(*m_data->gameData->m_assetManager->GetAsset<sf::Texture>("Abutton", AssetManager::AssetType::TEXTURE));
			break;
		case 1:
			m_data->spriteTab[i].setTexture(*m_data->gameData->m_assetManager->GetAsset<sf::Texture>("Bbutton", AssetManager::AssetType::TEXTURE));
			break;
		case 2:
			m_data->spriteTab[i].setTexture(*m_data->gameData->m_assetManager->GetAsset<sf::Texture>("Xbutton", AssetManager::AssetType::TEXTURE));
			break;
		case 3:
			m_data->spriteTab[i].setTexture(*m_data->gameData->m_assetManager->GetAsset<sf::Texture>("Ciseaux", AssetManager::AssetType::TEXTURE));
			break;
		case 4:
			m_data->spriteTab[i].setTexture(*m_data->gameData->m_assetManager->GetAsset<sf::Texture>("Pierre", AssetManager::AssetType::TEXTURE));
			break;
		case 5:
			m_data->spriteTab[i].setTexture(*m_data->gameData->m_assetManager->GetAsset<sf::Texture>("Feuille", AssetManager::AssetType::TEXTURE));
			break;
		default:
			break;
		}
		m_data->spriteTab[i].setPosition({ (float)100 * i, 100.f });
	}

	m_data->timerText.setFont(*m_data->gameData->m_assetManager->GetAsset<sf::Font>("RPSFont", AssetManager::AssetType::FONT));


	m_data->playerChoiceSprite[0].setTexture(*m_data->gameData->m_assetManager->GetAsset<TextureAnimated>("LeftHands", AssetManager::AssetType::TEXTURE_ANIMATED));
	m_data->playerChoiceSprite[1].setTexture(*m_data->gameData->m_assetManager->GetAsset<TextureAnimated>("RightHands", AssetManager::AssetType::TEXTURE_ANIMATED));

	m_data->playerChoiceSprite[0].setOrigin(sf::Vector2f(0, 0.5f));
	m_data->playerChoiceSprite[1].setOrigin(sf::Vector2f(1, 0.5f));

	m_data->playerChoiceSprite[0].setPosition({SCREEN_WIDTH * 0.1f, SCREEN_HEIGHT * 0.66f});
	m_data->playerChoiceSprite[1].setPosition({SCREEN_WIDTH * 0.9f, SCREEN_HEIGHT * 0.66f});

	m_data->playerChoiceSprite[0].SetAnimation("Rock");
	m_data->playerChoiceSprite[1].SetAnimation("Rock");

	m_data->victoryText.setFont(*m_data->gameData->m_assetManager->GetAsset<sf::Font>("RPSFont", AssetManager::AssetType::FONT));
	m_data->victoryText.setPosition({ SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 });
	
	//Temporaire
	if (((GameData*)this->m_keptData)->m_gonnaPlayIndex.size() == 0)
	{
		((GameData*)this->m_keptData)->m_gonnaPlayIndex.push_back(0);
		((GameData*)this->m_keptData)->m_gonnaPlayIndex.push_back(1);
	}

	m_data->timer.SetTimeTarget(PLAY_TIME);

	m_data->playersChoice[0] = RPS_NONE;
	m_data->playersChoice[1] = RPS_NONE;

	m_data->state = STATE_PLAY;
	m_data->roundNB = 0;

	for (short i = 0; i < 2; i++)
	{
		m_data->animator[i].Modify(0.4f, 60.f);
		m_data->animator[i].SetRotation(0.f, powf(-1, i+1) * 45.f);
		m_data->animator[i].SetAnimationEasing(anim::Animator::ROTATION, anim::Easing::INSINE);
	}
}                               

void RockPaperScissors::Unload()
{
	m_data->gameData->m_assetManager->DeleteContainer("rockpaperScissor");
	m_data->gameData = NULL;
	delete m_data;
}

void RockPaperScissors::PollEvent(sf::Event& _event)
{
	switch (_event.type)
	{
		if (m_data->state == STATE_PLAY)
		{
			if (!m_data->timer.IsFinished())
			{
				case sf::Event::JoystickButtonPressed:
				{
					//Check id joueur = joueur QUI joue
					//Check ses boutons
					int playerID = this->m_data->gameData->GetPlayerIDFromJoystick(_event.joystickButton.joystickId);
					if (this->m_data->gameData->IsPlayerParticipating(playerID))
					{
						std::cout << playerID << std::endl;
					}

					if (this->m_data->gameData->IsPlayerParticipating(playerID))
					{
						switch (_event.joystickButton.button)
						{
						case 0:
							this->m_data->playersChoice[playerID] = RPS_SCISSORS;
							break;

						case 1:
							this->m_data->playersChoice[playerID] = RPS_ROCK;
							break;

						case 2:
							this->m_data->playersChoice[playerID] = RPS_PAPER;
							break;

						default:
							break;
						}
					}
				}
				break;
			}
			
		}
		break;

	default:
		break;
	}
}

void RockPaperScissors::Update(float _deltaTime)
{   
	m_data->timer.Update(_deltaTime);
	char buffer[20];
	std::snprintf(buffer, 20, "%02.2f", m_data->timer.GetRemainingTime());
	m_data->timerText.setString(buffer);
	
	switch (m_data->state)
	{
		case STATE_PLAY:

			//Animation gestion for hands
			for (short i = 0; i < 2; i++)
			{
				this->m_data->animator[i].Update(_deltaTime);
				if (this->m_data->animator[i].IsFinished())
				{
					if (m_data->timer.IsFinished() && m_data->animator[i].IsReversed())
					{
						m_data->animator[i].SetPause(true);
					}
					else
					{
						m_data->animator[i].ToggleReverse();
					}
				}
				this->m_data->animator[i].AnimateObject(this->m_data->playerChoiceSprite[i]);
			}


			if (m_data->animator[0].IsPaused() && m_data->animator[1].IsPaused())
			{
				this->m_data->timer.SetTimeTarget(PAUSE_TIME, true);
				//Check if player have no active choice
				if (this->m_data->playersChoice[this->m_data->gameData->m_gonnaPlayIndex[0]] == RPS_NONE)
				{
					this->m_data->playersChoice[this->m_data->gameData->m_gonnaPlayIndex[0]] = RPS_Choice(random::RandomInt(0, 2));
				}
				if (this->m_data->playersChoice[this->m_data->gameData->m_gonnaPlayIndex[1]] == RPS_NONE)
				{
					this->m_data->playersChoice[this->m_data->gameData->m_gonnaPlayIndex[1]] = RPS_Choice(random::RandomInt(0, 2));
				}

				//Check if the game ends
				if (this->m_data->playersChoice[this->m_data->gameData->m_gonnaPlayIndex[0]] == this->m_data->playersChoice[this->m_data->gameData->m_gonnaPlayIndex[1]])
				{
					if (this->m_data->roundNB < 2)
					{
						this->m_data->state = STATE_PAUSE;
						this->m_data->roundNB++;
					}
					else
					{
						this->m_data->state = STATE_ENDGAME;
					}
				}
				else if (this->m_data->playersChoice[this->m_data->gameData->m_gonnaPlayIndex[0]] > this->m_data->playersChoice[this->m_data->gameData->m_gonnaPlayIndex[1]]
					|| m_data->playersChoice[m_data->gameData->m_gonnaPlayIndex[0]] == RPS_ROCK && this->m_data->playersChoice[this->m_data->gameData->m_gonnaPlayIndex[1]] == RPS_SCISSORS)
				{
					std::cout << "player " << this->m_data->gameData->m_gonnaPlayIndex[0] + 1 << " win";
					this->m_data->gameData->AddPlayerWin(this->m_data->gameData->m_gonnaPlayIndex[0]);
					this->m_data->gameData->AddPlayerWin(this->m_data->gameData->m_gonnaPlayIndex[1]);
					this->m_data->state = STATE_ENDGAME;
				}
				else
				{
					std::cout << "player " << this->m_data->gameData->m_gonnaPlayIndex[1] + 1 << " win";
					this->m_data->gameData->AddPlayerWin(this->m_data->gameData->m_gonnaPlayIndex[1]);
					this->m_data->gameData->AddPlayerWin(this->m_data->gameData->m_gonnaPlayIndex[0]);
					this->m_data->state = STATE_ENDGAME;
				}
			}
			this->UpdatePlayerChoiceTexture();

			break;
		case STATE_PAUSE:

			if (m_data->timer.IsFinished())
			{
				this->m_data->timer.SetTimeTarget(PLAY_TIME, true);
				for (short i = 0; i < 2; i++)
				{
					this->m_data->playerChoiceSprite[i].SetAnimation("Rock");
					this->m_data->animator[i].SetPause(false);
					this->m_data->playersChoice[i] = RPS_NONE;
				}

				this->m_data->state = STATE_PLAY;
				this->UpdatePlayerChoiceTexture();
				break;
			}

			break;
		case STATE_ENDGAME:
			if (m_data->timer.IsFinished())
			{
				ChangeScene("Board");	
			}
			break;

	default:
		break;
	}
}

void RockPaperScissors::Draw(sf::RenderWindow& _renderWindow)
{
	for (short i = 0; i < 6; i++)
	{
		_renderWindow.draw(m_data->spriteTab[i]);
	}

	for (short i = 0; i < 2; i++)
	{
		_renderWindow.draw(this->m_data->playerChoiceSprite[i]);
	}
	_renderWindow.draw(m_data->timerText);
}

void RockPaperScissors::UpdatePlayerChoiceTexture()
{
	for (short i = 0; i < 2; i++)
	{
		switch (this->m_data->playersChoice[i])
		{
			case RPS_ROCK:
				this->m_data->playerChoiceSprite[i].SetAnimation("Rock");
				break;
			case RPS_PAPER:
				this->m_data->playerChoiceSprite[i].SetAnimation("Paper");
				break;
			case RPS_SCISSORS:
				this->m_data->playerChoiceSprite[i].SetAnimation("Scissors");
				break;
			default:
				break;
		}
	}
}