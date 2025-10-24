#ifndef _INC_BOARD_HPP
#define _INC_BOARD_HPP

#include "../../Scenes/Scene.hpp"
#include "../../Map/Tiled.h"

class BaseGame : public SceneBase
{
	private:
		struct SceneData
		{
			Tiled Tile;
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
