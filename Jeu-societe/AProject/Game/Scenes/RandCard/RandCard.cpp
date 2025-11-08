#include "RandCard.hpp"
#include "../../../Utilities/MathPlus.hpp"

#define CARD_SIZE_X 40.f
#define CARD_SIZE_Y 80.f
#define BORDER_X 40.f


void RandCard::PrintCards(sf::RenderWindow& _renderWindow)
{
	float cardSpacing = (SCREEN_WIDTH - 2 * BORDER_X) / m_data->cards.size();


	for (int i = 0; i < m_data->cards.size(); ++i)
	{
		if (i == m_data->cardChosen)
		{
			m_data->cardSprAnim.setPosition(BORDER_X + cardSpacing * i, SCREEN_HEIGHT / 4.f);
			//m_data->cardSprAnim.draw(_renderWindow, sf::RenderStates::Default);
			_renderWindow.draw(m_data->cardSprAnim);
		}
		else
		{
			m_data->staticCardSpr.setPosition(BORDER_X + cardSpacing * i, SCREEN_HEIGHT / 4.f);
			_renderWindow.draw(m_data->staticCardSpr, sf::RenderStates::Default);
		}
	}

}

void RandCard::SetCardChosen(int _card)
{
	m_data->cardChosen = mathp::ModuloPositiveI(_card, (int)m_data->cards.size());
}
void RandCard::AddCardChosen(int _value)
{
	this->SetCardChosen(m_data->cardChosen + _value);
}

void RandCard::NextPlayer(void)
{
	if (m_data->currentPlayer >= m_data->players.size())
	{
		//Check if the current player was the last in the vector 
		m_data->currentPlayer = 0;
	}
	else
	{
		//Next player
		m_data->currentPlayer++;
	}
	m_data->cardChosen = 0;
	m_data->gameState = CHOOSE_CARD;
}

void RandCard::Load(void)
{
	m_data = new SceneData;

	m_data->menuSystem = new MenuSystem();
	m_data->menuSystem->MenuAdd("TestMenu", true);


	// Liaison au GameData passé dans m_keptData (comme dans RockPaperScissors)
	m_data->gameData = (GameData*)this->m_keptData;

	//Debug names
	std::string playersNames[4] = { "Yann", "Lorenzo", "Kyllian", "Damien" };
	((GameData*)this->m_keptData)->m_gonnaPlayIndex.push_back(0);
	((GameData*)this->m_keptData)->m_gonnaPlayIndex.push_back(1);

	int nbOfPlayers = ((GameData*)this->m_keptData)->m_gonnaPlayIndex.size();
	std::cout << "nb of player " << nbOfPlayers << std::endl;

	//Copy players playing from GameData
	for (int i = 0; i < nbOfPlayers; ++i)
	{
		int playerId = (int)((GameData*)this->m_keptData)->m_gonnaPlayIndex.at(i);
		m_data->players.push_back({ playersNames[i],  (short)playerId });
	}

	m_data->buttonTest = new Button();
	
	m_data->textanim = new TextureAnimated();
	m_data->textanim->LoadFromFile("Assets/Sprites/ButtonPlaceHolder.anim", TextureAnimated::AnimationType::ANIMATION_ANIM);
	m_data->buttonTest->setTexture(*m_data->textanim);

	m_data->menuSystem->MenuAddButton("TestMenu", "ButtonTest", m_data->buttonTest);
	m_data->menuSystem->SetMenuHolder("TestMenu");

	//Font
	m_data->font.loadFromFile("Assets/Fonts/Platinum Sign.ttf");
	m_data->text.setFont(m_data->font);
	m_data->text.setCharacterSize(15u);
	m_data->text.setOrigin(0, 0);

	//Static Card
	m_data->staticCardTex.loadFromFile("Assets/Sprites/RandomCard/StaticCard.png");
	m_data->staticCardSpr.setTexture(m_data->staticCardTex);
	 	 
	//Anim Card
	m_data->cardTexAnim.LoadFromFile("Assets/Sprites/RandomCard/CardData.texanim", TextureAnimated::ANIMATION_TEXANIM);
	m_data->cardSprAnim.setTexture(m_data->cardTexAnim);
	m_data->cardSprAnim.setScale(1.2f, 1.2f);


	int nbOfNormalCard = 2 * int(m_data->players.size());
	int nbOfBomb = 1 * int(m_data->players.size());
	int totalCard = nbOfNormalCard + nbOfBomb;

	//Initialize random cards
	for (int i = 0; i < totalCard; ++i)
	{
		if (nbOfNormalCard > 0 && nbOfBomb > 1)
		{
			int randomNb = rand() % 2;
			if (randomNb == 0)
			{
				m_data->cards.push_back(NORMAL);
				nbOfNormalCard--;
			}
			else
			{
				m_data->cards.push_back(BOMB);
				nbOfBomb--;
			}
		}
		else if (nbOfNormalCard > 0)
		{
			m_data->cards.push_back(NORMAL);
			nbOfNormalCard--;
		}
		else
		{
			m_data->cards.push_back(BOMB);
			nbOfBomb--;
		}
	}
	//DEBUG : print all cards in terminal
	for (int i = 0; i < m_data->cards.size(); ++i)
	{
		std::cout << "Card n " << i << " : " << m_data->cards[i] << std::endl;
	}

}
void RandCard::Unload(void)
{
	m_data->cards.clear();
	m_data->players.clear();
	m_data->deadPlayers.clear();

	delete this->m_data;
	this->m_data = NULL;
}

void RandCard::PollEvent(sf::Event& _event)
{
	m_data->menuSystem->PollEvent(_event);

	switch (m_data->gameState)
	{
		case CHOOSE_CARD:

			//Restart card, when you comme from the last animation
			m_data->cardSprAnim.SetAnimation("OFF");

			switch (_event.type)
			{
				case sf::Event::JoystickButtonPressed:

					if (_event.joystickButton.joystickId == m_data->currentPlayer)
					{
						//Confirm choice
						if (m_data->cardChosen != -1)
						{
							CardType cardType = m_data->cards[m_data->cardChosen];
							switch (cardType)
							{
							case NORMAL:
								m_data->cardSprAnim.SetAnimation("NORMAL");
								break;
							case BOMB:
								m_data->cardSprAnim.SetAnimation("BOMB");
								break;
							}
							m_data->gameState = ANIMATION;
							//m_data->cardSprAnim.RestartAnimation();
							m_data->cardSprAnim.Restart();
						}
					}
					break;
				case sf::Event::KeyPressed:
					break;
				case sf::Event::JoystickMoved:

					//X Y joystick gauche
					//U V joystick droite
					//Z R pression des gachettes
					//La croix povX povY

					if (_event.joystickButton.joystickId == m_data->currentPlayer)
					{

						if (m_data->inputDelay > 0.1f)
						{
							switch (_event.joystickMove.axis)
							{
								case sf::Joystick::Axis::X:
								case sf::Joystick::Axis::U:

									//Move the position of card chosen
									AddCardChosen((int)roundf(_event.joystickMove.position / 100.f));
									m_data->inputDelay = 0.f;
									break;

								default:
									break;
							}
						}
					}
					break;

				default:
					break;
			}
			break;

		case END:
			if (_event.type == sf::Event::KeyPressed)
			{
				if (_event.key.code == sf::Keyboard::Enter)
				{
					//Return to main menu
					ChangeScene("RussianRoulette");
				}
			}
	}
}
void RandCard::Update(float _deltaTime)
{
	//m_data->buttonTest->Update(_deltaTime);
	m_data->menuSystem->Update(_deltaTime);

	switch (m_data->gameState) 
	{
		case CHOOSE_CARD:

			m_data->inputDelay += _deltaTime;

			break;

		case ANIMATION:

			m_data->cardSprAnim.Update(_deltaTime);
			if (m_data->cardSprAnim.IsFinished())
			{
				//std::cout << "Current player : " << m_data->currentPlayer << std::endl;
				//std::cout << "Player Vector size : " << m_data->players.size() << std::endl;

				if (m_data->cards[m_data->cardChosen] == BOMB)
				{
					m_data->deadPlayers.push_back(m_data->players.at(m_data->currentPlayer));
					m_data->players.erase(m_data->players.begin() + m_data->currentPlayer);

					NextPlayer();
					
					//Check if only one player left
					if (m_data->players.size() <= 1)
					{
						m_data->deadPlayers.push_back(m_data->players.at(0));
						//Print if you want check
						//std::cout << "Player " << m_data->players[0].name << " is the winner !" << std::endl;
						m_data->gameState = END;


						//Save data
						int nbOfPlayers = m_data->gameData->m_gonnaPlayIndex.size();
						for (int i = 0 ; i < nbOfPlayers; i++)
						{
							//Print if you want check
							//std::cout << "END, player rank" << i << " player : " << m_data->deadPlayers.at(i).id << " name :" << m_data->deadPlayers.at(i).name << std::endl;
							m_data->gameData->AddPlayerWin(m_data->deadPlayers.at(i).id);
						}
						ChangeScene(1);
						return;
					}					
				}
				else
				{
					//DEBUG
					//std::cout << "Player " << m_data->players[m_data->currentPlayer].name << " survived !" << std::endl << std::endl;
					m_data->cards.erase(m_data->cards.begin() + m_data->cardChosen);


					if (m_data->currentPlayer + 1 >= m_data->players.size())
					{
						m_data->currentPlayer = 0;
					}
					else
					{
						m_data->currentPlayer++;
					}
					m_data->cardChosen = -1;
					m_data->gameState = CHOOSE_CARD;
				}
			}
			break;
	}
}
void RandCard::Draw(sf::RenderWindow& _renderWindow)
{
	m_data->menuSystem->Draw(_renderWindow, sf::RenderStates::Default);
	_renderWindow.draw(m_data->text);
	PrintCards(_renderWindow);
}