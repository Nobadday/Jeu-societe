#include "Scene1.hpp"

Scene1::Scene1(sf::RenderWindow& _window)
	: _window(_window)
{


	
	texture.loadFromFile("monstre.png");
	sprite.setTexture(texture);

}

Scene1::~Scene1()
{


}

Scene Scene1::PollEvent(sf::Event& _event)
{


	return SCENE_1;
}

void Scene1::Update(float _dt)
{

}

void Scene1::Draw(sf::RenderWindow& window)
{
	window.draw(sprite);

}