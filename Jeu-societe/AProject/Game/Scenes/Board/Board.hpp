#ifndef _INC_BOARD_HPP
#define _INC_BOARD_HPP

#include "../../Common.hpp"

#include "../../Map/Tiled.h"

#include "../../Utilities/Camera/Camera.hpp"

#include "../../Animation/Animation/Animator.hpp"

#include "../../Animation/Graphics/SpriteAnimated.hpp"

#include "../../Animation/Graphics/AnimatedObject.hpp"

#include "../../Animation/Graphics/TextureAnimated.hpp"

#include "../../Animation/Graphics/TextureAtlas.hpp"

#define TIME_WIN_DISPLAY 1.0f
#define MAX_TOUR_EFFECT 2

class Effect
{
private:
	sf::Sprite m_sprite;
	sf::Vector2f m_position;
	float m_angle;
	float m_duration;
	float m_elapsedTime;

	bool m_active;
public:
	Effect(const sf::Texture& _texture, const sf::Vector2f& _position, float _duration ,float _angle)
		: m_angle(_angle), m_position(_position), m_duration(_duration), m_elapsedTime(_duration), m_active(true)
	{
		m_sprite.setTexture(_texture);
		m_sprite.setPosition(m_position);
		m_sprite.setOrigin(m_sprite.getLocalBounds().width / 2, m_sprite.getLocalBounds().height / 2);
	}
	void Update(float _deltaTime)
	{
		if (!m_active)
			return;
		m_elapsedTime -= _deltaTime;
		if (m_elapsedTime <= 0 )
		{
			m_active = false;
		}
	}
	void Draw(sf::RenderWindow& _renderWindow)
	{
		if (m_active)
		{
			float factor = pow(m_elapsedTime / m_duration, 2);
			m_sprite.setRotation(m_angle);
			m_sprite.setColor(sf::Color(255, 255, 255, 255 * factor));
			_renderWindow.draw(m_sprite);
		}
	}
	bool IsActive() const { return m_active; }

	void SetIsActive(bool _m_active) { m_active = _m_active; };

};

class BaseGame : public SceneBase
{
	private:
		enum State
		{
			START,
			PLAY,
			DEPLACEMENT,
			DEPLACEMENT_SPLIT,
			DEPLACEMENT_BRIGE,
			DEPLACEMENT_ACTION,
			DEPLACEMENT_ACTION_BACK,
			CASE_ACTION,
			BATTLE_ACTION,
			DUEL,
			WIN,
			WIN_DEPLACEMENT,
			CASE_ACTION_END,
			WAITING_BRIDGE_ROLL,
			WAITING_PATH_CHOICE
		};

		enum StatePlayer
		{
			NONE = -1,
			INFEC,
			IMMUN,
			CANT_PLAY,
			CONFUSED
		};

		struct Player
		{
			SpriteAnimated sprite;
			
			TextureAnimated texture;
			
			sf::Vector2f boardPosition;
			
			int currentCaseIndex;

			int startRandom;

			StatePlayer state;

			int tourstate;

			int pendingMovement;  // Nouveau : mouvement restant
			int currentPathId;    // Nouveau : ID du chemin actuel (-1 = chemin principal)
			bool waitingBridgeRoll;  // Nouveau : en attente du lancer pour le pont
		};

		struct SceneData
		{
			Tiled tile;

			std::vector<MapObject> posCase;

			Camera camera;

			std::vector<Player> players;

			anim::Animator animator;

			anim::Animator animator2;

			State state;

			float timeWin;

			int currentPlayerIndex;

			std::vector<int> pathChoices;

			sf::Texture smokeTp;

			std::vector<Effect> effects;
		};

		GameData* m_gameData;

		SceneData* m_data;

	private:
		void CaseAction();

		void SetBoardState(State _state, int _newIndex = 0);

		void SetWinDeplacement(int _newIndex);

		void BoardStateUpdate(float _dt);

		int OnSameCase();

		std::string RandomDuel();

		void SortStart();

		std::string RandomBattle();

		// Fonctions de gestion de la caméra
		void UpdateCameraToShowAllPlayers();
		
		void UpdateCameraFollowPlayer(float _deltaTime);

		void BonusMalusLuck(bool _malus);

		void Bonus(int _chance);
		
		void Malus(int _chance);

		bool HasPathChoice(int caseIndex);

		std::vector<int> GetAvailablePaths(int caseIndex);

		std::vector<int> GetAvailablePathsBack(int caseIndex);

		int GetNextCaseIndex(int currentIndex, int pathChoice);

		void ProcessPathChoice(int choiceIndex);

		void ProcessBridgeRoll();

		void SwapPlayers();

		void CreateSmokeEffect(Player& _player);
	public:
		virtual void Load(void);
		
		virtual void Unload(void);
		
		virtual void PollEvent(sf::Event& _event);
		
		virtual void Update(float _deltaTime);

		virtual void Draw(sf::RenderWindow& _renderWindow);
};


#endif // _INC_BOARD_HPP

