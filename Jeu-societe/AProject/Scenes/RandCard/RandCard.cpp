#include "RandCard.hpp"

void RandCard::Load(void)
{
	m_data = new SceneData;

	//DEBUG
	const char* players[] = { "Player1", "Player2", "Player3", "Player4" };
	int nbOfPlayers = sizeof(players) / sizeof(players[0]);


	m_data->font.loadFromFile("Assets/Fonts/Platinum Sign.ttf");
	m_data->text.setFont(m_data->font);
	m_data->text.setCharacterSize(15u);
	m_data->text.setOrigin(0, 0);

	//m_data->gunTexAnim.LoadFromFile("Assets/Sprites/RussianRoulette/PiouMort.json", TextureAnimated::ANIMATION_ASEPRITE);
	m_data->cardTexAnim.LoadFromFile("Assets/Sprites/RussianRoulette/Damien.texanim", TextureAnimated::ANIMATION_TEXANIM);
	m_data->cardSprAnim.setTexture(m_data->cardTexAnim);
	m_data->cardSprAnim.GetAn().SetFramerate(1.f);

	int nbOfNormalCard = 2 * nbOfPlayers;
	int nbOfBomb = 1 * nbOfPlayers;
	m_data->currentNbOfCards = nbOfNormalCard + nbOfBomb;


	for (int i = 0; i < m_data->currentNbOfCards; ++i)
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
	for (int i = 0; i < m_data->currentNbOfCards; ++i)
	{
		std::cout << "Card n " << i << " : " << m_data->cards[i] << std::endl;
	}



}

void RandCard::Unload(void)
{
	delete this->m_data;
	this->m_data = NULL;
}

void RandCard::PollEvent(sf::Event& _event)
{

}
void RandCard::Update(float _deltaTime)
{




}
void RandCard::Draw(sf::RenderWindow& _renderWindow)
{
	//m_data->gunSprAnim.draw(_renderWindow, sf::RenderStates::Default);
	_renderWindow.draw(m_data->text);
}