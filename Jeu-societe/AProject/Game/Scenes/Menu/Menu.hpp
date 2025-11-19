#pragma once
#ifndef _INC_MENU_HPP
#define _INC_MENU_HPP

#include "../../../Scenes/Scene.hpp"
#include "../../../Common.hpp"
#include "../../../Animation/Graphics/SpriteAnimated.hpp"
#include "../../../Animation/Graphics/TextureAnimated.hpp"
#include "../../../Ui/MenuSystem.hpp"
#include "../../../Utilities/AssetManager.hpp"



class Menu : public SceneBase
{
private:
	enum MenuState
	{
		MAIN_MENU,
		OPTIONS,
		CREDITS,
		PLAYER_NB_SELECTION,
		PLAYER_SELECTION
	};
	struct GameSettings
	{
		int playerCount;

	};
	struct Settings
	{
		float musicVolume;
		float soundVolume;
		float frameRateLimit;
		bool vsync;
	};
	struct UI
	{
		std::map<std::string, Button> buttonMap;
		sf::Sprite background;

	};
	struct SceneData
	{
		UI ui;
		MenuState state;
		//MenuSystem* menuSystem;

		GameData* gameData;
	};
	SceneData* m_data;
	void LoadUI(void);
	void ButtonsUpdate(float _dt);
	void ButtonsPollEvent(sf::Event& _event);
	void ButtonsDraw(sf::RenderWindow& _renderWindow);

public:
	virtual void Load(void);
	virtual void Unload(void);


	virtual void PollEvent(sf::Event& _event);
	virtual void Update(float _deltaTime);
	virtual void Draw(sf::RenderWindow& _renderWindow);
};

#endif // _INC_MENU_HPP