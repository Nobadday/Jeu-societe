#include "Board.hpp"

void BaseGame::Load(void)
{
	m_data = new SceneData;
	m_data->tile.InitTiled("Assets/Map/map.json");
	m_data->camera.Reset(((GameData*)this->m_keptData)->m_renderWindow->getDefaultView());

	MapLayer layer = m_data->tile.GetMapLayer("point");

	m_data->posCase = layer.GetObjects();

	sf::Sprite tempSprite;
	sf::Texture tempT;

	tempT.loadFromFile("Assets/Images/Placeholder.png");

	tempSprite.setTexture(tempT);

	Player tempPlayer;
	tempPlayer.sprite = tempSprite;
	tempPlayer.currentCaseIndex = 0;
	tempPlayer.boardPosition = m_data->posCase[0].GetPosition();

	m_data->players.push_back(tempPlayer);

	m_data->animator.AnimateObject(m_data->players[0].sprite);

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
	 if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
	 {
		 int rando  = 1 + rand() % 6;
		 std::cout << "Roll Dice: " << rando << std::endl;
		 m_data->players[0].currentCaseIndex += rando;
	 }

	 m_data->animator.SetGoTo(m_data->players[0].boardPosition, m_data->posCase[m_data->players[0].currentCaseIndex % m_data->posCase.size()].GetPosition());

	 m_data->animator.Update(_deltaTime);
	 m_data->camera.Move(movement);
 }

 void BaseGame::Draw(sf::RenderWindow& _renderWindow)
 {
	 const sf::View& referenceView = m_data->camera.GetView();
	 _renderWindow.setView(referenceView);
	 m_data->tile.DrawMapLayers(_renderWindow, referenceView.getCenter());
	 for (auto& player : m_data->players)
	 {
		 player.sprite.setPosition(player.boardPosition);

		 _renderWindow.draw(player.sprite);
	 }
 }

