#include "Board.hpp"

void BaseGame::Load(void)
{
	m_data = new SceneData;
	m_data->tile.InitTiled("Assets/Map/map.json");
	m_data->camera.Reset(((GameData*)this->m_keptData)->m_renderWindow->getDefaultView());
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
	 sf::Vector2f movement(0.f, 0.f);
	 if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
	 {
		 movement.y -= 100.f * _deltaTime;
		 //std::cout << "Up\n";
	 }
	 if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	 {
		 movement.y += 100.f * _deltaTime;
		 //std::cout << "Down\n";
	 }
	 if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
	 {
		 movement.x -= 100.f * _deltaTime;
		 //std::cout << "Left\n";
	 }
	 if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	 {
		 movement.x += 100.f * _deltaTime;
		 //std::cout << "Right\n";
	 }

	 m_data->camera.Move(movement);
 }

 void BaseGame::Draw(sf::RenderWindow& _renderWindow)
 {
	 const sf::View& referenceView = m_data->camera.GetView();
	 _renderWindow.setView(referenceView);
	 m_data->tile.DrawMapLayers(_renderWindow, referenceView.getCenter());
 }
