#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "Common.hpp"
#include "Menu.hpp"
#include "Game.hpp"
#include "GameOver.hpp"

typedef enum GameState
{
	MENU,
	GAME,
	GAME_OVER
}GameState;

void CleanupGameState(void);
void SetGameState(GameState _gameState);
void SetEnumGameState(GameState _gameState);
GameState GetGameState(void);

#endif
