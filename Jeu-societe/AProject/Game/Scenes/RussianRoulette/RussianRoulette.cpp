#include "./RussianRoulette.hpp"
#include "../../../Utilities/Random.hpp"


void RussianRoulette::Load(void)
{
	m_data = new SceneData;

	m_data->gameData = (GameData*)this->m_keptData;

	//DEBUG
	std::string playersNames[4] = { "Yann", "Lorenzo", "Kyllian", "Damien" };
	//m_data->gameData->m_gonnaPlayIndex.push_back(0);
	//m_data->gameData->m_gonnaPlayIndex.push_back(1);

	int nbOfPlayers = (int)m_data->gameData->m_gonnaPlayIndex.size();

	//Copy players playing from GameData
	for (int i = 0; i < nbOfPlayers; ++i)
	{
		int playerId = m_data->gameData->m_gonnaPlayIndex.at(i);
		m_data->players.push_back({ playersNames[i],  (short)playerId, true });
	}

	m_data->currentPlayer = 0;

	m_data->bullet = random::RandomInt(1, 6);
	
	m_data->font.loadFromFile("Assets/Fonts/Platinum Sign.ttf");
	m_data->text.setFont(m_data->font);
	m_data->text.setCharacterSize(15u);
	m_data->text.setOrigin(0,0);

	//m_data->gunTexAnim.LoadFromFile("Assets/Sprites/RussianRoulette/PiouMort.json", TextureAnimated::ANIMATION_ASEPRITE);
	m_data->gunTexAnim.LoadFromFile("Assets/Sprites/RussianRoulette/Damien.texanim", TextureAnimated::ANIMATION_TEXANIM);
	m_data->gunSprAnim.setTexture(m_data->gunTexAnim);
	//m_data->gunSprAnim.SetFramerate(1.f);

	m_data->gameState = WAITING;
}

void RussianRoulette::Unload(void)
{
	delete this->m_data;
	this->m_data = NULL;
}

void RussianRoulette::PollEvent(sf::Event& _event)
{
	int joyId = m_data->gameData->m_playerDataList[m_data->players[m_data->currentPlayer].id].m_joystickId;

	switch (m_data->gameState)
	{
		case WAITING:

			switch (_event.type)
			{
				//Debug ;)
				case sf::Event::KeyPressed:


					//break;
				case sf::Event::JoystickButtonPressed:

					//Check for each player, if it's their turn
					/*for (int i = 0; i < m_data->players.size(); i++)
					{*/
					

						if (joyId == _event.joystickButton.joystickId)
						{
							//If it's their turn, check for input
							//if (_event.joystickButton.joystickId == m_data->players[i].id)
							//{
								m_data->text.setString("");
								int randomNb = random::RandomInt(1,6);

								//DEBUG
								std::cout << "nbRANDOM = " << randomNb << " bullet = " << m_data->bullet << std::endl;
								std::cout << "player :  = " << m_data->players[m_data->currentPlayer].name << std::endl;

								if (randomNb == m_data->bullet)
								{
									//DEBUG
									std::cout << "player : " << m_data->players[m_data->currentPlayer].name << " killed" << std::endl;
									m_data->players[m_data->currentPlayer].isAlive = false;


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
							//}
						}
					/*}*/
					break;
				default:
					break;
			}

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
			if (m_data->gunSprAnim.IsFinished())
			{
				if (!m_data->players[m_data->currentPlayer].isAlive)
				{
					m_data->gameState = END;

					char buffer[100];
					std::snprintf(buffer, 100, "PLAYER %s IS DEAD", m_data->players[m_data->currentPlayer].name.c_str());
					std::cout << buffer << std::endl;
					m_data->text.setString(buffer);

					std::cout << "Game Over ! Player " << m_data->players[m_data->currentPlayer].name << " is dead !" << std::endl;
					m_data->deadPlayers.push_back(m_data->players[m_data->currentPlayer]);
					

					//DEBUG juste pour faire fonctionner
					//Va falloir que j'améliore car pas beau
					// 
					//Add next player to dead player, its winer
					if (m_data->currentPlayer + 1 < m_data->players.size())
					{
						m_data->currentPlayer++;
					}
					else
					{
						m_data->currentPlayer = 0;
					}

					m_data->deadPlayers.push_back(m_data->players[m_data->currentPlayer]);





					//Save data
					int nbOfPlayers = (int)m_data->gameData->m_gonnaPlayIndex.size() - 1;
					std::cout << "nb player : " << nbOfPlayers << std::endl;

					for (int i = 0; i < nbOfPlayers; i++)
					{

						m_data->gameData->AddPlayerWin(m_data->deadPlayers.at(nbOfPlayers - i).id);
					}




					//Load bullet for next game, its not useful
					m_data->bullet = random::RandomInt(1, 6);
				}
				else
				{
					m_data->gameState = WAITING;

					m_data->gunSprAnim.Restart();

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
			}

		break;
	case END:

		std::cout << "FINI, CHANGEMENT De SCENE ICI" << std::endl;
		SceneBase::ChangeScene("Board", false);
		break;
	}
}
void RussianRoulette::Draw(sf::RenderWindow& _renderWindow)
{


	_renderWindow.draw(m_data->gunSprAnim);
	_renderWindow.draw(m_data->text);
}