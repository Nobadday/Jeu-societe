#pragma once

#include "../../Common.hpp"
#include "../../Scenes/Scene.hpp"
#include "../../Utilities/StringFormat.hpp"

class BasketPlayer
{
	public:
	BasketPlayer(short _id);
	~BasketPlayer();
	static void UpdateAllPlayers(float _dt);
	static void DrawAllPlayers(sf::RenderWindow& _renderWindow);
	short GetID(void) const;
	sf::Sprite* ballSprite;
	sf::Sprite* hoopSprite;
	sf::VertexArray* aimLine;
private:
	short id;
	float force;
	void Update(float _dt);
	void Draw(sf::RenderWindow& _renderWindow);
};

class Basket : public SceneBase
{
public:

	struct SceneData
	{
		sf::Text* text;
		BasketPlayer* player1;
		BasketPlayer* player2;
	};

	SceneData* m_data;

	virtual void Load(void);
	virtual void Unload(void);
	virtual void PollEvent(sf::Event& _event);
	virtual void Update(float _deltaTime);
	virtual void Draw(sf::RenderWindow& _renderWindow);
};

