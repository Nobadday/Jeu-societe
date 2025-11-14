#include "Podium.hpp"

void Podium::Load()
{
	m_data = new SceneData;
	m_data->clsS.setPosition(100, 0);
}

void Podium::Unload()
{
	delete this->m_data;
	this->m_data = NULL;
}

void Podium::PollEvent(sf::Event& _event)
{

}

void Podium::Update(float _dt)
{

}

void Podium::Draw(sf::RenderWindow& _renderWindow)
{

}