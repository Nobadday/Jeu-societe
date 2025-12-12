#ifndef _INC_WARMUP_HPP
#define _INC_WARMUP_HPP

#include "../../../Scenes/Scene.hpp"
#include "../../../Common.hpp"
#include "../../../Video+Audio/HighResVideoPlayer.hpp"
#include "../../../Animation/Graphics.hpp"


class Warmup : public SceneBase
{
private:
	enum State
	{
		TRANS_1,
		VIDEO,
		TRANS_2
	};
	struct SceneData
	{
		TextPlus text;
		SpriteAnimated iconsChara;
		sf::Sprite background;
		State state = TRANS_1;
		sf::Vector2f scaleVid = { 1.f,1.f };
		bool playersReady = false;
		std::vector<bool> playersReadyVec;

		sf::Shader chromaKeyShader;
		HighResVideoPlayer videoPlayer;

		GameData* gameData;
		//* of audio because i'm tired of cast void*
		AudioEngine* audio;
	};
	SceneData* m_data;

	void PrintIcons(sf::RenderWindow& _renderWindow);

public:
	virtual void Load(void);
	virtual void Unload(void);


	virtual void PollEvent(sf::Event& _event);
	virtual void Update(float _deltaTime);
	virtual void Draw(sf::RenderWindow& _renderWindow);
};

#endif //!_INC_WARMUP_HPP