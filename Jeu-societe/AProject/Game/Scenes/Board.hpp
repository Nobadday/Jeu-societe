#ifndef _INC_BOARD_HPP
#define _INC_BOARD_HPP

#include "../../Scenes/Scene.hpp"
#include "../../Map/Tiled.h"
#include "../../Utilities/Camera/Camera.hpp"
#include "../../Utilities/MathPlus.hpp"
#include "../../Utilities/Random.hpp"
#include "../../Animation/Animation/Animator.hpp"

class BaseGame : public SceneBase
{
	private:
		enum State
		{
			START,
			DEPLACEMENT,
			DEPLACEMENT_ACTION,
			CASE_ACTION,
			BATTLE_ACTION,
			WIN,
			LOST,
			CASE_ACTION_END,
		};

		struct Player
		{
			sf::Sprite sprite;
			
			sf::Texture texture;
			
			sf::Vector2f boardPosition;
			
			int currentCaseIndex;
		};

		struct SceneData
		{
			Tiled tile;

			std::vector<MapObject> posCase;

			Camera camera;

			std::vector<Player>  players;

			anim::Animator animator;

			State state;

			int currentPlayerIndex;

		};

		SceneData* m_data;

		void CaseAction();

		void SetBoardState(State _state, int _newIndex = 0);
		
		void BoardStateUpdate();

	public:
		virtual void Load(void);
		
		virtual void Unload(void);
		
		virtual void PollEvent(sf::Event& _event);
		
		virtual void Update(float _deltaTime);

		virtual void Draw(sf::RenderWindow& _renderWindow);
};

#endif // _INC_BOARD_HPP