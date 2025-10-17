#ifndef GAME_H
#define GAME_H

#include "common.h"


class Scene1
{
	public:
		Scene1(sf::RenderWindow& _window);
		~Scene1();
		Scene PollEvent(sf::Event& _event);
		void Update(float _dt);
		void Draw(sf::RenderWindow& _window);
	private:

		sf::Texture texture;
		sf::Sprite sprite;
		sf::RenderWindow& _window;


};

#endif