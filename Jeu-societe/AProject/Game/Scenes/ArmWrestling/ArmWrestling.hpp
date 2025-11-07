#pragma once

#include "../../Common.hpp"
#include "../../Scenes/Scene.hpp"
#include "../../Utilities/StringFormat.hpp"

class ArmWrestlingPlayer
{
public:
	ArmWrestlingPlayer(sf::Vector2f _pos, sf::Color _color, float _scale, short _id);
	short GetID(void) const;
	short GetForce(void) const;
	void SetForce(short _force);

	void Update(float _dt, std::vector<ArmWrestlingPlayer>& allPlayers);
	void Draw(sf::RenderWindow& _renderWindow);

	// Ajouté : accès en lecture à la taille actuelle du bras
	float GetArmWidth() const;

private:
	short id;
	short force;
	sf::RectangleShape shape;
	sf::Text nameText;

	short GetOtherPlayerID(short _callerID, const std::vector<ArmWrestlingPlayer>& allPlayers) const;
};

class ArmWrestling : public SceneBase
{
public:

	struct SceneData
	{
		sf::Text titleText;
		sf::Text timeText;
		sf::RectangleShape fillBar;
		float timer = 0.0f;
		std::vector<ArmWrestlingPlayer> allPlayers;
		short nextID = 0;

		// Ajoutés pour gérer l'affichage du résultat et la communication avec GameData
		GameData* gameData = nullptr;
		sf::Text resultText;
		bool finished = false;
		float endDelay = 0.0f;
	};
	
	SceneData* m_data = nullptr;

	virtual void Load(void);
	virtual void Unload(void);
	virtual void PollEvent(sf::Event& _event);
	virtual void Update(float _deltaTime);
	virtual void Draw(sf::RenderWindow& _renderWindow);
private:

};