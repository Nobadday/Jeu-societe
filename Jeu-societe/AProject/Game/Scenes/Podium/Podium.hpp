#pragma once
#ifndef _INC_PODIUM_HPP
#define _INC_PODIUM_HPP

#include "../../../Common.hpp"
#include "../../../Scenes/Scene.hpp"
#include "../../../Ui/MenuSystem.hpp"
#include "../../../Animation/Animation.hpp"

 class Podium : public SceneBase
{
	private:

		enum State
		{
			NONE = -1,

			PODIUM,
			PLAYERS,
			CONGRATS,
			DONE
		};

		struct Confetti
		{
			sf::Vector2f velocity;
			sf::Vector2f position;
			sf::Color color;


			float rotation;
			float currentLife;
			float lifeTime;

			sf::Vector2f scale;
		};

		struct SceneData
		{
			GameData* gameData;

			std::vector<SpriteAtlas> playerSpriteArray;
			std::vector<SpriteAtlas> podiumsSpriteArray;

			std::vector<TextPlus> playerTextArray;

			sf::Sprite background;
			sf::Sprite congrat;

			anim::Animator animatorArray[4];

			State state = NONE;

			sf::RectangleShape confetti;
			std::vector<Confetti> confettiVector;

			TextPlus continueText;
			sf::Sprite Button;

			AudioEngine* audio;
			bool patate = false;
		};
		SceneData* m_data = NULL;

		void Apply(Confetti& _confetti, sf::RectangleShape& _rectangle);

	public:
		virtual void Load(void);
		virtual void Unload(void);

		virtual void PollEvent(sf::Event& _event);
		virtual void Update(float _deltaTime);
		virtual void Draw(sf::RenderWindow& _renderWindow);

		int GetPlayerClassement(int _i);
};

#endif // !_INC_PODIUM_HPP
