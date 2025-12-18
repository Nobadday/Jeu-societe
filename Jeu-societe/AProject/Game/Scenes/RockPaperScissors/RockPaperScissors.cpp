#include "RockPaperScissors.hpp"

#define PLAY_TIME 5
#define PAUSE_TIME 2

std::string choiceString[RockPaperScissors::RPS_Choice::RPS_CHOICE_COUNT]{
	"_R",
	"_P",
	"_S"

};



void RockPaperScissors::Load()
{
	m_data = new SceneData;

	m_data->gameData = (GameData*)this->m_keptData;
	m_data->gameData->m_assetManager->LoadManifest("Manifests/RockPaperScissor.json", "rockpaperScissor");


	m_data->background.setTexture(*m_data->gameData->m_assetManager->GetAsset<sf::Texture>("MinigameBackground", AssetManager::AssetType::TEXTURE));
	
	m_data->timerText.setFont(*m_data->gameData->m_assetManager->GetAsset<sf::Font>("RPSFont", AssetManager::AssetType::FONT));
	m_data->timerText.setOrigin(sf::Vector2f(0.6f, 0.8f));
	m_data->timerText.setPosition(SCREEN_WIDTH / 2, SCREEN_HEIGHT * 0.1f);
	m_data->timerText.setCharacterSize(75u);
	m_data->timerText.SetOutline(2, sf::Color::Black);


	m_data->playerChoiceSprite[0].setTexture(*m_data->gameData->m_assetManager->GetAsset<TextureAtlas>("RPSHand", AssetManager::AssetType::TEXTURE_ATLAS));
	m_data->playerChoiceSprite[1].setTexture(*m_data->gameData->m_assetManager->GetAsset<TextureAtlas>("RPSHand", AssetManager::AssetType::TEXTURE_ATLAS));

	m_data->victoryText.setFont(*m_data->gameData->m_assetManager->GetAsset<sf::Font>("RPSFont", AssetManager::AssetType::FONT));
	m_data->victoryText.setPosition({ SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 });
	
	//Temporaire
	if (((GameData*)this->m_keptData)->m_gonnaPlayIndex.size() == 0)
	{
		((GameData*)this->m_keptData)->m_gonnaPlayIndex.push_back(0);
		((GameData*)this->m_keptData)->m_gonnaPlayIndex.push_back(1);
	}

	m_data->timer.SetTimeTarget(PLAY_TIME);

	for (short i = 0; i < 2; i++)
	{
		m_data->playersChoice[i] = RPS_NONE;
	}

	m_data->state = STATE_WARMUP;
	m_data->roundNB = 0;

	for (short i = 0; i < 2; i++)
	{
		m_data->animator[i].Modify(0.4f, 60.f);
		m_data->animator[i].SetRotation(0.f, powf(-1.f, i+1.f) * 45.f);
		m_data->animator[i].SetAnimationEasing(anim::Animator::ROTATION, anim::Easing::INSINE);
	}

	m_data->playerChoiceSprite[0].setOrigin(sf::Vector2f(1.f, 0.f));
	m_data->playerChoiceSprite[1].setOrigin(sf::Vector2f(1.f, 0.f));

	m_data->playerChoiceSprite[0].setScale(-1.25f, 1.25f);
	m_data->playerChoiceSprite[1].setScale(1.25f, 1.25f);

	m_data->playerChoiceSprite[0].setPosition({ -170, SCREEN_HEIGHT * 0.45f });
	m_data->playerChoiceSprite[1].setPosition({ SCREEN_WIDTH + 170, SCREEN_HEIGHT * 0.45f });

	ChangePlayersChoiceTexture();
	m_data->transition.PlayTransition();
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

		case sf::Event::JoystickButtonPressed:
		{
			if (m_data->state == STATE_PLAY && !m_data->timer.IsFinished())
			{
				//Check id joueur = joueur QUI joue
				//Check ses boutons
				int playerID = this->m_data->gameData->GetPlayerIDFromJoystick(_event.joystickButton.joystickId);
				if (this->m_data->gameData->IsPlayerParticipating(playerID))
				{
					int realID = 0;
					for (int temp = 0; temp < this->m_data->gameData->m_gonnaPlayIndex.size(); temp++)
					{
						if (this->m_data->gameData->m_gonnaPlayIndex[temp] == playerID)
						{
							realID = temp;
						}
					}

					switch (_event.joystickButton.button)
					{
						case 0:
							this->m_data->playersChoice[realID] = RPS_PAPER;
							break;

						case 1:
							this->m_data->playersChoice[realID] = RPS_SCISSORS;
							break;

						case 2:
							this->m_data->playersChoice[realID] = RPS_ROCK;
							break;

					default:
						break;
					}
				}
			}
		}
		break;

	default:
		break;
	}
}

void RockPaperScissors::Update(float _deltaTime)
{   	

	switch (m_data->state)
	{
		case STATE_WARMUP:

			m_data->transition.Update(_deltaTime);
			
			if (m_data->transition.IsFinished())
			{
				m_data->state = STATE_PLAY;
			}
			break;

		case STATE_PLAY:

			UpdateTimer(_deltaTime);
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

				for (short i = 0; i < 4; i++)
				{
					if (m_data->gameData->IsPlayerParticipating(i))
					{
						if (this->m_data->playersChoice[i] == RPS_NONE)
						{
							this->m_data->playersChoice[i] = RPS_Choice(random::RandomInt(0, 2));
						}
					}
				}

				if (this->m_data->playersChoice[0] == this->m_data->playersChoice[1])
				{
					if (this->m_data->roundNB < 2)
					{
						this->m_data->state = STATE_PAUSE;
						this->m_data->roundNB++;
					}
					else
					{
						this->m_data->state = STATE_ENDGAME;
						m_data->transition.SetTransition(TransitionClass::FADED_OUT);
						m_data->transition.PlayTransition();
					}
				}
				else if (this->m_data->playersChoice[0] > this->m_data->playersChoice[1]
					|| m_data->playersChoice[0] == RPS_ROCK && this->m_data->playersChoice[1] == RPS_SCISSORS)
				{
					std::cout << "player " << this->m_data->gameData->m_gonnaPlayIndex[0] + 1 << " win";
					this->m_data->gameData->AddPlayerWin(this->m_data->gameData->m_gonnaPlayIndex[0]);
					this->m_data->gameData->AddPlayerWin(this->m_data->gameData->m_gonnaPlayIndex[1]);
					this->m_data->state = STATE_ENDGAME;
					m_data->transition.SetTransition(TransitionClass::FADED_OUT);
					m_data->transition.PlayTransition();
				}
				else
				{
					std::cout << "player " << this->m_data->gameData->m_gonnaPlayIndex[1] + 1 << " win";
					this->m_data->gameData->AddPlayerWin(this->m_data->gameData->m_gonnaPlayIndex[1]);
					this->m_data->gameData->AddPlayerWin(this->m_data->gameData->m_gonnaPlayIndex[0]);
					this->m_data->state = STATE_ENDGAME;
					m_data->transition.SetTransition(TransitionClass::FADED_OUT);
					m_data->transition.PlayTransition();
				}
				ChangePlayersChoiceTexture();
			}

			break;
		case STATE_PAUSE:

			UpdateTimer(_deltaTime);
			if (m_data->timer.IsFinished())
			{
				this->m_data->timer.SetTimeTarget(PLAY_TIME, true);
				for (short i = 0; i < 2; i++)
				{
					m_data->playersChoice[i] = RPS_ROCK;
					this->m_data->animator[i].SetPause(false);
					this->m_data->playersChoice[i] = RPS_NONE;
				}

				this->m_data->state = STATE_PLAY;
				this->ChangePlayersChoiceTexture();
				break;
			}

			break;
		case STATE_ENDGAME:

			UpdateTimer(_deltaTime);
			if (m_data->timer.IsFinished())
			{
				m_data->transition.Update(_deltaTime);

				if (m_data->transition.IsFinished())
				{
					ChangeScene("Board");
				}
			}

			break;

	default:
		break;
	}
}

void RockPaperScissors::Draw(sf::RenderWindow& _renderWindow)
{
	sfMod::RenderWindow* bWindow = m_data->gameData->m_renderWindow;

	_renderWindow.draw(m_data->background);

	for (short i = 0; i < 2; i++)
	{
		_renderWindow.draw(this->m_data->playerChoiceSprite[i]);
	}
	_renderWindow.draw(m_data->timerText);

	switch (m_data->state)
	{
		case STATE_WARMUP:
		case STATE_ENDGAME:
			m_data->transition.Draw(bWindow);
			break;
	}
}

void RockPaperScissors::ChangePlayersChoiceTexture()
{
	for (short i = 0; i < 2; i++)
	{
		std::string string = m_data->gameData->m_playerDataList[m_data->gameData->m_gonnaPlayIndex[i]].GetTextureName(m_data->gameData->m_playerDataList[m_data->gameData->m_gonnaPlayIndex[i]].m_playerSkin);
		
		if (m_data->playersChoice[i] == RPS_NONE)
		{
			m_data->playerChoiceSprite[i].SetTextureFrame(string + choiceString[RPS_ROCK]);
		}
		else
		{
			m_data->playerChoiceSprite[i].SetTextureFrame(string + choiceString[m_data->playersChoice[i]]);
		}
	}
}

void RockPaperScissors::UpdateTimer(float _dt)
{
	m_data->timer.Update(_dt);
	char buffer[20];
	std::snprintf(buffer, 20, "%02.2f", m_data->timer.GetRemainingTime());
	m_data->timerText.setString(buffer);
}