#ifndef MENU_H
#define MENU_H



#include "common.h"

class Scene2
{
	public:
		Scene2();
		~Scene2();
		Scene PollEvent(sf::Event& _event);
		void Update(float _dt);
		void Draw(sf::RenderWindow& _window);
	private:
};



#endif