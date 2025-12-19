#ifndef FLAGGAME_H
#define FLAGGAME_H

#include "../../../Common.hpp"
#include "../../../Scenes/Scene.hpp"
#include "../../Ui/Transition/Transition.hpp"
#include "../../../Animation/Animation/Timer.hpp" 
#include "../../../Animation/Graphics/SpriteAtlas.hpp" 
#include "../../../Animation/Graphics/TextureAtlas.hpp" 
#include "../../../Animation/Graphics/SpriteAnimated.hpp" 
#include "../../../Animation/Graphics/TextureAnimated.hpp" 

#define MAX_ROUND 3

class FlagGame : public SceneBase
{
private:
	enum State
	{
		TRANSITION,
		STATE_WAITING,
		STATE_PLAYING,
		STATE_ROUND_END,
		STATE_GAME_OVER
	};

	struct Player
	{
		bool isEliminated;
		GamePadBindList currentInput;
		sf::Text inputText;
		int eliminationOrder;
		float eliminationTime;
		SpriteAtlas buttonSprite;
		SpriteAnimated playerIcone;
	};

	struct SceneData
	{
		sf::Text roundText;
		sf::Text timerText;
		sf::Text notEnoughPlayersText;
		sf::Text requiredInputText;
		sf::Text resultText;

		SpriteAtlas buttonSprite;
		
		// Ajout du sprite pour le background
		sf::Sprite backgroundSprite;
		
		State state;
		GameData* gameData;
		
		Player playerData[4];
		int currentRound;
		int playersRemaining;
		int eliminationCounter;
		float totalGameTime;
		
		Timer roundTimer;
		Timer inputChangeTimer;
		
		GamePadBindList requiredInput;
		sf::String stringTab[6];

		float currentInputChangeMin;
		float currentInputChangeMax;

		TransitionClass transition;
		bool end = false;
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
	int GetFirstEliminatedPlayer(void);
	void InitializePlayerIcon(int playerID, int positionIndex);
};

#endif // !FLAGGAME_H
