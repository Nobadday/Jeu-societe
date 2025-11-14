#include "Podium.hpp"

void Podium::Load()
{
	m_data = new SceneData;

	m_data->m_playerDataList = ((GameData*)this->m_keptData)->m_playerDataList;
	m_data->playerOrder = ((GameData*)this->m_keptData)->m_gonnaPlayIndex;

	m_data->PodiumTexture.loadFromFile("Assets/RockPaperSizor/Podium.png");
	m_data->Podium.setTexture(m_data->PodiumTexture);
	m_data->Podium.setOrigin(sf::Vector2f(m_data->Podium.getGlobalBounds().width/2, m_data->Podium.getGlobalBounds().height));
	m_data->Podium.setPosition(sf::Vector2f(SCREEN_WIDTH/ 2.5f, SCREEN_HEIGHT));


	m_data->font.loadFromFile("Assets/RockPaperSizor/Impact.ttf");
	for (short i = 0; i < 4; i++)
	{
		m_data->playerTab[i].setFont(m_data->font);

		 

		char buffer[10];
		sprintf_s(buffer, "player%d", m_data->playerOrder[i]);
		m_data->playerTab[i].setString(buffer);
	}
}

void Podium::Unload()
{
	delete this->m_data;
	this->m_data = NULL;
}

void Podium::PollEvent(sf::Event& _event)
{
	//this->m_data->menuSystem.PollEvent(_event);
}

void Podium::Update(float _dt)
{
	//this->m_data->menuSystem.Update(_dt);
}

void Podium::Draw(sf::RenderWindow& _renderWindow)
{
	//this->m_data->menuSystem.Draw(_renderWindow, sf::RenderStates::Default);
	_renderWindow.draw(m_data->Podium);
}