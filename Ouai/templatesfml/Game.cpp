#include "Game.hpp"


Game::Game()
{

}
Game::~Game()
{

}

void Game::Update(float _dt)
{

}
void Game::Draw(sf::RenderWindow* const _renderWindow)
{

}

void Game::KeyPressed(sf::Event::KeyEvent _key, sf::RenderWindow* _renderWindow)
{
	switch (_key.code)
	{
	case sf::Keyboard::Key::Escape:
		SetGameState(MENU);
		break;
	default:
		break;
	}
}

void Game::KeyReleased(sf::Event::KeyEvent _key, sf::RenderWindow* _renderWindow)
{
}