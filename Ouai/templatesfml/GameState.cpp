#include "GameState.hpp"

GameState gameState = MENU;

void LoadGameState(void);

Menu* menu = NULL;
Game* game = NULL;
GameOver* gameOver = NULL;

void SetGameState(GameState _gameState)
{
	CleanupGameState();
	SetEnumGameState(_gameState);
	LoadGameState();
}

void SetEnumGameState(GameState _gameState)
{
	gameState = _gameState;
}

GameState GetGameState(void)
{
	return gameState;
}

void LoadGameState()
{
	switch (gameState)
	{
	case MENU:
		menu = new Menu();
		break;
	case GAME:
		game = new Game();
		break;
	case GAME_OVER:
		gameOver = new GameOver();
		break;
	}
}

void CleanupGameState()
{
	switch (gameState)
	{
	case MENU:
		delete menu;
		menu = NULL;
		break;
	case GAME:
		delete game;
		game = NULL;
		break;
	case GAME_OVER:
		delete gameOver;
		gameOver = NULL;
		break;
	}
}
