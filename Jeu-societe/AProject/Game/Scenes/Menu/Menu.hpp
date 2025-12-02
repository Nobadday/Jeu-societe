#pragma once
#ifndef _INC_MENU_HPP
#define _INC_MENU_HPP

#include "../../../Scenes/Scene.hpp"
#include "../../../Common.hpp"
#include "../../../Animation/Graphics.hpp"
#include "../../../Ui/MenuSystem.hpp"

class Menu : public SceneBase
{
private:
	enum MenuState
	{
		MAIN_MENU,
		OPTIONS,
		CREDITS,
		PLAYER_NB_SELECTION,
		PLAYER_SELECTION,
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
		sf::Sprite logoGame;
		sf::Sprite logoCrea;
		SpriteAnimated iconsChara;
		std::vector<std::string> charaAvaible;
		TextPlus playerCount;
	};
	struct SceneData
	{
		UI ui;

		MenuState state = MAIN_MENU;
		ControlerCurrentButton controlerBtn = PLAY;
		float inputDelay = 0.f;
		int currentPlayer = 0;
		std::vector<int> currentCharaSelected;
		std::vector<bool> charaSelected;

		std::vector<PlayerData> playerDataVec;

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
	void ChangeSelection(int _value, int _joystick);
	void PressSelection(int _id);
	void PrintIcons(sf::RenderWindow& _renderWindow);

public:
	virtual void Load(void);
	virtual void Unload(void);


	virtual void PollEvent(sf::Event& _event);
	virtual void Update(float _deltaTime);
	virtual void Draw(sf::RenderWindow& _renderWindow);
};

#endif // _INC_MENU_HPP