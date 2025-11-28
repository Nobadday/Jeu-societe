#pragma once
#ifndef _INC_PODIUM_HPP
#define _INC_PODIUM_HPP

#include "../../../Common.hpp"
#include "../../../Scenes/Scene.hpp"
#include "../../../Ui/MenuSystem.hpp"



 class Podium : public SceneBase
{
	private:
		struct SceneData
		{
			GameData* gameData;

			std::vector<SpriteAtlas> playerSpriteArray;
			std::vector<TextPlus> playerTextArray;

			std::vector<SpriteAtlas> podiumsSpriteArray;


			sf::Sprite background;
		};
		SceneData* m_data;

	public:
		virtual void Load(void);
		virtual void Unload(void);

		virtual void PollEvent(sf::Event& _event);
		virtual void Update(float _deltaTime);
		virtual void Draw(sf::RenderWindow& _renderWindow);
};

#endif // !_INC_PODIUM_HPP
