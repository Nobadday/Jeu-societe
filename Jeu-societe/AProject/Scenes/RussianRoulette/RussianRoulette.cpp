#include "RussianRoulette.hpp"


void RussianRoulette::Load(void)
{
	m_data = new SceneData;

	//DEBUG
	const char* players[] = { "Player1", "Player2", "Player3", "Player4" };
	int nbOfPlayers = sizeof(players) / sizeof(players[0]);


	//DEBUG
	m_data->players.push_back({ "Player0", 0, true});
	m_data->players.push_back({ "Player1", 1, true });
	m_data->players.push_back({ "Player2", 2, true });
	m_data->players.push_back({ "Player3", 3, true });



	m_data->bullet = 1 + rand() % 5;
	
	m_data->font.loadFromFile("Assets/Fonts/Platinum Sign.ttf");
	m_data->text.setFont(m_data->font);
	m_data->text.setCharacterSize(15u);
	m_data->text.setOrigin(0,0);



	//m_data->gunTexAnim.LoadFromFile("Assets/Sprites/RussianRoulette/PiouMort.json", TextureAnimated::ANIMATION_ASEPRITE);
	m_data->gunTexAnim.LoadFromFile("Assets/Sprites/RussianRoulette/Damien.texanim", TextureAnimated::ANIMATION_TEXANIM);
	m_data->gunSprAnim.setTexture(m_data->gunTexAnim);
	m_data->gunSprAnim.GetAn().SetFramerate(1.f);

	m_data->gameState = WAITING;
}

void RussianRoulette::Unload(void)
{
	delete this->m_data;
	this->m_data = NULL;
}

void RussianRoulette::PollEvent(sf::Event& _event)
{
	//std::cout << "Gamstate : " << m_data->gameState << std::endl;

	switch (m_data->gameState)
	{
		case WAITING:

			//Check input
			if (_event.type == sf::Event::KeyPressed || _event.type == sf::Event::JoystickButtonPressed)
			{	
				//Check for each player, if it's their turn
				for (int i = 0; i < m_data->players.size(); i++)
				{
					if (m_data->currentPlayer == i)
					{
						//If it's their turn, check for input
						if (_event.joystickButton.joystickId == m_data->players[i].id)
						{
							m_data->text.setString("");
							int randomNb = (1 + rand() % 5);

							//DEBUG
							std::cout << "nbRANDOM = " << randomNb << " bullet = " << m_data->bullet << std::endl;
							std::cout << "player :  = " << m_data->players[i].name << std::endl;

							if (randomNb == m_data->bullet)
							{
								//DEBUG
								std::cout << "player : " << m_data->players[i].name << " killed" << std::endl;
								m_data->players[i].isAlive = false;


								//Launch sound
								//Set and Launch death animation
								m_data->gunSprAnim.SetAnimation("KMS");
							}
							else
							{
								//DEBUG
								std::cout << "vivant" << std::endl;

								//Launch sound
								//Set and Launch normal animation
								m_data->gunSprAnim.SetAnimation("Left_Walk");
							}
							m_data->gameState = SPINNING;
						}
					}
				}
			}
			break;



			//switch (m_data->currentPlayer)
			//{
			//	case 0:
			//		//Attendre input
			//		if (_event.type == sf::Event::KeyPressed)
			//		{
			//			m_data->text.setString("");
			//			int randomNb = (1 + rand() % 5);
			//			//DEBUG
			//			std::cout << "nbRANDOM = " << randomNb << " bullet = "<< m_data->bullet<<  std::endl;
			//			if (randomNb == m_data->bullet)
			//			{
			//				//DEBUG
			//				std::cout << "player : " << m_data->currentPlayer << " killed" << std::endl;
			//				//Launch sound
			//				//Set and Launch death animation
			//				m_data->gunSprAnim.SetAnimation("KMS");
			//				m_data->playerKilled = m_data->currentPlayer;
			//				m_data->killed = true;
			//			}
			//			else 
			//			{
			//				//DEBUG
			//				std::cout << "vivant" << std::endl;
			//				//Launch sound
			//				//Set and Launch normal animation
			//				m_data->gunSprAnim.SetAnimation("Left_Walk");
			//			}
			//			m_data->gameState = SPINNING;
			//		}
			//		break;
			//	case 1:
			//		//Attendre input
			//						//Attendre input
			//		if (_event.type == sf::Event::KeyPressed)
			//		{
			//			m_data->text.setString("");
			//			int randomNb = (1 + rand() % 5);
			//			//DEBUG
			//			std::cout << "nbRANDOM = " << randomNb << " bullet = " << m_data->bullet << std::endl;
			//			if (randomNb == m_data->bullet)
			//			{
			//				//DEBUG
			//				std::cout << "player : " << m_data->currentPlayer << " killed" << std::endl;
			//				//Launch sound
			//				//Set and Launch death animation
			//				m_data->gunSprAnim.SetAnimation("KMS");
			//				m_data->playerKilled = m_data->currentPlayer;
			//				m_data->killed = true;
			//			}
			//			else
			//			{
			//				//DEBUG
			//				std::cout << "vivant" << std::endl;
			//				//Launch sound
			//				//Set and Launch normal animation
			//				m_data->gunSprAnim.SetAnimation("Left_Walk");
			//			}
			//			m_data->gameState = SPINNING;
			//		}
			//		break;
			//	case 2:
			//		//Attendre input
			//		if (_event.type == sf::Event::KeyPressed)
			//		{
			//			m_data->text.setString("");
			//			int randomNb = (1 + rand() % 5);
			//			//DEBUG
			//			std::cout << "nbRANDOM = " << randomNb << " bullet = " << m_data->bullet << std::endl;
			//			if (randomNb == m_data->bullet)
			//			{
			//				//DEBUG
			//				std::cout << "player : " << m_data->currentPlayer << " killed" << std::endl;
			//				//Launch sound
			//				//Set and Launch death animation
			//				m_data->gunSprAnim.SetAnimation("KMS");
			//				m_data->playerKilled = m_data->currentPlayer;
			//				m_data->killed = true;
			//			}
			//			else
			//			{
			//				//DEBUG
			//				std::cout << "vivant" << std::endl;
			//				//Launch sound
			//				//Set and Launch normal animation
			//				m_data->gunSprAnim.SetAnimation("Left_Walk");
			//			}
			//			m_data->gameState = SPINNING;
			//		}
			//		break;
			//	case 3:
			//		//Attendre input
			//		if (_event.type == sf::Event::KeyPressed)
			//		{
			//			m_data->text.setString("");
			//			int randomNb = (1 + rand() % 5);
			//			//DEBUG
			//			std::cout << "nbRANDOM = " << randomNb << " bullet = " << m_data->bullet << std::endl;
			//			if (randomNb == m_data->bullet)
			//			{
			//				//DEBUG
			//				std::cout << "player : " << m_data->currentPlayer << " killed" << std::endl;
			//				//Launch sound
			//				//Set and Launch death animation
			//				m_data->gunSprAnim.SetAnimation("KMS");
			//				m_data->playerKilled = m_data->currentPlayer;
			//				m_data->killed = true;
			//			}
			//			else
			//			{
			//				//DEBUG
			//				std::cout << "vivant" << std::endl;
			//				//Launch sound
			//				//Set and Launch normal animation
			//				m_data->gunSprAnim.SetAnimation("Left_Walk");
			//			}
			//			m_data->gameState = SPINNING;
			//		}
			//		break;
			//}

			break;
		case SPINNING:

			break;

		case END:
			break;

	}
}
void RussianRoulette::Update(float _deltaTime)
{
	switch (m_data->gameState)
	{
		case WAITING:

			//Attendre input
			m_data->text.setString("PRESS ANY BUTTON TO SPIN THE CHAMBER !");
			break;
		case SPINNING:

			m_data->gunSprAnim.Update(_deltaTime);

			//Animation ended
			if (m_data->gunSprAnim.GetAn().IsFinished())
			{
				if (!m_data->players[m_data->currentPlayer].isAlive)
				{
					m_data->gameState = END;

					char buffer[100];
					std::snprintf(buffer, 100, "PLAYER %s IS DEAD", m_data->players[m_data->currentPlayer].name);
					std::cout << buffer << std::endl;
					m_data->text.setString(buffer);

					std::cout << "Game Over ! Player " << m_data->players[m_data->currentPlayer].name << " is dead !" << std::endl;
					//Load bullet for next game, its not useful
					m_data->bullet = 1 + rand() % 5;
				}
				else
				{
					m_data->gameState = WAITING;

					m_data->gunSprAnim.RestartAnimation();

					//Next player



					if (m_data->currentPlayer + 1 < m_data->players.size())
					{
						m_data->currentPlayer++;
					}
					else
					{
						m_data->currentPlayer = 0;
					}
				}








				//if (m_data->killed)
				//{
				//	m_data->gameState = END;


				//	char buffer[100];
				//	std::snprintf(buffer, 100, "PLAYER %d IS DEAD", m_data->playerKilled + 1);
				//	std::cout << buffer << std::endl;
				// 
				//	m_data->text.setString(buffer);

				//	std::cout << "Game Over ! Player " << m_data->playerKilled << " is dead !" << std::endl;
				//	m_data->killed = false;
				//	//Load bullet for next
				//	m_data->bullet = 1 + rand() % 5;
				//}
				//else
				//{
				//	m_data->gameState = WAITING;

				//	m_data->gunSprAnim.RestartAnimation();

				//	//Next player
				//	if (m_data->currentPlayer + 1 < m_data->nbOfPlayers)
				//	{
				//		m_data->currentPlayer++;
				//	}
				//	else
				//	{
				//		m_data->currentPlayer = 0;
				//	}
				//}
			}

		break;
	case END:

		std::cout << "FINI, CHANGEMENT De SCENE ICI" << std::endl;
		break;

	}



}
void RussianRoulette::Draw(sf::RenderWindow& _renderWindow)
{
	m_data->gunSprAnim.draw(_renderWindow, sf::RenderStates::Default);
	_renderWindow.draw(m_data->text);
}