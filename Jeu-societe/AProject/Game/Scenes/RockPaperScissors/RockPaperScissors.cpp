#include "RockPaperScissors.hpp"
#include "../../Utilities/Random.hpp"

#define PLAY_TIME 5
#define PAUSE_TIME 2

void RockPaperScissors::Load()
{
	m_data = new SceneData;
	//PlaceHolders
	m_data->textureTab[0].loadFromFile("Assets/RockPaperSizor/A.png");
	m_data->textureTab[1].loadFromFile("Assets/RockPaperSizor/B.png");
	m_data->textureTab[2].loadFromFile("Assets/RockPaperSizor/X.png");

	m_data->textureTab[3].loadFromFile("Assets/RockPaperSizor/ciseaux.png");
	m_data->textureTab[4].loadFromFile("Assets/RockPaperSizor/caillou.png");
	m_data->textureTab[5].loadFromFile("Assets/RockPaperSizor/papier.png");

	for (short i = 0; i < 6; i++)
	{
		m_data->spriteTab[i].setTexture(m_data->textureTab[i]);
		m_data->spriteTab[i].setPosition({ (float)100 * i, 100.f });
	}

	m_data->font.loadFromFile("Assets/RockPaperSizor/Impact.ttf");
	m_data->timerText.setFont(m_data->font);

	//a retirer plus tard
	m_data->playerChoiceSprite[0].setPosition({SCREEN_WIDTH * 0.66f, SCREEN_HEIGHT * 0.66f});
	m_data->playerChoiceSprite[1].setPosition({SCREEN_WIDTH * 0.33f, SCREEN_HEIGHT * 0.66f});

	m_data->victoryText.setFont(m_data->font);
	m_data->victoryText.setPosition({ SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 });

	
	//Temporaire
	//m_data->gameData->AddPlayerPlaying(0);
	((GameData*)this->m_keptData)->m_gonnaPlayIndex.push_back(0);
	((GameData*)this->m_keptData)->m_gonnaPlayIndex.push_back(1);

	m_data->gameData = (GameData*)this->m_keptData;
	m_data->timer.SetTimeTarget(5);

	m_data->state = STATE_PLAY;
	m_data->roundNB = 0;

}                               

void RockPaperScissors::Unload()
{
	delete m_data;
}

void RockPaperScissors::PollEvent(sf::Event& _event)
{
	switch (_event.type)
	{
		if (m_data->state == STATE_PLAY)
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

	if (this->m_data->state == STATE_PLAY)
	{
		if (m_data->timer.IsFinished())
		{
			if (this->m_data->playersChoice[this->m_data->gameData->m_gonnaPlayIndex[0]] == this->m_data->playersChoice[this->m_data->gameData->m_gonnaPlayIndex[1]])
			{
				if (this->m_data->roundNB < 2)
				{
					this->UpdatePlayerChoiceTexture();
					this->m_data->state = STATE_PAUSE;
					this->m_data->roundNB++;
					this->m_data->timer.SetTimeTarget(PAUSE_TIME, true);
				}
				else
				{
					this->UpdatePlayerChoiceTexture();
					this->m_data->state = STATE_EQUALITY;
					this->m_data->timer.SetTimeTarget(PAUSE_TIME, true);
				}
			}
			else
			{
				this->UpdatePlayerChoiceTexture();
				this->m_data->timer.SetTimeTarget(PAUSE_TIME, true);
				this->m_data->state = STATE_VICTORY;	
			}
		}
	}
	else if (this->m_data->state == STATE_PAUSE)
	{
		if (this->m_data->timer.IsFinished())
		{
			this->m_data->timer.SetTimeTarget(PLAY_TIME, true);
			this->m_data->state = STATE_PLAY;
		}
	}
	else if (this->m_data->state == STATE_VICTORY)
	{
		if (this->m_data->timer.IsFinished())
		{
			if (this->m_data->playersChoice[this->m_data->gameData->m_gonnaPlayIndex[0]] > this->m_data->playersChoice[this->m_data->gameData->m_gonnaPlayIndex[1]]
				|| m_data->playersChoice[m_data->gameData->m_gonnaPlayIndex[0]] == RPS_ROCK && this->m_data->playersChoice[this->m_data->gameData->m_gonnaPlayIndex[1]] == RPS_SCISSORS)
			{
				std::cout << "player " << this->m_data->gameData->m_gonnaPlayIndex[0] + 1 << " win";
				this->m_data->gameData->AddPlayerWin(this->m_data->gameData->m_gonnaPlayIndex[0]);
				this->m_data->gameData->AddPlayerWin(this->m_data->gameData->m_gonnaPlayIndex[1]);
			}
			else
			{
				std::cout << "player " << this->m_data->gameData->m_gonnaPlayIndex[1] + 1 << " win";
				this->m_data->gameData->AddPlayerWin(this->m_data->gameData->m_gonnaPlayIndex[1]);
				this->m_data->gameData->AddPlayerWin(this->m_data->gameData->m_gonnaPlayIndex[0]);
			}

			ChangeScene("Board", false);
		}
	}
	else if (this->m_data->state == STATE_EQUALITY)
	{
		if (this->m_data->timer.IsFinished())
		{
			ChangeScene("Board", false);
		}
	}
}

void RockPaperScissors::Draw(sf::RenderWindow& _renderWindow)
{
	for (short i = 0; i < 6; i++)
	{
		_renderWindow.draw(m_data->spriteTab[i]);
	}

	if (this->m_data->state != STATE_PLAY)
	{
		for (short i = 0; i < 2; i++)
		{
			_renderWindow.draw(this->m_data->playerChoiceSprite[i]);
		}
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
				this->m_data->playerChoiceSprite[i].setTexture(this->m_data->textureTab[4]);
				break;
			case RPS_PAPER:
				this->m_data->playerChoiceSprite[i].setTexture(this->m_data->textureTab[5]);
				break;
			case RPS_SCISSORS:
				this->m_data->playerChoiceSprite[i].setTexture(this->m_data->textureTab[3]);
				break;
			default:
				break;
		}
	}
}