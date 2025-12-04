#pragma once
#ifndef _INC_MENU_HPP
#define _INC_MENU_HPP

#include "../../../Scenes/Scene.hpp"
#include "../../../Common.hpp"
#include "../../../Animation/Graphics.hpp"
#include "../../../Ui/Button.hpp"

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
	enum ControlerCurrentButton
	{
		PLAY,
		SETTINGS,
		LEAVE,
		LESS,
		//2 play here is bad, but idk how to change easely
		PLAY_SELECTION,
		MORE,
	};
	struct GameSettings
	{
		int playerCount = 1;

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
		TextPlus playerCount;
	};
	struct SceneData
	{
		UI ui;
		ControlerCurrentButton controlerBtn = PLAY;
		float inputDelay = 0.f;


		MenuState state;
		//MenuSystem* menuSystem;
		GameSettings gameSettings;
		GameData* gameData;
		//* of audio because i'm tired of cast void*
		AudioEngine* audio;

	};
	SceneData* m_data;
	void LoadUI(void);
	void ButtonsUpdate(float _dt);
	void ButtonsPollEvent(sf::Event& _event);
	void DrawUI(sf::RenderWindow& _renderWindow);
	void ChangeSelection(int _value);
	void PressSelection(void);
public:
	virtual void Load(void);
	virtual void Unload(void);


	virtual void PollEvent(sf::Event& _event);
	virtual void Update(float _deltaTime);
	virtual void Draw(sf::RenderWindow& _renderWindow);
};

#endif // _INC_MENU_HPP