#include "FlagMiniGame.hpp"

void FlagMiniGame::Load()
{
	m_data = new SceneData;
}

void FlagMiniGame::Unload()
{
	delete this->m_data;
	this->m_data = NULL;
}

void FlagMiniGame::PollEvent(sf::Event& _event)
{
	
}

void FlagMiniGame::Update(float _deltaTime)
{
	
}

void FlagMiniGame::Draw(sf::RenderWindow& _renderWindow)
{

}