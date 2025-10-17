#pragma once
#ifndef FLAGMINIGAME_HPP
#define FLAGMINIGAME_HPP

#include "../../Common.hpp"

class FlagMiniGame : public SceneBase
{
	private:
		struct SceneData
		{

		};
		SceneData* m_data;

	public:
		virtual void Load(void);

		virtual void Unload(void);

		virtual void PollEvent(sf::Event& _event);
		virtual void Update(float _deltaTime);
		virtual void Draw(sf::RenderWindow& _renderWindow);

};


#endif // !FLAGMINIGAME_HPP
