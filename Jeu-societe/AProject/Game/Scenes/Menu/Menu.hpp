#pragma once
#ifndef _INC_MENU_HPP
#define _INC_MENU_HPP

#include "../../../Scenes/Scene.hpp"
#include "../../../Common.hpp"
#include "../../../Animation/Graphics.hpp"
#include "../../../Ui/Button.hpp"
#include "../../Video+Audio/HighResVideoPlayer.hpp"

#include "../../Bot/BotAI.hpp"

class Menu : public SceneBase
{
private:
	enum MenuState
	{
		INTRO,
		MAIN_MENU,
		OPTIONS,
		CREDITS,
		PLAYER_NB_SELECTION,
		BOT_SELECTION,
		PLAYER_SELECTION,
	};
	enum ControlerCurrentButton
	{
		PLAY,
		SETTINGS,
		LEAVE,
		CREDITS_BTN,
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
		TextPlus playerText;
		Button botToggleBtn;  // Bouton pour activer/désactiver les bots
		TextPlus botCountText; // Texte affichant le nombre de bots
		int botCount;          // Nombre de bots (0-2)
	};
	struct SceneData
	{
		UI ui;

		MenuState state = INTRO;
		ControlerCurrentButton controlerBtn = PLAY;
		float inputDelay = 0.f;
		int currentPlayer = 0;
		std::vector<int> currentCharaSelected;
		std::vector<bool> charaSelected;

		std::vector<PlayerData> playerDataVec;

		HighResVideoPlayer introVideo;
		GameSettings gameSettings;
		GameData* gameData;
		//* of audio because i'm tired of cast void*
		AudioEngine* audio;
	};
	SceneData* m_data;
	void LoadUI(void);
	void LoadButtons();
	void LoadSprites();
	void LoadCharacterIcons();
	void LoadText();
	void PositionMainMenuButtons();
	void ButtonsUpdate(float _dt);
	void ButtonsPollEvent(sf::Event& _event);
	void DrawUI(sfMod::RenderWindow* _renderWindow);

	void ChangeSelection(int _value, int _joystick);
	void HandleMainMenuSelection(int _value);
	void PrintBotSelection(sfMod::RenderWindow* _renderWindow);
	void HandleOptionsSelection(int _value);
	void HandlePlayerSelection(int _value, int _joystick);
	void PressSelection(int _id);

	void PrintIcons(sfMod::RenderWindow* _renderWindow);
	void PrintOptions(sfMod::RenderWindow* _renderWindow);

	sf::Vector2f GetButtonPosition(ControlerCurrentButton _button) const;

	int CalculateMaxBots(int totalPlayers);

	void ReturnPressed(void);

	PlayerData::PlayerSkin GetPlayerSkinFromIndex(int _index) const;

public:
	virtual void Load(void);
	virtual void Unload(void);


	virtual void PollEvent(sf::Event& _event);
	virtual void Update(float _deltaTime);
	virtual void Draw(sf::RenderWindow& _renderWindow);
};

#endif // _INC_MENU_HPP