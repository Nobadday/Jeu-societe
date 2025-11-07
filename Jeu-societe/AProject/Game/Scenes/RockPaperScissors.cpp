#include "RockPaperScissors.hpp"
#include "../../Utilities/Random.hpp"

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
	//m_data->player1ChoiceSprite.setPosition({ SCREEN_WIDTH * 0.66f, SCREEN_HEIGHT * 0.66f });
	//m_data->player2ChoiceSprite.setPosition({ SCREEN_WIDTH * 0.33f, SCREEN_HEIGHT * 0.66f });

	m_data->victoryText.setFont(m_data->font);
	m_data->victoryText.setPosition({ SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 });

	m_data->gameData = (GameData*)this->m_keptData;
	
	//Temporaire
	m_data->gameData->AddPlayerPlaying(0);
	((GameData*)this->m_keptData)->m_gonnaPlayIndex.push_back(0);



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
				//redémarrer la partie
				// 
				//this->m_data->timer.Restart();
			}
			else
			{
				if (this->m_data->playersChoice[this->m_data->gameData->m_gonnaPlayIndex[0]] > this->m_data->playersChoice[this->m_data->gameData->m_gonnaPlayIndex[1]]
					|| m_data->playersChoice[m_data->gameData->m_gonnaPlayIndex[0]] == RPS_ROCK && this->m_data->playersChoice[this->m_data->gameData->m_gonnaPlayIndex[1]] == RPS_SCISSORS)
				{
					std::cout << "player " << this->m_data->playersChoice[this->m_data->gameData->m_gonnaPlayIndex[0]] << " win";
					this->m_data->gameData->AddPlayerWin(this->m_data->gameData->m_gonnaPlayIndex[0]);
					this->m_data->gameData->AddPlayerWin(this->m_data->gameData->m_gonnaPlayIndex[1]);
				}
				else
				{
					std::cout << "player " << this->m_data->playersChoice[this->m_data->gameData->m_gonnaPlayIndex[1]] << " win";
					this->m_data->gameData->AddPlayerWin(this->m_data->gameData->m_gonnaPlayIndex[1]);
					this->m_data->gameData->AddPlayerWin(this->m_data->gameData->m_gonnaPlayIndex[0]);
				}

				this->m_data->state = STATE_VICTORY;
			}
		}
	}

	//	m_data->timer.SetPause(false);
	//	if (m_data->timer.IsFinished() && m_data->timer.GetTimeTarget() == INIT_TIME)
	//	{
	//		//fin du warm up
	//		m_data->timer.SetTimeTarget(PLAY_TIME);
	//		m_data->timer.Restart();
	//	}
	//	else if (m_data->timer.IsFinished() && m_data->timer.GetTimeTarget() == PLAY_TIME)
	//	{
	//		//fin du temps de jeu
	//		if (m_data->player1Choice != m_data->player2Choice)
	//		{
	//			char buffer2[15];
	//			if (m_data->player1Choice == RPS_SCISSORS && m_data->player2Choice == RPS_PAPER
	//				|| m_data->player2Choice == RPS_SCISSORS && m_data->player1Choice == RPS_ROCK
	//				|| m_data->player1Choice == RPS_PAPER && m_data->player2Choice == RPS_ROCK)
	//			{
	//				std::snprintf(buffer2, 15, "PLAYER 1 WIN");
	//			}
	//			else
	//			{
	//				std::snprintf(buffer2, 15, "PLAYER 2 WIN");
	//			}
	//			m_data->timer.SetTimeTarget(0);
	//			m_data->timer.SetPause(true);
	//			m_data->victoryText.setString(buffer2);
	//		}
	//		else
	//		{
	//			m_data->player1Choice = (RPS_Choice)random::RandomInt(0, 2);
	//			m_data->player2Choice = (RPS_Choice)random::RandomInt(0, 2);
	//			m_data->timer.Restart();

	//			//retourner le gagnant
	//		}
	//	}
	//}
}

void RockPaperScissors::Draw(sf::RenderWindow& _renderWindow)
{
	for (short i = 0; i < 6; i++)
	{
		_renderWindow.draw(m_data->spriteTab[i]);
	}

	//switch (m_data->player1Choice)
	//{
	//	case RPS_ROCK:
	//		m_data->player1ChoiceSprite.setTexture(m_data->textureTab[4]);
	//		break;
	//	case RPS_PAPER:
	//		m_data->player1ChoiceSprite.setTexture(m_data->textureTab[3]);
	//		break;
	//	case RPS_SCISSORS:
	//		m_data->player1ChoiceSprite.setTexture(m_data->textureTab[5]);
	//		break;
	//	default:
	//		break;
	//}

	//switch (m_data->player2Choice)
	//{
	//	case RPS_ROCK:
	//		m_data->player2ChoiceSprite.setTexture(m_data->textureTab[4]);
	//		break;
	//	case RPS_PAPER:
	//		m_data->player2ChoiceSprite.setTexture(m_data->textureTab[3]);
	//		break;
	//	case RPS_SCISSORS:
	//		m_data->player2ChoiceSprite.setTexture(m_data->textureTab[5]);
	//		break;
	//	default:
	//		break;
	//}
	//
	//_renderWindow.draw(m_data->player1ChoiceSprite);
	//_renderWindow.draw(m_data->player2ChoiceSprite);

	//_renderWindow.draw(m_data->victoryText);

	_renderWindow.draw(m_data->timerText);
}