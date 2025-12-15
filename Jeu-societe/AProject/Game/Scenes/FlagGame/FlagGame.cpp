#include "FlagGame.hpp"

#define ROUND_TIME 5.0f
#define ROUND_END_TIME 3.0f
#define INPUT_CHANGE_MIN 0.5f
#define INPUT_CHANGE_MAX 3.0f

void FlagGame::Load(void)
{
	m_data = new SceneData();
	m_data->gameData = (GameData*)this->m_keptData;

	if (((GameData*)this->m_keptData)->m_gonnaPlayIndex.size() == 0)
	{
		((GameData*)this->m_keptData)->m_gonnaPlayIndex.push_back(0);
		((GameData*)this->m_keptData)->m_gonnaPlayIndex.push_back(1);
		((GameData*)this->m_keptData)->m_gonnaPlayIndex.push_back(2);
		((GameData*)this->m_keptData)->m_gonnaPlayIndex.push_back(3);
	}

	m_data->gameData->m_assetManager->LoadManifest("Manifests/FlagGame.json", "FlagGame");
	m_data->state = STATE_WAITING;
	m_data->currentRound = 0;
	m_data->playersRemaining = 0;
	m_data->eliminationCounter = 0;
	m_data->totalGameTime = 0.0f;

	// NOUVEAU : Initialiser les valeurs de difficulté
	m_data->currentInputChangeMin = INPUT_CHANGE_MIN;
	m_data->currentInputChangeMax = INPUT_CHANGE_MAX;

	// Initialize background sprite
	m_data->backgroundSprite.setTexture(*m_data->gameData->m_assetManager->GetAsset<sf::Texture>("MinigameBackground"));
	m_data->backgroundSprite.setPosition(0, 0);

	// Initialize title text
	m_data->titleText.setFont(*m_data->gameData->m_assetManager->GetAsset<sf::Font>("FlagFont"));
	m_data->titleText.setCharacterSize(24);
	m_data->titleText.setFillColor(sf::Color::White);
	m_data->titleText.setString("Flag Game Mini-Game");
	m_data->titleText.setPosition(SCREEN_WIDTH / 2, 20);
	m_data->titleText.setOrigin(m_data->titleText.getLocalBounds().width / 2, 0);

	// Initialize round text
	m_data->roundText.setFont(*m_data->gameData->m_assetManager->GetAsset<sf::Font>("FlagFont"));
	m_data->roundText.setCharacterSize(25);
	m_data->roundText.setFillColor(sf::Color::Blue);
	m_data->roundText.setPosition(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 4);

	// Initialize timer text
	m_data->timerText.setFont(*m_data->gameData->m_assetManager->GetAsset<sf::Font>("FlagFont"));
	m_data->timerText.setCharacterSize(25);
	m_data->timerText.setFillColor(sf::Color::White);
	m_data->timerText.setPosition(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 3);

	// Initialize required input text
	m_data->requiredInputText.setFont(*m_data->gameData->m_assetManager->GetAsset<sf::Font>("FlagFont"));
	m_data->requiredInputText.setCharacterSize(25);
	m_data->requiredInputText.setFillColor(sf::Color::Yellow);
	m_data->requiredInputText.setPosition(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2.5f);

	// Initialize not enough players text
	m_data->notEnoughPlayersText.setFont(*m_data->gameData->m_assetManager->GetAsset<sf::Font>("FlagFont"));
	m_data->notEnoughPlayersText.setCharacterSize(25);
	m_data->notEnoughPlayersText.setFillColor(sf::Color::Red);
	m_data->notEnoughPlayersText.setString("Not Enough Players Connected!");
	m_data->notEnoughPlayersText.setPosition(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
	m_data->notEnoughPlayersText.setOrigin(m_data->notEnoughPlayersText.getLocalBounds().width / 2, m_data->notEnoughPlayersText.getLocalBounds().height / 2);

	// Initialize result text
	m_data->resultText.setFont(*m_data->gameData->m_assetManager->GetAsset<sf::Font>("FlagFont"));
	m_data->resultText.setCharacterSize(30);
	m_data->resultText.setFillColor(sf::Color::Green);
	m_data->resultText.setPosition(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);

	m_data->buttonSprite.setTexture(*m_data->gameData->m_assetManager->GetAsset<TextureAtlas>("Input"));
	m_data->buttonSprite.setOrigin({ 0.5f,0.5f });
	m_data->buttonSprite.setPosition(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2.5f);

	m_data->buttonSprite.SetTextureFrame("A");

	m_data->stringTab[0] = "A";
	m_data->stringTab[1] = "B";
	m_data->stringTab[2] = "X";
	m_data->stringTab[3] = "Y";
	m_data->stringTab[4] = "LB";
	m_data->stringTab[5] = "RB";

	// Initialize player data for all players in m_gonnaPlayIndex
	for (int i = 0; i < 4; ++i)
	{
		m_data->playerData[i].isEliminated = true;
		m_data->playerData[i].currentInput = (GamePadBindList)(-1);
		m_data->playerData[i].eliminationOrder = 0;
		m_data->playerData[i].eliminationTime = 0.0f;
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

				// Position des éléments en bas de l'écran
				float spacing = SCREEN_WIDTH / ((float)m_data->gameData->m_gonnaPlayIndex.size() + 1);
				float xPos = spacing * (i + 1);

				// Icône du joueur en bas
				InitializePlayerIcon(playerID, i);
				m_data->playerData[playerID].playerIcone.setPosition(xPos, SCREEN_HEIGHT);

				// Bouton au-dessus de l'icône
				m_data->playerData[playerID].buttonSprite.setOrigin({ 0.5f, 0.5f });
				m_data->playerData[playerID].buttonSprite.setPosition(xPos, SCREEN_HEIGHT - 250.f);
				m_data->playerData[playerID].buttonSprite.setTexture(*m_data->gameData->m_assetManager->GetAsset<TextureAtlas>("Input"));
				m_data->playerData[playerID].buttonSprite.SetTextureFrame("A");
				m_data->playerData[playerID].buttonSprite.setColor(sf::Color::White);
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

void FlagGame::InitializePlayerIcon(int playerID, int positionIndex)
{
	// Initialiser les sprites d'icône et d'aura pour le joueur
	m_data->playerData[playerID].playerIcone.setTexture(*m_data->gameData->m_assetManager->GetAsset<TextureAnimated>("Icone"));

	m_data->playerData[playerID].playerIcone.setOrigin({ 0.5f, 1.f });

	m_data->playerData[playerID].playerIcone.setScale({ 0.8f, 0.8f });

	// Définir l'animation selon le skin du joueur
	switch (m_data->gameData->m_playerDataList[playerID].GetPlayerSkin())
	{
	case PlayerData::CHARACTER_1_1:
		m_data->playerData[playerID].playerIcone.SetAnimation("Perso1-1");
		break;
	case PlayerData::CHARACTER_1_2:
		m_data->playerData[playerID].playerIcone.SetAnimation("Perso1-2");
		break;
	case PlayerData::CHARACTER_2_1:
		m_data->playerData[playerID].playerIcone.SetAnimation("Perso2-1");
		break;
	case PlayerData::CHARACTER_2_2:
		m_data->playerData[playerID].playerIcone.SetAnimation("Perso2-2");
		break;
	case PlayerData::CHARACTER_3_1:
		m_data->playerData[playerID].playerIcone.SetAnimation("Perso3-1");
		break;
	case PlayerData::CHARACTER_3_2:
		m_data->playerData[playerID].playerIcone.SetAnimation("Perso3-2");
		break;
	case PlayerData::CHARACTER_4_1:
		m_data->playerData[playerID].playerIcone.SetAnimation("Perso4-1");
		break;
	case PlayerData::CHARACTER_4_2:
		m_data->playerData[playerID].playerIcone.SetAnimation("Perso4-2");
		break;
	default:
		break;
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
				// Check if it's a tie (all players eliminated simultaneously)
				int totalPlayers = (int)m_data->gameData->m_gonnaPlayIndex.size();
				int eliminatedPlayers = 0;
				int maxEliminationOrder = 0;
				int playersWithMaxOrder = 0;

				// Count eliminated players and find the highest elimination order
				for (int playerID : m_data->gameData->m_gonnaPlayIndex)
				{
					if (m_data->playerData[playerID].isEliminated)
					{
						eliminatedPlayers++;
						if (m_data->playerData[playerID].eliminationOrder > maxEliminationOrder)
						{
							maxEliminationOrder = m_data->playerData[playerID].eliminationOrder;
							playersWithMaxOrder = 1;
						}
						else if (m_data->playerData[playerID].eliminationOrder == maxEliminationOrder)
						{
							playersWithMaxOrder++;
						}
					}
				}

				// If all players are eliminated and multiple players share the last elimination order, it's a tie
				bool isTie = (m_data->playersRemaining == 0 && playersWithMaxOrder > 1);

				if (isTie)
				{
					// Tie scenario - don't add anyone to the win list
					std::cout << "Game ended in a tie! No winners added to win list." << std::endl;
				}
				else if (m_data->playersRemaining == 1)
				{
					// Award winner if there is one
					for (int playerID : m_data->gameData->m_gonnaPlayIndex)
					{
						if (!m_data->playerData[playerID].isEliminated)
						{
							m_data->gameData->AddPlayerWin(playerID);
							std::cout << "Player " << (playerID + 1) << " wins!" << std::endl;
							break;
						}
					}

					// Add losers in REVERSE order of elimination (last eliminated first, first eliminated last)
					std::vector<std::pair<int, int>> eliminatedPlayersList;

					for (int playerID : m_data->gameData->m_gonnaPlayIndex)
					{
						if (m_data->playerData[playerID].isEliminated && m_data->playerData[playerID].eliminationOrder > 0)
						{
							eliminatedPlayersList.push_back({ playerID, m_data->playerData[playerID].eliminationOrder });
						}
					}

					// Sort by elimination order in DESCENDING order (last eliminated first)
					std::sort(eliminatedPlayersList.begin(), eliminatedPlayersList.end(),
						[](const std::pair<int, int>& a, const std::pair<int, int>& b) {
							return a.second > b.second;
						});

					// Add all losers in reverse order
					for (const auto& player : eliminatedPlayersList)
					{
						m_data->gameData->AddPlayerWin(player.first);
						std::cout << "Player " << (player.first + 1) << " added as loser (order "
							<< player.second << ", time: "
							<< m_data->playerData[player.first].eliminationTime << "s)" << std::endl;
					}
				}
			}

			ChangeScene("Board", false);
		}
		break;
	}
}

void FlagGame::Draw(sf::RenderWindow& _renderWindow)
{
	sfMod::RenderWindow& renderWindow = *m_data->gameData->m_renderWindow;

	// IMPORTANT : Dessiner le background en PREMIER
	renderWindow.draw(m_data->backgroundSprite);

	if (!HasEnoughPlayers())
	{
		renderWindow.draw(m_data->notEnoughPlayersText);
		return;
	}

	renderWindow.draw(m_data->titleText);

	if (m_data->state == STATE_PLAYING || m_data->state == STATE_ROUND_END)
	{
		renderWindow.draw(m_data->roundText);
		renderWindow.draw(m_data->timerText);
		renderWindow.draw(m_data->buttonSprite);

		// Draw only participating players' icons and buttons at the bottom
		if (m_data->gameData)
		{
			for (int playerID : m_data->gameData->m_gonnaPlayIndex)
			{
				if (playerID >= 0 && playerID < 4)
				{
					// Dessiner l'icône
					renderWindow.draw(m_data->playerData[playerID].playerIcone);
					// Dessiner le bouton au-dessus de l'icône
					renderWindow.draw(m_data->playerData[playerID].buttonSprite);
				}
			}
		}
	}

	if (m_data->state == STATE_GAME_OVER)
	{
		renderWindow.draw(m_data->resultText);
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
	UpdatePlayerInputTexts();

	// NOUVEAU : Augmenter la difficulté progressivement
	// Réduire le temps du round après le premier
	float roundTime = ROUND_TIME;
	if (m_data->currentRound > 1)
	{
		// Réduire de 0.5 seconde par round (min 2 secondes)
		roundTime = std::max(2.0f, ROUND_TIME - (m_data->currentRound - 1) * 0.5f);
	}

	// NOUVEAU : Accélérer le changement d'input requis
	float inputChangeMin = INPUT_CHANGE_MIN;
	float inputChangeMax = INPUT_CHANGE_MAX;
	
	if (m_data->currentRound > 1)
	{
		// Réduire les intervalles de changement d'input
		float reduction = (m_data->currentRound - 1) * 0.15f;
		inputChangeMin = std::max(0.2f, INPUT_CHANGE_MIN - reduction);
		inputChangeMax = std::max(0.8f, INPUT_CHANGE_MAX - reduction * 1.5f);
	}

	// Set timers avec les nouvelles valeurs
	m_data->roundTimer.SetTimeTarget(roundTime, true);
	m_data->inputChangeTimer.SetTimeTarget(random::RandomFloat(inputChangeMin, inputChangeMax), true);
	
	ChangeRequiredInput();
	
	// Stocker les valeurs pour le prochain changement d'input
	m_data->currentInputChangeMin = inputChangeMin;
	m_data->currentInputChangeMax = inputChangeMax;
}

void FlagGame::EvaluateRound(void)
{
	// Track players eliminated this round
	std::vector<int> playersEliminatedThisRound;

	// Check each participating player's input
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

					m_data->playerData[playerID].buttonSprite.setColor(sf::Color::Red);

					// Record elimination order and time
					m_data->playerData[playerID].eliminationOrder = m_data->eliminationCounter;
					m_data->playerData[playerID].eliminationTime = m_data->totalGameTime;

					playersEliminatedThisRound.push_back(playerID);

					std::cout << "Player " << (playerID + 1) << " eliminated! (Order: "
						<< m_data->eliminationCounter << ", Time: "
						<< m_data->totalGameTime << "s)" << std::endl;
				}
				else
				{
					m_data->playerData[playerID].buttonSprite.setColor(sf::Color::Green);
				}
			}
		}
	}

	UpdatePlayerInputTexts();

	// Check if all remaining players were eliminated this round (tie scenario)
	bool isTie = (m_data->playersRemaining == 0 && playersEliminatedThisRound.size() > 1);

	// Prepare for next round or game over
	if (isTie)
	{
		// It's a tie - all players lost simultaneously
		m_data->resultText.setString("Tie! No Winner!");
		m_data->resultText.setOrigin(m_data->resultText.getLocalBounds().width / 2, m_data->resultText.getLocalBounds().height / 2);
		std::cout << "Tie detected! All remaining players eliminated in round " << m_data->currentRound << std::endl;
	}
	else if (m_data->playersRemaining == 1)
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
					break;
				}
			}
		}
	}
	else if (m_data->playersRemaining == 0)
	{
		m_data->resultText.setString("No Winner!");
		m_data->resultText.setOrigin(m_data->resultText.getLocalBounds().width / 2, m_data->resultText.getLocalBounds().height / 2);
	}

	m_data->state = STATE_ROUND_END;
	m_data->roundTimer.SetTimeTarget(ROUND_END_TIME, true);
}

void FlagGame::ChangeRequiredInput(void)
{
	m_data->requiredInput = GetRandomValidInput();
	m_data->buttonSprite.SetTextureFrame(m_data->stringTab[(int)m_data->requiredInput]);

	char inputBuffer[100];
	std::snprintf(inputBuffer, 100, "Press: %s", GetGamePadButtonName(m_data->requiredInput));
	m_data->requiredInputText.setString(inputBuffer);
	m_data->requiredInputText.setOrigin(m_data->requiredInputText.getLocalBounds().width / 2, m_data->requiredInputText.getLocalBounds().height / 2);

	// MODIFICATION : Utiliser les valeurs dynamiques de difficulté
	float nextChangeDelay = random::RandomFloat(
		m_data->currentInputChangeMin, 
		m_data->currentInputChangeMax
	);
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
			m_data->playerData[playerID].buttonSprite.setColor(sf::Color::Red);
			m_data->playerData[playerID].playerIcone.setColor(sf::Color(255, 100, 100, 255));
		}
		else
		{
			m_data->playerData[playerID].buttonSprite.setColor(sf::Color::Green);
			m_data->playerData[playerID].playerIcone.setColor(sf::Color::White);

			if (m_data->playerData[playerID].currentInput != (GamePadBindList)(-1))
			{
				// Afficher le bouton du JOUEUR
				m_data->playerData[playerID].buttonSprite.setColor(sf::Color::White);
				m_data->playerData[playerID].buttonSprite.SetTextureFrame(m_data->stringTab[(int)m_data->playerData[playerID].currentInput]);
			}
		}
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