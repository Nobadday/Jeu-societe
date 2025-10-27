#ifndef _INC_BOARD_HPP
#define _INC_BOARD_HPP

#include "../../Scenes/Scene.hpp"
#include "../../Map/Tiled.h"
#include "../../Utilities/Camera/Camera.hpp"
#include "../../Utilities/MathPlus.hpp"
#include "../../Animation/Animation/Animator.hpp"

class BaseGame : public SceneBase
{
	private:
		struct Player
		{
			sf::Sprite sprite;
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
		};
		SceneData* m_data;

	public:
		virtual void Load(void);
		
		virtual void Unload(void);
		
		virtual void PollEvent(sf::Event& _event);
		
		virtual void Update(float _deltaTime);

		virtual void Draw(sf::RenderWindow& _renderWindow);
};



#endif // _INC_PLATO_HPP
