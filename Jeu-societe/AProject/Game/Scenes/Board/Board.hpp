#ifndef _INC_BOARD_HPP
#define _INC_BOARD_HPP

#include "../../Common.hpp"

#include "../../Map/Tiled.h"
#include "../../Video+Audio/HighResVideoPlayer.hpp"

#include "../../Utilities/Camera/Camera.hpp"

#include "../../Animation/Animation/Animator.hpp"

#include "../../Animation/Graphics/SpriteAnimated.hpp"0

#include "../../Animation/Graphics/AnimatedObject.hpp"

#include "../../Animation/Graphics/TextureAnimated.hpp"

#include "../../Animation/Graphics/TextureAtlas.hpp"

#define TIME_WIN_DISPLAY 1.0f
#define TIME_LBM_DISPLAY 6.0f
#define TIME_DIS_DISPLAY 0.001f
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

	void UpdateSpecial(float _deltaTime )
	{
		if (!m_active)
			return;
		m_elapsedTime -= _deltaTime;
		if (m_elapsedTime <= 0)
		{
			m_elapsedTime = m_duration;
		}
	}

	void Draw(sf::RenderWindow& _renderWindow)
	{
		if (m_active)
		{
			float factor = pow(m_elapsedTime / m_duration, 1.f);
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
			STATE = -1,
			START,
			PLAY,
			DICE_ANIMATION,  // NOUVEAU : État pour l'animation du dé
			DEPLACEMENT,
			DEPLACEMENT_BACK,
			DEPLACEMENT_SPLIT,
			DEPLACEMENT_BRIGE,
			DEPLACEMENT_ACTION,
			DEPLACEMENT_ACTION_2,
			DEPLACEMENT_ACTION_BACK,
			CASE_ACTION,
			BATTLE_ACTION,
			DUEL,
			WIN,
			WIN_DEPLACEMENT,
			CASE_ACTION_END,
			WAITING_BRIDGE_ROLL,
			WAITING_FIN_ROLL,
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

		enum LBM
		{
			NONELBM = -1,
			LUCKY,
			BONUS,
			MALUS
		};

		enum PosIcone
		{
			UP_LEFT,
			UP_RIGHT,
			DOWN_LEFT,
			DONW_RIGHT
		};

		struct Player
		{
			SpriteAnimated sprite;
			
			TextureAnimated texture;
			
			sf::Vector2f boardPosition;
			
			int currentCaseIndex;

			int startRandom;

			StatePlayer state;

			sf::Text playeur;
			sf::Text v;

			int tourstate;

			PosIcone posIcone;

			int pendingMovement;  // Nouveau : mouvement restant
			int currentPathId;    // Nouveau : ID du chemin actuel (-1 = chemin principal)
			bool waitingBridgeRoll;  // Nouveau : en attente du lancer pour le pont
		};

		struct LuckBonusMalus
		{
			SpriteAnimated sprite;

			sf::Text text;

			std::string name;
			
			std::string chosse;

			LBM state;

			int rando;

			int swap;

			bool active;
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

			bool smokeOff;

			sf::Texture smoke;

			std::vector<Effect> effectSwap;
			std::vector<Effect> effectsMap;

			float timeLBM;

			LuckBonusMalus HudLBM;

			SpriteAnimated icone;

			bool active;
			
			// NOUVEAU : Variables pour gérer l'animation du dé
			int diceResult;           // Résultat du lancer de dé (1-6)
			bool diceAnimationPlaying;  // Indique si la vidéo est en cours
			sf::Vector2f dicePosition;  // Position d'affichage du dé

			float timeDice;

			// NOUVEAU : Shader pour le chroma key
			sf::Shader chromaKeyShader;
			std::vector<HighResVideoPlayer*> diceVideos;
			// MODIFICATION : Stocker des pointeurs vers les vidéos
			
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

		void ProcessFinRoll();

		void SwapPlayers(int _swap);

		void CaseAvancePlus(int _rando);

		void ImuniteMalus();

		void CaseMoins(int _rando);

		void Infecte();

		void PaseTour();

		void CreateSmokeEffectForSwap(Player& _player);

		void CreateSmokeEffectAnotherPart(sf::Vector2f _posMin, sf::Vector2f _posMax);

		void UpdateLBM(float _dt);

		void LBMDisplayUpdate(float _dt);

		void DrawLBM(sf::RenderWindow& _renderWindow);

		void DrawIconePlayer(sf::RenderWindow& _renderWindow, int _i);

	public:
		void LoadAsync(std::atomic<float>& progress);

		virtual void Load(void);
		
		virtual void Unload(void);
		
		virtual void PollEvent(sf::Event& _event);
		
		virtual void Update(float _deltaTime);

		virtual void Draw(sf::RenderWindow& _renderWindow);
};


#endif // _INC_BOARD_HPP

