#include "Menu.hpp"

Menu::Menu()
{
	printf("Menu\n");
}
Menu::~Menu()
{
}
void Menu::Update(float _dt)
{

}
void Menu::Draw(sf::RenderWindow* const _renderWindow)
{

}

void Menu::KeyPressed(sf::Event::KeyEvent _key, sf::RenderWindow* _renderWindow)
{
	switch (_key.code)
	{
	case sf::Keyboard::Key::Escape:
		_renderWindow->close();
		break;
	case sf::Keyboard::Key::Space:
		SetGameState(GAME);
		break;
	default:
		break;
	}
}

void Menu::KeyReleased(sf::Event::KeyEvent _key, sf::RenderWindow* _renderWindow)
{

}