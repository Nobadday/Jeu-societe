#include "RockPaperScissors.hpp"
#include "../../Utilities/Random.hpp"

void RockPaperSizor::Load()
{
	m_data = new SceneData;
	//PlaceHolders
	m_data->textureTab[0].loadFromFile("Assets/RockPaperSizor/A.png");
	m_data->textureTab[1].loadFromFile("Assets/RockPaperSizor/B.png");
	m_data->textureTab[2].loadFromFile("Assets/RockPaperSizor/X.png");

	m_data->textureTab[3].loadFromFile("Assets/RockPaperSizor/ciseaux.png");
	m_data->textureTab[4].loadFromFile("Assets/RockPaperSizor/caillou.png");
	m_data->textureTab[5].loadFromFile("Assets/RockPaperSizor/papier.png");

	m_data->spriteTab[0].setTexture(m_data->textureTab[0]);
	m_data->spriteTab[1].setTexture(m_data->textureTab[1]);
	m_data->spriteTab[2].setTexture(m_data->textureTab[2]);
	m_data->spriteTab[3].setTexture(m_data->textureTab[3]);
	m_data->spriteTab[4].setTexture(m_data->textureTab[4]);
	m_data->spriteTab[5].setTexture(m_data->textureTab[5]);

	m_data->player1Choice = RPS_NONE;
	m_data->player2Choice = RPS_NONE;

	m_data->timer.SetTimeTarget(2.f);

	m_data->font.loadFromFile("Assets/RockPaperSizor/Impact.ttf");
	m_data->timerText.setFont(m_data->font);

	for (short i = 0; i < 6; i++)
	{
		m_data->spriteTab[i].setPosition({ (float)100 * i, 100.f });
	}

	//a retirer plus tard
	m_data->player1ChoiceSprite.setPosition({ SCREEN_WIDTH * 0.66f, SCREEN_HEIGHT * 0.66f });
	m_data->player2ChoiceSprite.setPosition({ SCREEN_WIDTH * 0.33f, SCREEN_HEIGHT * 0.66f });
	
	m_data->victoryText.setFont(m_data->font);
	m_data->victoryText.setPosition({ SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 });
}                               

void RockPaperSizor::Unload()
{
	delete m_data;
}

void RockPaperSizor::PollEvent(sf::Event& _event)
{
	switch (_event.type)
	{
		case sf::Event::JoystickButtonPressed:
			
			if (m_data->isStarted && !m_data->timer.IsPaused())
			{
				switch (_event.joystickButton.button)
				{
				case 0:
					//Changer valeurs brute par id manettes joueurs
					if (_event.joystickButton.joystickId == 0)
					{
						m_data->player1Choice = RPS_SCISSORS;
					}
					else if (_event.joystickButton.joystickId == 1)
					{
						m_data->player2Choice = RPS_SCISSORS;
					}
					break;
				case 1:
					if (_event.joystickButton.joystickId == 0)
					{
						m_data->player1Choice = RPS_PAPER;
					}
					else if (_event.joystickButton.joystickId == 1)
					{
						m_data->player2Choice = RPS_PAPER;
					}
					break;
				case 2:
					if (_event.joystickButton.joystickId == 0)
					{
						m_data->player1Choice = RPS_ROCK;
					}
					else if (_event.joystickButton.joystickId == 1)
					{
						m_data->player2Choice = RPS_ROCK;
					}
					break;
				}
				break;
			}
			break;
	default:
		break;
	}
}

void RockPaperSizor::Update(float _deltaTime)
{   
	m_data->timer.Update(_deltaTime);

	char buffer[20];
	std::snprintf(buffer, 50, "%02f", m_data->timer.GetRemainingTime());
	m_data->timerText.setString(buffer);

	if (m_data->timer.IsFinished() && !m_data->isStarted && !m_data->timer.IsPaused())
	{
		m_data->isStarted = true;
		m_data->timer.SetTimeTarget(5.f);
		m_data->timer.Restart();

		m_data->player1Choice = (RPS_Choice)random::RandomInt(0,2);
		m_data->player2Choice = (RPS_Choice)random::RandomInt(0, 2);
	}
	else if (m_data->timer.IsFinished() && m_data->isStarted)
	{
		m_data->isStarted = false;
		if (m_data->player1Choice != m_data->player2Choice)
		{
			m_data->timer.TogglePause();

			char buffer2[15];

			if (m_data->player1Choice == RPS_SCISSORS && m_data->player2Choice == RPS_PAPER
				|| m_data->player2Choice == RPS_SCISSORS && m_data->player1Choice == RPS_ROCK
				|| m_data->player1Choice == RPS_PAPER && m_data->player2Choice == RPS_ROCK)
			{
				std::snprintf(buffer2, 50, "PLAYER 1 WIN");
			}
			else
			{
				std::snprintf(buffer2, 50, "PLAYER 2 WIN");
			}

			m_data->victoryText.setString(buffer2);
		}
	}
}

void RockPaperSizor::Draw(sf::RenderWindow& _renderWindow)
{
	for (short i = 0; i < 6; i++)
	{
		_renderWindow.draw(m_data->spriteTab[i]);
	}

	switch (m_data->player1Choice)
	{
		case RPS_ROCK:
			m_data->player1ChoiceSprite.setTexture(m_data->textureTab[4]);
			break;
		case RPS_PAPER:
			m_data->player1ChoiceSprite.setTexture(m_data->textureTab[3]);
			break;
		case RPS_SCISSORS:
			m_data->player1ChoiceSprite.setTexture(m_data->textureTab[5]);
			break;
		default:
			break;
	}

	switch (m_data->player2Choice)
	{
		case RPS_ROCK:
			m_data->player2ChoiceSprite.setTexture(m_data->textureTab[4]);
			break;
		case RPS_PAPER:
			m_data->player2ChoiceSprite.setTexture(m_data->textureTab[3]);
			break;
		case RPS_SCISSORS:
			m_data->player2ChoiceSprite.setTexture(m_data->textureTab[5]);
			break;
		default:
			break;
	}
	
	_renderWindow.draw(m_data->player1ChoiceSprite);
	_renderWindow.draw(m_data->player2ChoiceSprite);

	_renderWindow.draw(m_data->victoryText);

	_renderWindow.draw(m_data->timerText);
}