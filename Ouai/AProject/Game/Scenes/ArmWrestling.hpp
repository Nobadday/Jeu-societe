#pragma once

#include "../../Common.hpp"
#include "../../Scenes/Scene.hpp"
#include "../../Utilities/StringFormat.hpp"

class ArmWrestlingPlayer
{
public:
	ArmWrestlingPlayer(sf::Vector2f _pos, sf::Color _color, float _scale, short _id);
	~ArmWrestlingPlayer();
	static void UpdateAllPlayers(float _dt);
	static void DrawAllPlayers(sf::RenderWindow& _renderWindow);
	short GetID(void) const;
	short GetForce(void) const;
	void SetForce(short _force);
	static ArmWrestlingPlayer& GetPlayerByID(short _id);
private:
	short id;
	short force;
	short scale;
	sf::RectangleShape* shape;
	sf::Text* nameText;
	void Update(float _dt);
	void Draw(sf::RenderWindow& _renderWindow);
	static short GetOtherPlayerID(short _callerID);
};

class ArmWrestling : public SceneBase
{
	public:

		struct SceneData
		{
			sf::Text* titleText;
			sf::Text* timeText;
			sf::RectangleShape* fillBar;
			float timer;
			ArmWrestlingPlayer* player1;
			ArmWrestlingPlayer* player2;
		};
		
		SceneData* m_data;

		virtual void Load(void);
		virtual void Unload(void);
		virtual void PollEvent(sf::Event& _event);
		virtual void Update(float _deltaTime);
		virtual void Draw(sf::RenderWindow& _renderWindow);
private:

};