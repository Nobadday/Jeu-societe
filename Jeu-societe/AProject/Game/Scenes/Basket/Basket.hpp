#pragma once

#include "../../Common.hpp"
#include "../../Scenes/Scene.hpp"
#include "../../Utilities/StringFormat.hpp"

class Basket : public SceneBase
{
private:
	class BasketPlayer
	{
	public:
		BasketPlayer(short _id, Basket* _basketScene);
		~BasketPlayer();

		void Update(float _dt);
		void Draw(sf::RenderWindow& _renderWindow);
		
		short GetID() const;
		void SetWon(bool _won);
		bool GetWon() const;
		float GetWinTime() const;

	private:
		short id;
		float force;
		bool won;
		float winTime;  // Temps auquel le joueur a gagné
		
		sf::Sprite ballSprite;
		sf::Sprite hoopSprite;
		sf::VertexArray aimLine;
		
		Basket* basketScene;
	};

	struct SceneData
	{
		sf::Font font;
		sf::Text timerText;
		sf::Text winnerText;
		short winnerPlayers;
		float timerToReset;
		float elapsedTime;  // Temps total écoulé depuis le début
		
		sf::Texture ballTexture;
		sf::Texture hoopTexture;
		
		std::vector<BasketPlayer> players;
		
		// Ajouté pour gérer la communication avec GameData
		GameData* gameData = nullptr;
	};

	SceneData* m_data;

public:
	virtual void Load();
	virtual void Unload();
	virtual void PollEvent(sf::Event& _event);
	virtual void Update(float _deltaTime);
	virtual void Draw(sf::RenderWindow& _renderWindow);

private:
	void ResetBasketGame();
	float JoystickToTargetPercentage(float _joystickY, float _ballY, float _axisAbsMax);
	bool NearlyEqual(float _a, float _b, float _tolerance);
	const char* GetWinnerID();
};

