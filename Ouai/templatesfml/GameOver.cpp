#include "GameOver.hpp"

GameOver::GameOver()
{
	printf("GameOver\n");
}
GameOver::~GameOver()
{
}
void GameOver::Update(float _dt)
{

}
void GameOver::Draw(sf::RenderWindow* const _renderWindow)
{

}

void GameOver::KeyPressed(sf::Event::KeyEvent _key, sf::RenderWindow* _renderWindow)
{
	switch (_key.code)
	{
		case sf::Keyboard::Key::Escape:
			SetGameState(MENU);
		break;
		case sf::Keyboard::Key::Space:
			SetGameState(GAME);
			break;
	default:
		break;
	}
}

void GameOver::KeyReleased(sf::Event::KeyEvent _key, sf::RenderWindow* _renderWindow)
{

}