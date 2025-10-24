#include "Board.hpp"

void BaseGame::Load(void)
{
	m_data = new SceneData;
	m_data->Tile.InitTiled("Assets/Map/map.json");
}

void BaseGame::Unload(void)
{
	delete this->m_data;
	this->m_data = NULL;
}

void BaseGame::PollEvent(sf::Event& _event)
{

}

 void BaseGame::Update(float _deltaTime)
 {
 
 }

 void BaseGame::Draw(sf::RenderWindow& _renderWindow)
 {
	 m_data->Tile.DrawMapLayers(_renderWindow, sf::Vector2f(0.f, 0.f));
 }
