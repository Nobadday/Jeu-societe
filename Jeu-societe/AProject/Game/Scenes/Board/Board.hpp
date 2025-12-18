#ifndef _INC_BOARD_HPP
#define _INC_BOARD_HPP

#include "../../../Common.hpp"

#include "../../../Map/Tiled.h"

#include "../../../Video+Audio/HighResVideoPlayer.hpp"

#include "../../../Utilities/Camera/Camera.hpp"

#include "../../../Animation/Animation/Animator.hpp"

#include "../../../Animation/Graphics/SpriteAnimated.hpp"

#include "../../../Animation/Graphics/SpriteAtlas.hpp"

#include "../../../Animation/Graphics/AnimatedObject.hpp"

#include "../../../Animation/Graphics/TextureAnimated.hpp"

#include "../../../Animation/Graphics/TextureAtlas.hpp"

#include "../../../Animation/Graphics/TextPlus.hpp"

#define TIME_WIN_DISPLAY 0.5f
#define TIME_LBM_DISPLAY 6.0f
#define TIME_END_DISPLAY 2.0f
#define TIME_DIS_DISPLAY 0.2f
#define TIME_START_DISPLAY 1.2f
#define MAX_TOUR_EFFECT 2
#define TRANSITION_1 6 
#define TRANSITION_2 7 

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
			m_sprite.setColor(sf::Color(255, 255, 255, (sf::Uint8)(255 * factor)));
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
			INTRO,
			START,
			PLAY,
			DICE_ANIMATION,
			DICE_ANIMATION_BRIDGE,  // NOUVEAU
			DICE_ANIMATION_END,      // NOUVEAU
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
			WAITING_PATH_CHOICE,
			END
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

			TextPlus playeur;
			TextPlus v;
			TextPlus diceNumber; // NOUVEAU : Texte pour afficher le numéro du dé

			int tourstate;

			PosIcone posIcone;

			int pendingMovement;  // Nouveau : mouvement restant
			int currentPathId;    // Nouveau : ID du chemin actuel (-1 = chemin principal)
			bool waitingBridgeRoll;  // Nouveau : en attente du lancer pour le pont
			bool firstTime = false;
		};

		struct LuckBonusMalus
		{
			SpriteAnimated sprite;

			TextPlus text;

			std::string name;
			
			std::string chosse;

			LBM state;

			int rando;

			int swap;

			bool active;
		};

		struct TexteDisplay
		{
			std::vector<TextPlus*> texte; // CHANGÉ : vecteur de pointeurs
			float displayTime;
			float currentTime;
			bool isActive;
			
			// NOUVEAU : Gestion du fade
			enum FadeState
			{
				FADE_NONE,
				FADE_IN,
				FADE_DISPLAY,
				FADE_OUT				
			};
			
			FadeState fadeState;
			float fadeInDuration;
			float fadeOutDuration;
			float fadeTimer;
		};

		struct SceneData
		{
			float timeStart;

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
			SpriteAtlas iconeState;
			SpriteAnimated iconeAura;

			bool active;
			
			// NOUVEAU : Variables pour gérer l'animation du dé
			int diceResult;           // Résultat du lancer de dé (1-6)
			bool diceAnimationPlaying;  // Indique si la vidéo est en cours
			sf::Vector2f dicePosition;  // Position d'affichage du dé

			float timeDice;

			// NOUVEAU : Shader pour le chroma key
			sf::Shader chromaKeyShader;
			std::vector<HighResVideoPlayer*> diceVideos;
			HighResVideoPlayer* currentDiceVideo;
			// MODIFICATION : Stocker des pointeurs vers les vidéos

			sf::Sprite arrow;

			sf::Vector2f posArrow[2];

			TexteDisplay texteDisplay;

			float timerEnd;

			bool endActive;
			
		};

		GameData* m_gameData;

		SceneData* m_data;

		AudioEngine* m_audioEngine;

	private:
		
		void SortDrawOrder();

		void CaseAction();

		void SetBoardState(State _state, int _newIndex = 0);

		void SetWinDeplacement(int _newIndex);

		void BoardStateUpdate(float _dt);

		int OnSameCase();

		std::string RandomDuel();

		void SortStart(float _dt);

		std::string RandomBattle();

		// NOUVEAU : Méthodes helper pour simplifier le code
		bool CheckPlayerInput(sf::Event& _event, bool& shouldRoll);

		void ProcessDiceRoll(int rando);

		void HandleMovementState(State state, float _dt);

		void InitiateMovement(int nextIndex, bool backwards = false);

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

		void ShowTextDisplay(const std::string& message, float duration);

	public:
		void LoadAsync(std::atomic<float>& progress);

		virtual void Load(void);
		
		virtual void Unload(void);

		virtual void PollEvent(sf::Event& _event);
		
		virtual void Update(float _deltaTime);

		virtual void Draw(sf::RenderWindow& _renderWindow);
};


#endif // _INC_BOARD_HPP

