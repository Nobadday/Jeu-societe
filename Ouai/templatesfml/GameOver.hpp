#ifndef GAME_OVER_H
#define GAME_OVER_H

#include "Common.hpp"
#include "GameState.hpp"

class GameOver
{
public:
	GameOver();
	~GameOver();
	static void Update(float _dt);
	static void Draw(sf::RenderWindow* const _renderWindow);
	static void KeyPressed(sf::Event::KeyEvent _key, sf::RenderWindow* _renderWindow);
	static void KeyReleased(sf::Event::KeyEvent _key, sf::RenderWindow* _renderWindow);
};

#endif // !GAME_OVER_H
