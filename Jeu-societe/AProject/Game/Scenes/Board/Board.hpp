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

class BaseGame : public SceneBase
{
	private:
		enum State
		{
			START,
			PLAY,
			DEPLACEMENT,
			DEPLACEMENT_ACTION,
			CASE_ACTION,
			BATTLE_ACTION,
			DUEL,
			WIN,
			WIN_DEPLACEMENT,
			CASE_ACTION_END,
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

			std::vector<int> pathChoices;  // Nouveau : choix de chemins disponibles
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

		int GetNextCaseIndex(int currentIndex, int pathChoice);

		void ProcessPathChoice(int choiceIndex);

	public:
		virtual void Load(void);
		
		virtual void Unload(void);
		
		virtual void PollEvent(sf::Event& _event);
		
		virtual void Update(float _deltaTime);

		virtual void Draw(sf::RenderWindow& _renderWindow);
};

#endif // _INC_BOARD_HPP

