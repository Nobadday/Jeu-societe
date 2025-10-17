#pragma once
#ifndef _INC_RUSSIAN_ROULETTE_HPP
#define _INC_RUSSIAN_ROULETTE_HPP

#include "../Scene.hpp"
#include "../../Common.hpp"
#include "../../Animation/Graphics/SpriteAnimated.hpp"
#include "../../Animation/Graphics/TextureAnimated.hpp"

class RussianRoulette : public SceneBase
{
	private:
		enum GameState
		{
			//Wait for input
			WAITING,
			//Animation 
			SPINNING,
			END
		};
		struct SceneData
		{
			sf::Texture backgroundTex;
			sf::Sprite backgroundSpr;

			//sf::Texture gunTex;
			//sf::Sprite gunSpr;

			TextureAnimated gunTexAnim;
			SpriteAnimated gunSprAnim;

			GameState gameState = WAITING;
			int bullet = -1;
			bool killed = false;

			//DEBUG
			int currentPlayer = 0;
			int nbOfPlayers = 4;
			int playerKilled = -1;
			sf::Font font;
			sf::Text text;

		};
		SceneData* m_data;

	public:
		virtual void Load(void);
		virtual void Unload(void);


		virtual void PollEvent(sf::Event& _event);
		virtual void Update(float _deltaTime);
		virtual void Draw(sf::RenderWindow& _renderWindow);
};

#endif // _INC_RUSSIAN_ROULETTE_HPP