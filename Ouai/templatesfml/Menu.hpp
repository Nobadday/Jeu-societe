#ifndef MENU_H
#define MENU_H

#include "Common.hpp"
#include "GameState.hpp"

class Menu
{
public:
	Menu();
	~Menu();

	static void Update(float _dt);
	static void Draw(sf::RenderWindow* const _renderWindow);
	static void KeyPressed(sf::Event::KeyEvent _key, sf::RenderWindow* _renderWindow);
	static void KeyReleased(sf::Event::KeyEvent _key, sf::RenderWindow* _renderWindow);
};

#endif // !MENU_H
