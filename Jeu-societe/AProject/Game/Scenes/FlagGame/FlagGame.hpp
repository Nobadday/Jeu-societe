#ifndef FLAGGAME_H
#define FLAGGAME_H

#include "../../Common.hpp"
#include "../../Scenes/Scene.hpp"
#include "../../Utilities/StringFormat.hpp"

#define MAX_ROUND 3

class FlagGamePlayer
{
public:
	FlagGamePlayer(short _id);
	~FlagGamePlayer();
	static void UpdateAllPlayers(float _dt);
	static void DrawAllPlayers(sf::RenderWindow& _renderWindow);
	short GetID(void) const;
	bool IsInGame(void) const;
	void SetInGame(bool _inGame);
	FlagGamePlayer& GetPlayerByID(short _id);
	GamePadBindList GetSavedInput(void) const;
private:
	short id;
	bool inGame;
	GamePadBindList savedInput;
	sf::Text* inputText;
	void Update(float _dt);
	void Draw(sf::RenderWindow& _renderWindow);
};

class FlagGame : public SceneBase
{
	public:
	struct SceneData
	{
		sf::Text* titleText;
		sf::Text* countRoundText;
		sf::Text* timerText;
		sf::Text* noEnoughtPlayer;
		sf::Text* InputText;
		short round;
		float timerBeforeNextRound;
		float randomDelayforChangeInput;
		short randomInputID;
	};
	SceneData* m_data;
	virtual void Load(void);
	virtual void Unload(void);
	virtual void PollEvent(sf::Event& _event);
	virtual void Update(float _deltaTime);
	virtual void Draw(sf::RenderWindow& _renderWindow);
private:
	void NextRound(void);
	void NextInput(void);
	void ResetFlagGame(void);
};

#endif // !FLAGGAME_H
