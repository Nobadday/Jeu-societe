#ifndef FLAGGAME_H
#define FLAGGAME_H

#include "../../../Common.hpp"
#include "../../../Scenes/Scene.hpp"
#include "../../../Utilities/StringFormat.hpp"
#include "../../../Animation/Animation/Timer.hpp" 

#define MAX_ROUND 3

class FlagGame : public SceneBase
{
private:
	enum State
	{
		STATE_WAITING,
		STATE_PLAYING,
		STATE_ROUND_END,
		STATE_GAME_OVER
	};

	struct PlayerData
	{
		bool isEliminated;
		GamePadBindList currentInput;
		sf::Text inputText;
		int eliminationOrder; // Order of elimination (0 = not eliminated, 1 = first eliminated, etc.)
		float eliminationTime; // Time when player was eliminated
		sf::Sprite buttonSprite;
	};

	struct SceneData
	{
		sf::Text titleText;
		sf::Text roundText;
		sf::Text timerText;
		sf::Text notEnoughPlayersText;
		sf::Text requiredInputText;
		sf::Text resultText;

		sf::Sprite buttonSprite;
		sf::Sprite backGround;
		
		State state;
		GameData* gameData;
		
		PlayerData playerData[4];
		int currentRound;
		int playersRemaining;
		int eliminationCounter; // Counter for tracking elimination order
		float totalGameTime; // Total time elapsed in game
		
		Timer roundTimer;
		Timer inputChangeTimer;
		
		GamePadBindList requiredInput;
		sf::String stringTab[6];
	};
	
	SceneData* m_data;

public:
	virtual void Load(void);
	virtual void Unload(void);
	virtual void PollEvent(sf::Event& _event);
	virtual void Update(float _deltaTime);
	virtual void Draw(sf::RenderWindow& _renderWindow);

private:
	void StartNewRound(void);
	void EvaluateRound(void);
	void ChangeRequiredInput(void);
	GamePadBindList GetRandomValidInput(void);
	bool IsInputValid(GamePadBindList _input);
	void UpdatePlayerInputTexts(void);
	bool HasEnoughPlayers(void);
	int GetFirstEliminatedPlayer(void); // Returns the player ID of the first eliminated player
};

#endif // !FLAGGAME_H
