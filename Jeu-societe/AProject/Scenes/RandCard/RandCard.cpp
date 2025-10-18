#include "RandCard.hpp"

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
			m_data->cardSprAnim.draw(_renderWindow, sf::RenderStates::Default);
		}
		else
		{
			m_data->staticCardSpr.setPosition(BORDER_X + cardSpacing * i, SCREEN_HEIGHT / 4.f);
			_renderWindow.draw(m_data->staticCardSpr, sf::RenderStates::Default);
		}
	}

}

void RandCard::Load(void)
{
	m_data = new SceneData;

	//DEBUG
	m_data->players.push_back({ "Player1"});
	m_data->players.push_back({ "Player2"});
	m_data->players.push_back({ "Player3"});
	m_data->players.push_back({ "Player4"});

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
	for (int i = 0; i < m_data->cards.size(); ++i)
	{
		std::cout << "Card n " << i << " : " << m_data->cards[i] << std::endl;
	}

}

void RandCard::Unload(void)
{
	m_data->cards.clear();
	m_data->players.clear();



	delete this->m_data;
	this->m_data = NULL;
}

void RandCard::PollEvent(sf::Event& _event)
{
	if (m_data->gameState == CHOOSE_CARD)
	{
		//Restart card, when you comme from the last animation
		m_data->cardSprAnim.SetAnimation("OFF");

		if (_event.type == sf::Event::KeyPressed)
		{
			//Move the position of card chosen to the right
			if (_event.key.code == sf::Keyboard::D)
			{
				m_data->cardChosen = (m_data->cardChosen + 1 > m_data->cards.size()) ? 0 : m_data->cardChosen + 1;
			}
			//Move the position of card chosen to the left
			else if (_event.key.code == sf::Keyboard::Q)
			{
				m_data->cardChosen = (m_data->cardChosen - 1 < 0) ? int(m_data->cards.size()) : m_data->cardChosen - 1;
			}
			//Confirm choice
			if (_event.key.code == sf::Keyboard::Enter)
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
				m_data->cardSprAnim.RestartAnimation();
			}
		}
	}



}
void RandCard::Update(float _deltaTime)
{
	std::cout << "Card Vector size : " << m_data->cards.size() << std::endl;
	std::cout << "Player Vector size : " << m_data->players.size() << std::endl;


	switch (m_data->gameState)
	{
		case CHOOSE_CARD:




			break;

		case ANIMATION:

			m_data->cardSprAnim.Update(_deltaTime);
			if (m_data->cardSprAnim.GetAn().IsFinished())
			{
				if (m_data->cards[m_data->cardChosen] == BOMB)
				{
					//DEBUG
					std::cout << "Player " << m_data->players[m_data->currentPlayer].name << " lost !" << std::endl;
					m_data->players.erase(m_data->players.begin() + m_data->currentPlayer);
				}
				else
				{
					//DEBUG
					std::cout << "Player " << m_data->players[m_data->currentPlayer].name << " survived !" << std::endl;
				}

				m_data->cards.erase(m_data->cards.begin() + m_data->cardChosen);
				m_data->cardChosen = -1;

				if (m_data->players.size() == 0)
				{
					//DEBUG
					std::cout << "Player " << m_data->players[m_data->currentPlayer].name << " is the winner !" << std::endl;
				}

				if (m_data->currentPlayer == m_data->players.size())
				{
					m_data->currentPlayer = 0;
				}
				else
				{
					m_data->currentPlayer++;
				}


				m_data->gameState = CHOOSE_CARD;
			}
			break;
	}



}
void RandCard::Draw(sf::RenderWindow& _renderWindow)
{
	_renderWindow.draw(m_data->text);
	PrintCards(_renderWindow);
}