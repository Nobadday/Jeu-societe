#include "FlagGame.hpp"
#include "../../Utilities/Random.hpp"

#define ROUND_TIME 5.0f
#define ROUND_END_TIME 3.0f
#define INPUT_CHANGE_MIN 0.5f
#define INPUT_CHANGE_MAX 3.0f

void FlagGame::Load(void)
{
	m_data = new SceneData();
	m_data->gameData = (GameData*)this->m_keptData;
	m_data->state = STATE_WAITING;
	m_data->currentRound = 0;
	m_data->playersRemaining = 0;
	m_data->eliminationCounter = 0;
	m_data->totalGameTime = 0.0f;

	// Load font
	m_data->font.loadFromFile("Assets/Font.ttf");

	// Initialize title text
	m_data->titleText.setFont(m_data->font);
	m_data->titleText.setCharacterSize(24);
	m_data->titleText.setFillColor(sf::Color::White);
	m_data->titleText.setString("Flag Game Mini-Game");
	m_data->titleText.setPosition(SCREEN_WIDTH / 2, 20);
	m_data->titleText.setOrigin(m_data->titleText.getLocalBounds().width / 2, 0);

	// Initialize round text
	m_data->roundText.setFont(m_data->font);
	m_data->roundText.setCharacterSize(25);
	m_data->roundText.setFillColor(sf::Color::Blue);
	m_data->roundText.setPosition(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 4);

	// Initialize timer text
	m_data->timerText.setFont(m_data->font);
	m_data->timerText.setCharacterSize(25);
	m_data->timerText.setFillColor(sf::Color::White);
	m_data->timerText.setPosition(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 3);

	// Initialize required input text
	m_data->requiredInputText.setFont(m_data->font);
	m_data->requiredInputText.setCharacterSize(25);
	m_data->requiredInputText.setFillColor(sf::Color::Yellow);
	m_data->requiredInputText.setPosition(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2.5f);

	// Initialize not enough players text
	m_data->notEnoughPlayersText.setFont(m_data->font);
	m_data->notEnoughPlayersText.setCharacterSize(25);
	m_data->notEnoughPlayersText.setFillColor(sf::Color::Red);
	m_data->notEnoughPlayersText.setString("Not Enough Players Connected!");
	m_data->notEnoughPlayersText.setPosition(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
	m_data->notEnoughPlayersText.setOrigin(m_data->notEnoughPlayersText.getLocalBounds().width / 2, m_data->notEnoughPlayersText.getLocalBounds().height / 2);

	// Initialize result text
	m_data->resultText.setFont(m_data->font);
	m_data->resultText.setCharacterSize(30);
	m_data->resultText.setFillColor(sf::Color::Green);
	m_data->resultText.setPosition(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);

	m_data->buttonTexture[0].loadFromFile("Assets/Sprites/FlagGame/A.png");
	m_data->buttonTexture[1].loadFromFile("Assets/Sprites/FlagGame/B.png");
	m_data->buttonTexture[2].loadFromFile("Assets/Sprites/FlagGame/X.png");
	m_data->buttonTexture[3].loadFromFile("Assets/Sprites/FlagGame/Y.png");
	m_data->buttonTexture[4].loadFromFile("Assets/Sprites/FlagGame/LB.png");
	m_data->buttonTexture[5].loadFromFile("Assets/Sprites/FlagGame/RB.png");

	m_data->buttonSprite.setPosition(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2.5f);


	// Initialize player data for all players in m_gonnaPlayIndex
	for (int i = 0; i < 4; ++i)
	{
		m_data->playerData[i].isEliminated = true;
		m_data->playerData[i].currentInput = (GamePadBindList)(-1);
		m_data->playerData[i].eliminationOrder = 0;
		m_data->playerData[i].eliminationTime = 0.0f;

		m_data->playerData[i].inputText.setFont(m_data->font);
		m_data->playerData[i].inputText.setCharacterSize(20);
		m_data->playerData[i].inputText.setFillColor(sf::Color::Green);
	}

	// Setup only participating players
	if (m_data->gameData)
	{
		for (int i = 0; i < m_data->gameData->m_gonnaPlayIndex.size(); ++i)
		{
			int playerID = m_data->gameData->m_gonnaPlayIndex[i];
			if (playerID >= 0 && playerID < 4)
			{
				m_data->playerData[playerID].isEliminated = false;
				m_data->playersRemaining++;

				// Position text based on number of participating players
				m_data->playerData[playerID].inputText.setPosition(
					SCREEN_WIDTH / ((float)m_data->gameData->m_gonnaPlayIndex.size() + 1) * (i + 1),
					175.f
				);
				m_data->playerData[playerID].buttonSprite.setPosition(SCREEN_WIDTH / ((float)m_data->gameData->m_gonnaPlayIndex.size() + 1) * (i + 1),
					100.f);
			}
		}
	}

	UpdatePlayerInputTexts();

	// Start the game if enough players
	if (HasEnoughPlayers())
	{
		m_data->state = STATE_PLAYING;
		StartNewRound();
	}
}

void FlagGame::Unload(void)
{
	delete m_data;
	m_data = nullptr;
}

void FlagGame::PollEvent(sf::Event& _event)
{
	if (m_data->state == STATE_PLAYING)
	{
		switch (_event.type)
		{
		case sf::Event::JoystickButtonPressed:
		{
			// Get player ID from joystick
			int playerID = m_data->gameData->GetPlayerIDFromJoystick(_event.joystickButton.joystickId);

			// Check if this player is participating
			if (m_data->gameData->IsPlayerParticipating(playerID))
			{
				if (playerID >= 0 && playerID < 4 && !m_data->playerData[playerID].isEliminated)
				{
					// Convert button to GamePadBindList
					GamePadBindList input = (GamePadBindList)_event.joystickButton.button;

					if (IsInputValid(input))
					{
						m_data->playerData[playerID].currentInput = input;
						UpdatePlayerInputTexts();
					}
				}
			}
			break;
		}
		default:
			break;
		}
	}
}

void FlagGame::Update(float _deltaTime)
{
	// Track total game time during playing state
	if (m_data->state == STATE_PLAYING)
	{
		m_data->totalGameTime += _deltaTime;
	}

	if (!HasEnoughPlayers() && m_data->state != STATE_WAITING)
	{
		m_data->state = STATE_WAITING;
		return;
	}

	switch (m_data->state)
	{
	case STATE_WAITING:
		if (HasEnoughPlayers())
		{
			m_data->state = STATE_PLAYING;
			StartNewRound();
		}
		break;

	case STATE_PLAYING:
		m_data->roundTimer.Update(_deltaTime);
		m_data->inputChangeTimer.Update(_deltaTime);

		// Update timer text
		char timerBuffer[20];
		std::snprintf(timerBuffer, 20, "Time: %.2f", m_data->roundTimer.GetRemainingTime());
		m_data->timerText.setString(timerBuffer);
		m_data->timerText.setOrigin(m_data->timerText.getLocalBounds().width / 2, m_data->timerText.getLocalBounds().height / 2);

		// Change required input randomly
		if (m_data->inputChangeTimer.IsFinished() && m_data->roundTimer.GetRemainingTime() > 1.0f)
		{
			ChangeRequiredInput();
		}

		// Check if round is over
		if (m_data->roundTimer.IsFinished())
		{
			EvaluateRound();
		}
		break;

	case STATE_ROUND_END:
		m_data->roundTimer.Update(_deltaTime);

		if (m_data->roundTimer.IsFinished())
		{
			if (m_data->currentRound >= MAX_ROUND || m_data->playersRemaining <= 1)
			{
				m_data->state = STATE_GAME_OVER;
				m_data->roundTimer.SetTimeTarget(ROUND_END_TIME, true);
			}
			else
			{
				m_data->state = STATE_PLAYING;
				StartNewRound();
			}
		}
		break;

	case STATE_GAME_OVER:
		m_data->roundTimer.Update(_deltaTime);

		if (m_data->roundTimer.IsFinished())
		{
			// Return to board
			if (m_data->gameData)
			{
				// Award winner if there is one (first in m_winIndex)
				if (m_data->playersRemaining == 1)
				{
					for (int playerID : m_data->gameData->m_gonnaPlayIndex)
					{
						if (!m_data->playerData[playerID].isEliminated)
						{
							m_data->gameData->AddPlayerWin(playerID);
							std::cout << "Player " << (playerID + 1) << " wins!" << std::endl;
							break;
						}
					}
				}

				// Add losers in REVERSE order of elimination (last eliminated first, first eliminated last)
				// Create a list of eliminated players sorted by elimination order
				std::vector<std::pair<int, int>> eliminatedPlayers; // <playerID, eliminationOrder>

				for (int playerID : m_data->gameData->m_gonnaPlayIndex)
				{
					if (m_data->playerData[playerID].isEliminated && m_data->playerData[playerID].eliminationOrder > 0)
					{
						eliminatedPlayers.push_back({ playerID, m_data->playerData[playerID].eliminationOrder });
					}
				}

				// Sort by elimination order in DESCENDING order (last eliminated first)
				std::sort(eliminatedPlayers.begin(), eliminatedPlayers.end(),
					[](const std::pair<int, int>& a, const std::pair<int, int>& b) {
						return a.second > b.second; // Reversed: higher order first
					});

				// Add all losers in reverse order (first eliminated will be last)
				for (const auto& player : eliminatedPlayers)
				{
					m_data->gameData->AddPlayerWin(player.first);
					std::cout << "Player " << (player.first + 1) << " added as loser (order "
						<< player.second << ", time: "
						<< m_data->playerData[player.first].eliminationTime << "s)" << std::endl;
				}
			}

			ChangeScene("Board", false);
		}
		break;
	}
}

void FlagGame::Draw(sf::RenderWindow& _renderWindow)
{
	if (!HasEnoughPlayers())
	{
		_renderWindow.draw(m_data->notEnoughPlayersText);
		return;
	}

	_renderWindow.draw(m_data->titleText);

	if (m_data->state == STATE_PLAYING || m_data->state == STATE_ROUND_END)
	{
		_renderWindow.draw(m_data->roundText);
		_renderWindow.draw(m_data->timerText);
		_renderWindow.draw(m_data->buttonSprite);
		//_renderWindow.draw(m_data->requiredInputText);

		// Draw only participating players' texts
		if (m_data->gameData)
		{
			for (int playerID : m_data->gameData->m_gonnaPlayIndex)
			{
				if (playerID >= 0 && playerID < 4)
				{
					_renderWindow.draw(m_data->playerData[playerID].inputText);
					_renderWindow.draw(m_data->playerData[playerID].buttonSprite);
				}
			}
		}
	}

	if (m_data->state == STATE_GAME_OVER)
	{
		_renderWindow.draw(m_data->resultText);
	}
}

void FlagGame::StartNewRound(void)
{
	m_data->currentRound++;

	// Update round text
	char roundBuffer[50];
	std::snprintf(roundBuffer, 50, "Round: %d / %d", m_data->currentRound, MAX_ROUND);
	m_data->roundText.setString(roundBuffer);
	m_data->roundText.setOrigin(m_data->roundText.getLocalBounds().width / 2, m_data->roundText.getLocalBounds().height / 2);

	// Reset player inputs for participating players
	if (m_data->gameData)
	{
		for (int playerID : m_data->gameData->m_gonnaPlayIndex)
		{
			if (playerID >= 0 && playerID < 4)
			{
				m_data->playerData[playerID].currentInput = (GamePadBindList)(-1);
			}
		}
	}
	m_data->backgroundMusic.PlayMusic("Assets/Music/FlagGame_BGM.wav", true);
	UpdatePlayerInputTexts();

	// Set timers
	m_data->roundTimer.SetTimeTarget(ROUND_TIME, true);
	ChangeRequiredInput();
}

void FlagGame::EvaluateRound(void)
{
	// Check each participating player's input

	m_data->backgroundMusic.StopMusic();
	m_data->endRound.PlaySound("Assets/Sounds/EndRound.wav", false);
	if (m_data->gameData)
	{
		for (int playerID : m_data->gameData->m_gonnaPlayIndex)
		{
			if (playerID >= 0 && playerID < 4 && !m_data->playerData[playerID].isEliminated)
			{
				if (m_data->playerData[playerID].currentInput != m_data->requiredInput)
				{
					m_data->playerData[playerID].isEliminated = true;
					m_data->playersRemaining--;
					m_data->eliminationCounter++;

					// Record elimination order and time
					m_data->playerData[playerID].eliminationOrder = m_data->eliminationCounter;
					m_data->playerData[playerID].eliminationTime = m_data->totalGameTime;

					std::cout << "Player " << (playerID + 1) << " eliminated! (Order: "
						<< m_data->eliminationCounter << ", Time: "
						<< m_data->totalGameTime << "s)" << std::endl;
				}
			}
		}
	}

	UpdatePlayerInputTexts();

	// Prepare for next round or game over
	if (m_data->playersRemaining == 1)
	{
		// Find winner
		if (m_data->gameData)
		{
			for (int playerID : m_data->gameData->m_gonnaPlayIndex)
			{
				if (playerID >= 0 && playerID < 4 && !m_data->playerData[playerID].isEliminated)
				{
					char resultBuffer[50];
					std::snprintf(resultBuffer, 50, "Player %d Wins!", playerID + 1);
					m_data->resultText.setString(resultBuffer);
					m_data->resultText.setOrigin(m_data->resultText.getLocalBounds().width / 2, m_data->resultText.getLocalBounds().height / 2);
					if(!m_data->audioPlayed)
					{
						m_data->winnerSound.PlaySound("Assets/Sounds/Winner.wav", false);
						m_data->audioPlayed = true;
					}
					break;
				}
			}
		}
	}
	else if (m_data->playersRemaining == 0)
	{
		m_data->resultText.setString("No Winner!");
		m_data->resultText.setOrigin(m_data->resultText.getLocalBounds().width / 2, m_data->resultText.getLocalBounds().height / 2);
		m_data->gameOverSound.PlaySound("Assets/Sounds/GameOver.wav", false);
	}

	m_data->state = STATE_ROUND_END;
	m_data->roundTimer.SetTimeTarget(ROUND_END_TIME, true);
}

void FlagGame::ChangeRequiredInput(void)
{
	m_data->requiredInput = GetRandomValidInput();
	m_data->buttonSprite.setTexture(m_data->buttonTexture[(int)m_data->requiredInput]);
	m_data->buttonSprite.setOrigin(m_data->buttonSprite.getLocalBounds().width / 2, m_data->buttonSprite.getLocalBounds().height / 2);

	m_data->newFlagSound.PlaySound("Assets/Sounds/NewFlag.wav", false);

	char inputBuffer[100];
	std::snprintf(inputBuffer, 100, "Press: %s", GetGamePadButtonName(m_data->requiredInput));
	m_data->requiredInputText.setString(inputBuffer);
	m_data->requiredInputText.setOrigin(m_data->requiredInputText.getLocalBounds().width / 2, m_data->requiredInputText.getLocalBounds().height / 2);

	float nextChangeDelay = random::RandomFloat(INPUT_CHANGE_MIN, INPUT_CHANGE_MAX);
	m_data->inputChangeTimer.SetTimeTarget(nextChangeDelay, true);
}

GamePadBindList FlagGame::GetRandomValidInput(void)
{
	GamePadBindList validInputs[] = {
		GAMEPAD_A, GAMEPAD_B, GAMEPAD_X, GAMEPAD_Y, GAMEPAD_LB, GAMEPAD_RB
	};

	int randomIndex = random::RandomInt(0, 5);
	return validInputs[randomIndex];
}

bool FlagGame::IsInputValid(GamePadBindList _input)
{
	return (_input == GAMEPAD_A || _input == GAMEPAD_B ||
		_input == GAMEPAD_X || _input == GAMEPAD_Y ||
		_input == GAMEPAD_LB || _input == GAMEPAD_RB);
}

void FlagGame::UpdatePlayerInputTexts(void)
{
	if (!m_data->gameData)
		return;

	for (int playerID : m_data->gameData->m_gonnaPlayIndex)
	{
		if (playerID < 0 || playerID >= 4)
			continue;

		if (m_data->playerData[playerID].isEliminated)
		{
			m_data->playerData[playerID].inputText.setFillColor(sf::Color::Red);
			m_data->playerData[playerID].buttonSprite.setColor(sf::Color::Red);
			char buffer[50];
			std::snprintf(buffer, 50, "Player %d\nELIMINATED", playerID + 1);
			m_data->playerData[playerID].inputText.setString(buffer);
		}
		else
		{
			m_data->playerData[playerID].inputText.setFillColor(sf::Color::Green);
			m_data->playerData[playerID].buttonSprite.setColor(sf::Color::Green);

			if (m_data->playerData[playerID].currentInput != (GamePadBindList)(-1))
			{
				char buffer[100];
				std::snprintf(buffer, 100, "Player %d\n%s", playerID + 1,
					GetGamePadButtonName(m_data->playerData[playerID].currentInput));
				m_data->playerData[playerID].inputText.setString(buffer);

				m_data->playerData[playerID].buttonSprite.setColor(sf::Color::White);
				m_data->playerData[playerID].buttonSprite.setTexture(
					m_data->buttonTexture[(int)m_data->playerData[playerID].currentInput]
				);
				m_data->playerData->inputPressed.PlaySound("Assets/Sounds/InputPressed.wav", false);
			}
			else
			{
				char buffer[50];
				std::snprintf(buffer, 50, "Player %d\nWaiting...", playerID + 1);
				m_data->playerData[playerID].inputText.setString(buffer);
			}
		}

		m_data->playerData[playerID].inputText.setOrigin(
			m_data->playerData[playerID].inputText.getLocalBounds().width / 2,
			m_data->playerData[playerID].inputText.getLocalBounds().height / 2
		);
		m_data->playerData[playerID].buttonSprite.setOrigin(
			m_data->playerData[playerID].buttonSprite.getLocalBounds().width / 2,
			m_data->playerData[playerID].buttonSprite.getLocalBounds().height / 2
		);
	}
}

bool FlagGame::HasEnoughPlayers(void)
{
	if (!m_data->gameData)
		return false;

	return m_data->gameData->m_gonnaPlayIndex.size() >= 2;
}

int FlagGame::GetFirstEliminatedPlayer(void)
{
	int firstEliminatedID = -1;
	int lowestOrder = INT_MAX;

	if (!m_data->gameData)
		return -1;

	// Find player with elimination order of 1 (first eliminated)
	for (int playerID : m_data->gameData->m_gonnaPlayIndex)
	{
		if (playerID >= 0 && playerID < 4)
		{
			if (m_data->playerData[playerID].isEliminated &&
				m_data->playerData[playerID].eliminationOrder > 0 &&
				m_data->playerData[playerID].eliminationOrder < lowestOrder)
			{
				lowestOrder = m_data->playerData[playerID].eliminationOrder;
				firstEliminatedID = playerID;
			}
		}
	}

	return firstEliminatedID;
}