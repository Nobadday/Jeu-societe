#ifndef GAME_H
#define GAME_H

#include "Common.hpp"
#include "GameState.hpp"
#include "Camera.hpp"

class Game
{
public:
	Game();
	~Game();
	static void Update(float _dt);
	static void Draw(sf::RenderWindow* const _renderWindow);
	static void KeyPressed(sf::Event::KeyEvent _key, sf::RenderWindow* _renderWindow);
	static void KeyReleased(sf::Event::KeyEvent _key, sf::RenderWindow* _renderWindow);
private:
	static sf::Sprite* backGround;

};

#endif
