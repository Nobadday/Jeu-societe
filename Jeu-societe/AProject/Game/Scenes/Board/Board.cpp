#include "Board.hpp"

constexpr unsigned int hash(const char* str, int h);

constexpr unsigned int hash(const char* str, int h = 0)
{
	return !str[h] ? 5381 : (hash(str, h + 1) * 33) ^ str[h];
}

void BaseGame::Load(void)
{
	m_data = new SceneData;
	m_gameData = (GameData*)this->m_keptData;

	m_gameData->m_assetManager->LoadManifest("Manifests/Board.json", "Board");

	m_data->tile.InitTiled("Assets/Map/map.json");
	m_data->camera.Reset(m_gameData->m_renderWindow->getDefaultView());

	MapLayer layer = m_data->tile.GetMapLayer("point");
	m_data->posCase = layer.GetObjects();

	m_data->players.resize(m_gameData->m_playerDataList.size());
	m_data->state = START;

	m_data->timeWin = TIME_WIN_DISPLAY;

	// Initialisation des joueurs
	for (int i = 0; i < m_data->players.size(); i++)
	{
		switch (m_gameData->m_playerDataList[i].GetPlayerSkin())
		{
		case PlayerData::CHARACTER_1_1:
			m_data->players[i].texture = *m_gameData->m_assetManager->GetAsset<TextureAnimated>("Perso1-1", AssetManager::AssetType::TEXTURE_ANIMATED);
			break;
		case PlayerData::CHARACTER_2_1:
			m_data->players[i].texture = *m_gameData->m_assetManager->GetAsset<TextureAnimated>("Perso2-1", AssetManager::AssetType::TEXTURE_ANIMATED);
			break;
		case PlayerData::CHARACTER_3_1:
			m_data->players[i].texture = *m_gameData->m_assetManager->GetAsset<TextureAnimated>("Perso3-1", AssetManager::AssetType::TEXTURE_ANIMATED);
			break;
		case PlayerData::CHARACTER_4_1:
			m_data->players[i].texture = *m_gameData->m_assetManager->GetAsset<TextureAnimated>("Perso4-1", AssetManager::AssetType::TEXTURE_ANIMATED);
			break;
		case PlayerData::CHARACTER_1_2:
			m_data->players[i].texture = *m_gameData->m_assetManager->GetAsset<TextureAnimated>("Perso1-2", AssetManager::AssetType::TEXTURE_ANIMATED);
			break;
		case PlayerData::CHARACTER_2_2:
			m_data->players[i].texture = *m_gameData->m_assetManager->GetAsset<TextureAnimated>("Perso2-2", AssetManager::AssetType::TEXTURE_ANIMATED);
			break;
		case PlayerData::CHARACTER_3_2:
			m_data->players[i].texture = *m_gameData->m_assetManager->GetAsset<TextureAnimated>("Perso3-2", AssetManager::AssetType::TEXTURE_ANIMATED);
			break;
		case PlayerData::CHARACTER_4_2:
			m_data->players[i].texture = *m_gameData->m_assetManager->GetAsset<TextureAnimated>("Perso4-2", AssetManager::AssetType::TEXTURE_ANIMATED);
			break;
		default:
			m_data->players[i].texture = *m_gameData->m_assetManager->GetAsset<TextureAnimated>("Perso1-1", AssetManager::AssetType::TEXTURE_ANIMATED);
			break;
		}

		m_data->players[i].sprite.setTexture(m_data->players[i].texture);

		m_data->players[i].sprite.setOrigin({ 0.5f,1.f });

		m_data->players[i].boardPosition = m_data->posCase[0].GetPosition();
		m_data->players[i].currentCaseIndex = 0;
		m_data->players[i].startRandom = 0;
		m_data->players[i].state = StatePlayer::NONE;
		m_data->players[i].pendingMovement = 0;      // Nouveau
		m_data->players[i].currentPathId = -1;       // Nouveau : -1 = chemin principal
	}

	m_data->currentPlayerIndex = 0;

	m_data->smokeTp.loadFromFile("Assets/Sprites/Board/smoke.png");

	// Position initiale de la caméra : afficher tous les joueurs
	UpdateCameraToShowAllPlayers();

	// Configuration des animateurs
	m_data->animator.Modify(1.0f, 60.0f, false, 1.0f);
	m_data->animator2.Modify(1.0f, 60.0f, false, 1.0f);
	m_data->animator.SetAnimationEasing(anim::Animator::GOTO, anim::Easing::INOUTSINE);
	m_data->animator2.SetAnimationEasing(anim::Animator::GOTO, anim::Easing::INOUTSINE);
	m_data->animator.End();
	m_data->animator2.End();
}

void BaseGame::Unload(void)
{
	m_gameData->m_assetManager->DeleteContainer("Board");
	this->m_gameData = NULL;
	delete this->m_data;
	this->m_data = NULL;
}

void BaseGame::PollEvent(sf::Event& _event)
{

	if (_event.type == sf::Event::KeyPressed)
	{
		if (_event.key.code == sf::Keyboard::R)
		{
			SwapPlayers();
		}
	}

	// Gestion du lancer de dé pour le pont
	if (m_data->state == WAITING_BRIDGE_ROLL)
	{
		bool shouldRoll = false;

		// Gestion joystick
		if (_event.type == sf::Event::JoystickButtonPressed)
		{
			if (m_gameData->m_playerDataList[m_data->currentPlayerIndex].m_joystickId == _event.joystickButton.joystickId)
			{
				if (_event.joystickButton.button == 0)
				{
					shouldRoll = true;
				}
			}
		}

		// Gestion clavier (DEBUG)
		if (_event.type == sf::Event::KeyPressed)
		{
			if (_event.key.code == sf::Keyboard::Space)
			{
				shouldRoll = true;
			}
		}

		if (shouldRoll)
		{
			ProcessBridgeRoll();
		}

		return;
	}

	// Si on attend un choix de chemin
	if (m_data->state == WAITING_PATH_CHOICE)
	{
		// Gestion joystick
		if (_event.type == sf::Event::JoystickMoved)
		{
			unsigned int joyId = _event.joystickMove.joystickId;
			if (m_gameData->m_playerDataList[m_data->currentPlayerIndex].m_joystickId == joyId)
			{
				// Axe vertical (Y) : haut = choix 0, bas = choix 1
				if (_event.joystickMove.axis == sf::Joystick::Y)
				{
					if (_event.joystickMove.position < -50.0f) // Haut
					{
						ProcessPathChoice(0);
						std::cout << "Chemin du haut choisi (joystick)" << std::endl;
					}
					else if (_event.joystickMove.position > 50.0f) // Bas
					{
						ProcessPathChoice(1);
						std::cout << "Chemin du bas choisi (joystick)" << std::endl;
					}
				}
			}
		}

		if (_event.type == sf::Event::JoystickButtonPressed)
		{
			if (m_gameData->m_playerDataList[m_data->currentPlayerIndex].m_joystickId == _event.joystickButton.joystickId)
			{
				if (_event.joystickButton.button == 0)
				{
					m_data->players[m_data->currentPlayerIndex].sprite.SetAnimation("Idle");
					SetBoardState(DEPLACEMENT_SPLIT);
				}
			}
		}

		// DEBUG : Touches clavier
		if (_event.type == sf::Event::KeyPressed)
		{
			if (_event.key.code == sf::Keyboard::Up || _event.key.code == sf::Keyboard::Z)
			{
				ProcessPathChoice(0);
				std::cout << "Chemin du haut choisi (clavier)" << std::endl;
			}
			else if (_event.key.code == sf::Keyboard::Down || _event.key.code == sf::Keyboard::S)
			{
				ProcessPathChoice(1);
				std::cout << "Chemin du bas choisi (clavier)" << std::endl;
			}

			if (_event.key.code == sf::Keyboard::Space)
			{
				m_data->players[m_data->currentPlayerIndex].sprite.SetAnimation("");
				SetBoardState(DEPLACEMENT_SPLIT);
			}

		}



		return; // Ne pas traiter d'autres événements pendant le choix
	}

	// Lambda pour factoriser le comportement du lancer de dé
	auto processDiceRoll = [this](int rando)
		{
			std::cout << "Roll Dice: " << rando << std::endl;
			std::cout << "Player startRandom: " << m_data->players[m_data->currentPlayerIndex].startRandom << std::endl;

			const int posCaseCount = static_cast<int>(m_data->posCase.size());

			//m_data->animator.Modify((float)rando, 60.0f, false, 1.0f);

			if (m_data->state != START)
			{
				auto& player = m_data->players[m_data->currentPlayerIndex];

				// CORRECTION : Initialiser le mouvement restant
				player.pendingMovement = rando;

				// Calculer le prochain index (première case du déplacement)
				int nextIndex = 0;

				if (player.state != NONE)
				{
					player.tourstate += 1;
					if (player.tourstate >= MAX_TOUR_EFFECT)
					{
						player.tourstate = 0;
						player.state = StatePlayer::NONE;
					}
				}

				if (player.state != StatePlayer::CONFUSED)
				{
					SetBoardState(DEPLACEMENT, nextIndex);
				}
				else
				{
					player.tourstate = 0;
					player.state = StatePlayer::NONE;
					SetBoardState(DEPLACEMENT_ACTION_BACK, nextIndex);
				}
			}
			else
			{
				m_data->players[m_data->currentPlayerIndex].startRandom = rando;
				std::cout << "Place: " << rando << std::endl;
			}
		};

	// Gestion des entrées joystick
	if (_event.type == sf::Event::JoystickButtonPressed)
	{
		if (m_gameData->m_playerDataList[m_data->currentPlayerIndex].m_joystickId == _event.joystickButton.joystickId && m_data->state != WIN_DEPLACEMENT)
		{
			if (_event.joystickButton.button == 0 && m_data->animator.IsFinished())
			{
				int rando = randmt::RandomInt(1, 6);
				processDiceRoll(rando);
			}
		}
	}

	// Gestion des entrées clavier (DEBUG)
	if (_event.type == sf::Event::KeyPressed)
	{
		if (_event.key.code == sf::Keyboard::Space && m_data->animator.IsFinished() && m_data->state != WIN_DEPLACEMENT)
		{
			int rando = randmt::RandomInt(1, 6);
			processDiceRoll(rando);
		}
	}

}

void BaseGame::Update(float _deltaTime)
{
	// Mise à jour des animations
	if (m_data->players[m_data->currentPlayerIndex].state != CANT_PLAY)
	{
		m_data->animator.Update(_deltaTime);
		m_data->animator2.Update(_deltaTime);

		// Mise à jour de la logique du plateau
		BoardStateUpdate(_deltaTime);
		m_data->players[m_data->currentPlayerIndex].sprite.Update(_deltaTime);

		for (int i = m_data->effects.size() - 1; i >= 0 ; i-- )
		{
			auto& effect = m_data->effects[i];

			effect.Update(_deltaTime);

			if (!effect.IsActive())
			{
				effect = m_data->effects.back();
				m_data->effects.pop_back();
			}
		}

		// Mise à jour de la caméra pour suivre le joueur actif
		UpdateCameraFollowPlayer(_deltaTime);

	}
	else
	{
		m_data->players[m_data->currentPlayerIndex].tourstate = 0;
		m_data->players[m_data->currentPlayerIndex].state = StatePlayer::NONE;
		SetBoardState(PLAY, 0);
	}

}

void BaseGame::Draw(sf::RenderWindow& _renderWindow)
{
	const sf::View& referenceView = m_data->camera.GetView();
	_renderWindow.setView(referenceView);

	std::string layer = "point";

	m_data->tile.DrawMapLayers(_renderWindow, referenceView.getCenter(), layer);

	// Créer un vecteur d'indices pour trier les joueurs par position Y
	std::vector<size_t> indices(m_data->players.size());
	std::iota(indices.begin(), indices.end(), 0);

	// Trier les indices par position Y croissante
	std::sort(indices.begin(), indices.end(),
		[this](size_t a, size_t b) {
			return m_data->players[a].boardPosition.y < m_data->players[b].boardPosition.y;
		});

	// Affichage des joueurs dans l'ordre trié
	for (size_t idx : indices)
	{
		m_data->players[idx].sprite.setPosition(m_data->players[idx].boardPosition);
		_renderWindow.draw(m_data->players[idx].sprite);
	}

	m_data->tile.DrawMapLayers(_renderWindow, referenceView.getCenter(), "point");

	for (auto& effect : m_data->effects)
	{
		effect.Draw(_renderWindow);
	}
}

void BaseGame::CaseAction()
{
	const std::string& caseType = m_data->posCase[m_data->players[m_data->currentPlayerIndex].currentCaseIndex].GetType();

	int sameCase = OnSameCase();
	if (sameCase != -1)
	{
		SetBoardState(DUEL, 0);
		return;
	}


	switch (hash(caseType.c_str()))
	{
		case hash("Bonus"):

			if (m_data->players[m_data->currentPlayerIndex].state != StatePlayer::INFEC)
			{
				std::cout << "Landed on a Bonus case!" << std::endl;

				BonusMalusLuck(false);

			}

			break;

		case hash("Malus"):

			if (m_data->players[m_data->currentPlayerIndex].state != StatePlayer::IMMUN)
			{
				std::cout << "Landed on a Malus case!" << std::endl;

				BonusMalusLuck(true);
			}
			break;

		case hash("Luck"):
			std::cout << "Landed on a Luck case!" << std::endl;
			BonusMalusLuck(randmt::Chance(0.5f));
			break;

		case hash("Battle"):
			std::cout << "Landed on a Battle case!" << std::endl;
			SetBoardState(BATTLE_ACTION);
			break;

	default:
	{
		SetBoardState(PLAY, 0);
	}
	break;
	}
}

void BaseGame::SetBoardState(State _state, int _newIndex)
{
	m_data->state = _state;

	switch (m_data->state)
	{
	case START:
		break;
	case PLAY:
		m_data->players[m_data->currentPlayerIndex].sprite.setScale({ 1.f,1.f });
		m_data->players[m_data->currentPlayerIndex].sprite.SetAnimation("Idle");
		m_data->pathChoices.clear();
		m_data->currentPlayerIndex = (m_data->currentPlayerIndex + 1) % m_data->players.size();
		break;
	case WIN:
		m_data->players[m_data->currentPlayerIndex].sprite.SetAnimation("Idle");
		break;

	case DEPLACEMENT:
		[[fallthrough]];
	case DEPLACEMENT_ACTION:
	{
		auto& player = m_data->players[m_data->currentPlayerIndex];

		// Vérifier s'il y a un choix de chemin à cette case
		if (HasPathChoice(player.currentCaseIndex) && player.pendingMovement > 0)
		{
			m_data->pathChoices = GetAvailablePaths(player.currentCaseIndex);
			m_data->state = WAITING_PATH_CHOICE;
			std::cout << "Choix de chemin requis : " << m_data->pathChoices.size() << " options" << std::endl;
			player.sprite.SetAnimation("Idle");
			return;
		}

		// Vérifier si on arrive sur un pont
		const MapObject& currentCase = m_data->posCase[player.currentCaseIndex];
		std::string caseType = "";
		if (currentCase.GetPropertyByName("type") != nullptr)
		{
			caseType = currentCase.GetPropertyByName("type")->GetStringValue();
		}
		if (caseType == "bridge" && player.pendingMovement > 0)
		{
			player.sprite.SetAnimation("Idle");
			std::cout << "Pont détecté ! Lancez le dé pour traverser..." << std::endl;
			player.waitingBridgeRoll = true;
			SetBoardState(WAITING_BRIDGE_ROLL);
			return;
		}

		// Pas de choix : avancer automatiquement
		std::vector<int> availablePaths = GetAvailablePaths(player.currentCaseIndex);
		if (availablePaths.empty())
		{
			std::cout << "Erreur : aucun chemin disponible!" << std::endl;
			SetBoardState(CASE_ACTION_END);
			return;
		}

		int nextIndex = availablePaths[0]; // Un seul chemin disponible

		sf::Vector2f startPos = player.boardPosition;
		sf::Vector2f endPos = m_data->posCase[nextIndex].GetPosition() +
			sf::Vector2f{ randmt::RandomFloat(-10, 10), randmt::RandomFloat(-10, 10) };

		m_data->animator.SetGoTo(startPos, endPos);
		m_data->animator.Restart();

		player.currentCaseIndex = nextIndex;
		player.sprite.SetAnimation("Right_Walk");

		// Vérifier si on arrive sur une convergence
		const MapObject& nextCase = m_data->posCase[nextIndex];
		if (nextCase.GetType() == "merge")
		{
			player.currentPathId = -1; // Retour au chemin principal
		}
	}
	break;

	case DEPLACEMENT_BRIGE:
		[[fallthrough]];
	case DEPLACEMENT_SPLIT:
	{
		auto& player = m_data->players[m_data->currentPlayerIndex];


		std::vector<int> availablePaths = GetAvailablePaths(player.currentCaseIndex);
		if (availablePaths.empty())
		{
			std::cout << "Erreur : aucun chemin disponible!" << std::endl;
			SetBoardState(CASE_ACTION_END);
			return;
		}

		int nextIndex = availablePaths[0];

		if (m_data->players[m_data->currentPlayerIndex].currentPathId != -1)
		{
			nextIndex = availablePaths[m_data->players[m_data->currentPlayerIndex].currentPathId - 1];
		}


		sf::Vector2f startPos = player.boardPosition;
		sf::Vector2f endPos = m_data->posCase[nextIndex].GetPosition() +
			sf::Vector2f{ randmt::RandomFloat(-10, 10), randmt::RandomFloat(-10, 10) };

		m_data->animator.SetGoTo(startPos, endPos);
		m_data->animator.Restart();

		player.currentCaseIndex = nextIndex;
		player.sprite.SetAnimation("Right_Walk");

		// Vérifier si on arrive sur une convergence
		const MapObject& nextCase = m_data->posCase[nextIndex];
		if (nextCase.GetType() == "merge")
		{
			player.currentPathId = -1; // Retour au chemin principal
		}
	}
	break;

	case DEPLACEMENT_ACTION_BACK:
	{
		auto& player = m_data->players[m_data->currentPlayerIndex];


		std::vector<int> availablePaths = GetAvailablePathsBack(player.currentCaseIndex);
		if (availablePaths.empty())
		{
			std::cout << "Erreur : aucun chemin disponible!" << std::endl;
			SetBoardState(CASE_ACTION_END);
			return;
		}

		int nextIndex = availablePaths[0];

		sf::Vector2f startPos = player.boardPosition;
		sf::Vector2f endPos = m_data->posCase[nextIndex].GetPosition() +
			sf::Vector2f{ randmt::RandomFloat(-10, 10), randmt::RandomFloat(-10, 10) };

		m_data->animator.SetGoTo(startPos, endPos);
		m_data->animator.Restart();

		player.currentCaseIndex = nextIndex;
		player.sprite.SetAnimation("Right_Walk");
		player.sprite.setScale(-1, 1);

		// Vérifier si on arrive sur une convergence
		const MapObject& nextCase = m_data->posCase[nextIndex];
		if (nextCase.GetType() == "merge")
		{
			player.currentPathId = -1; // Retour au chemin principal
		}
	}
	break;

	case WAITING_PATH_CHOICE:
		// Attente de l'entrée du joueur
		break;

	case WIN_DEPLACEMENT:
		SetWinDeplacement(_newIndex);
		break;

	case CASE_ACTION:
		break;
	case CASE_ACTION_END:
		SetBoardState(PLAY, 0);
		break;
	default:
		break;
	}
}

void BaseGame::SetWinDeplacement(int _newIndex)
{
	int winnerIndex = m_gameData->m_winIndex[0];
	int loserIndex = m_gameData->m_winIndex[m_gameData->m_winIndex.size() - 1];

	auto& playerWin = m_data->players[winnerIndex];
	auto& playerLose = m_data->players[loserIndex];


	std::vector<int> availablePaths = GetAvailablePaths(playerWin.currentCaseIndex);
	if (availablePaths.empty())
	{
		std::cout << "Erreur : aucun chemin disponible!" << std::endl;
		SetBoardState(CASE_ACTION_END);
		return;
	}

	std::vector<int> availablePathsLose = GetAvailablePathsBack(playerLose.currentCaseIndex);
	if (availablePaths.empty())
	{
		std::cout << "Erreur : aucun chemin disponible!" << std::endl;
		SetBoardState(CASE_ACTION_END);
		return;
	}

	int nextIndex = availablePaths[0];
	int nextIndexLose = availablePathsLose[0];

	sf::Vector2f startPosWin = playerWin.boardPosition;
	sf::Vector2f endPosWin = m_data->posCase[nextIndex].GetPosition() +
		sf::Vector2f{ randmt::RandomFloat(-10, 10), randmt::RandomFloat(-10, 10) };

	sf::Vector2f startPosLose = playerLose.boardPosition;
	sf::Vector2f endPosLose = m_data->posCase[nextIndexLose].GetPosition() +
		sf::Vector2f{ randmt::RandomFloat(-10, 10), randmt::RandomFloat(-10, 10) };

	m_data->animator.SetGoTo(startPosWin, endPosWin);
	m_data->animator.Restart();
	m_data->animator2.SetGoTo(startPosLose, endPosLose);
	m_data->animator2.Restart();

	playerWin.currentCaseIndex = nextIndex;
	playerWin.sprite.SetAnimation("Right_Walk");
	//player.sprite.setScale(-1, 1);

	playerLose.currentCaseIndex = nextIndexLose;
	playerLose.sprite.SetAnimation("Right_Walk");
	playerLose.sprite.setScale(-1, 1);

	// Vérifier si on arrive sur une convergence
	const MapObject& nextCase = m_data->posCase[nextIndex];
	if (nextCase.GetType() == "merge")
	{
		playerWin.currentPathId = -1; // Retour au chemin principal
	}
}

void BaseGame::BoardStateUpdate(float _dt)
{
	switch (m_data->state)
	{
	case START:
		SortStart();
		break;
	case DEPLACEMENT_SPLIT:
		[[fallthrough]];
	case DEPLACEMENT_BRIGE:
		[[fallthrough]];
	case DEPLACEMENT:
		m_data->players[m_data->currentPlayerIndex].boardPosition = m_data->animator.GetGoTo();
		if (m_data->animator.IsFinished())
		{
			// CORRECTION : Décrémenter le mouvement après avoir atteint la case
			auto& player = m_data->players[m_data->currentPlayerIndex];
			player.pendingMovement--;

			std::cout << "Mouvement restant : " << player.pendingMovement << std::endl;

			if (player.pendingMovement > 0)
			{
				// Continuer le déplacement vers la prochaine case
				SetBoardState(DEPLACEMENT);
			}
			else
			{
				// Déplacement terminé, exécuter l'action de case
				SetBoardState(CASE_ACTION);
			}
		}
		break;

	case DEPLACEMENT_ACTION:
		m_data->players[m_data->currentPlayerIndex].boardPosition = m_data->animator.GetGoTo();
		if (m_data->animator.IsFinished())
		{
			// CORRECTION : Décrémenter le mouvement après avoir atteint la case
			auto& player = m_data->players[m_data->currentPlayerIndex];
			player.pendingMovement--;

			std::cout << "Mouvement restant : " << player.pendingMovement << std::endl;

			if (player.pendingMovement > 0)
			{
				//// Continuer le déplacement vers la prochaine case
				SetBoardState(DEPLACEMENT_ACTION);
			}
			else
			{
				// Déplacement terminé, exécuter l'action de case
				SetBoardState(CASE_ACTION_END);
			}
		}
		break;
	case DEPLACEMENT_ACTION_BACK:
		m_data->players[m_data->currentPlayerIndex].boardPosition = m_data->animator.GetGoTo();
		if (m_data->animator.IsFinished())
		{
			// CORRECTION : Décrémenter le mouvement après avoir atteint la case
			auto& player = m_data->players[m_data->currentPlayerIndex];
			player.pendingMovement--;

			std::cout << "Mouvement restant : " << player.pendingMovement << std::endl;

			if (player.pendingMovement > 0)
			{
				SetBoardState(DEPLACEMENT_ACTION_BACK);
			}
			else
			{
				// Déplacement terminé, exécuter l'action de case
				SetBoardState(CASE_ACTION_END);
			}
		}
		break;

	case CASE_ACTION:
		CaseAction();
		break;

	case BATTLE_ACTION:
		m_gameData->InitMiniGamePlayer();
		for (int i = 0; i < m_data->players.size(); i++)
			m_gameData->AddPlayerPlaying(i);

		SetBoardState(WIN);
		m_data->camera.Reset(m_gameData->m_renderWindow->getDefaultView());
		m_gameData->m_renderWindow->setView(m_data->camera);
		ChangeScene(RandomBattle(), true);
		break;

	case DUEL:
		m_gameData->InitMiniGamePlayer();
		m_gameData->AddPlayerPlaying(m_data->currentPlayerIndex);
		m_gameData->AddPlayerPlaying(OnSameCase());

		SetBoardState(WIN);
		m_data->camera.Reset(m_gameData->m_renderWindow->getDefaultView());
		m_gameData->m_renderWindow->setView(m_data->camera);
		ChangeScene(RandomDuel(), true);
		break;

	case WIN:
		m_data->timeWin -= _dt;
		if (m_data->timeWin <= 0)
		{
			if (!m_gameData->m_winIndex.empty())
			{
				int winnerIndex = m_gameData->m_winIndex[0];
				int loserIndex = m_gameData->m_winIndex[m_gameData->m_winIndex.size() - 1];

				auto& playerWin = m_data->players[winnerIndex];
				auto& playerLose = m_data->players[loserIndex];

				playerWin.pendingMovement = 1;
				playerLose.pendingMovement = 1;

			}

			m_data->timeWin = TIME_WIN_DISPLAY;
			SetBoardState(WIN_DEPLACEMENT);
		}
		break;

	case WIN_DEPLACEMENT:
	{
		if (!m_gameData->m_winIndex.empty())
		{
			int winnerIndex = m_gameData->m_winIndex[0];
			int loserIndex = m_gameData->m_winIndex[m_gameData->m_winIndex.size() - 1];

			m_data->players[loserIndex].sprite.setScale({ -1.f,1.f });

			m_data->players[winnerIndex].boardPosition = m_data->animator.GetGoTo();
			m_data->players[loserIndex].boardPosition = m_data->animator2.GetGoTo();
			if (m_data->animator.IsFinished() && m_data->animator2.IsFinished())
			{
				// CORRECTION : Décrémenter le mouvement après avoir atteint la case
				auto& playerWin = m_data->players[winnerIndex];
				auto& playerLose = m_data->players[loserIndex];
				playerWin.pendingMovement--;
				playerLose.pendingMovement--;

				//std::cout << "Mouvement restant : " << player.pendingMovement << std::endl;

				if (playerWin.pendingMovement > 0 or playerLose.pendingMovement > 0)
				{
					SetBoardState(WIN_DEPLACEMENT);
				}
				else
				{
					// Déplacement terminé, exécuter l'action de case
					m_data->players[winnerIndex].sprite.SetAnimation("Idle");
					m_data->players[loserIndex].sprite.SetAnimation("Idle");
					m_data->players[loserIndex].sprite.setScale({ 1.f,1.f });
					SetBoardState(CASE_ACTION_END);
				}
			}
		}
		else
		{
			SetBoardState(CASE_ACTION_END);
		}
	}
	break;

	case CASE_ACTION_END:
		SetBoardState(PLAY, 0);
		break;

	default:
		break;
	}
}

int BaseGame::OnSameCase()
{
	std::vector< int > sameCasePlayersIndex;
	for (int i = 0; i < m_data->players.size(); i++)
	{
		if (m_data->players[i].currentCaseIndex == m_data->players[m_data->currentPlayerIndex].currentCaseIndex
			&& i != m_data->currentPlayerIndex)
		{
			sameCasePlayersIndex.push_back(i);
		}
	}

	if (!sameCasePlayersIndex.empty())
	{
		int randomIndex = randmt::RandomInt(0, (int)sameCasePlayersIndex.size() - 1);
		return sameCasePlayersIndex[randomIndex];
	}

	return -1;
}

std::string BaseGame::RandomDuel()
{
	// Liste des mini-jeux disponibles
	const std::string miniGames[] = {
		"rockPaperSizor",
		"ArmWrestling",
		"Basket",
		"FlagGame",
		"RandCard",
		"RuRoul"
	};

	// Nombre de mini-jeux disponibles
	const int miniGameCount = 6;

	// Sélection aléatoire d'un mini-jeu
	//int randomIndex = 2;
	int randomIndex = randmt::RandomInt(0, miniGameCount - 1);

	std::cout << "Random minigame selected: " << miniGames[randomIndex] << std::endl;

	return miniGames[randomIndex];
}

void BaseGame::SortStart()
{
	int somme = 1;
	m_data->currentPlayerIndex = 0;

	for (const auto& player : m_data->players)
	{
		if (player.startRandom != 0)
			m_data->currentPlayerIndex = (m_data->currentPlayerIndex + 1) % m_data->players.size();

		somme *= player.startRandom;
	}

	if (somme != 0)
	{
		// Tri des joueurs par ordre décroissant de jet de dé
		// Créer un vecteur d'indices
		std::vector<size_t> indices(m_data->players.size());
		std::iota(indices.begin(), indices.end(), 0);

		// Trier les indices par ordre décroissant de startRandom
		std::sort(indices.begin(), indices.end(),
			[this](size_t a, size_t b) { return m_data->players[a].startRandom > m_data->players[b].startRandom; });

		// Réorganiser m_data->players
		std::vector<Player> sortedPlayers(m_data->players.size());
		for (size_t i = 0; i < indices.size(); ++i) {
			sortedPlayers[i] = m_data->players[indices[i]];
		}
		m_data->players = std::move(sortedPlayers);

		// CORRECTION : Réassigner les textures aux sprites après le déplacement
		for (auto& player : m_data->players) {
			player.sprite.setTexture(player.texture);
		}

		// Réorganiser m_gameData->m_playerDataList
		std::vector<PlayerData> sortedPlayerData(m_gameData->m_playerDataList.size());
		for (size_t i = 0; i < indices.size(); ++i) {
			sortedPlayerData[i] = m_gameData->m_playerDataList[indices[i]];
		}
		m_gameData->m_playerDataList = std::move(sortedPlayerData);

		SetBoardState(PLAY, 0);
	}
}

std::string BaseGame::RandomBattle()
{
	// Liste des mini-jeux disponibles
	const std::string miniGames[] = {
		"Basket",
		"FlagGame",
		"RandCard",
		"RuRoul"
	};

	const int miniGameCount = 4;
	int randomIndex = randmt::RandomInt(0, miniGameCount - 1);

	std::cout << "Random minigame selected: " << miniGames[randomIndex] << std::endl;

	return miniGames[randomIndex];
}

void BaseGame::UpdateCameraToShowAllPlayers()
{
	if (m_data->players.empty())
		return;

	// Calculer la boîte englobante de tous les joueurs
	sf::Vector2f minPos = m_data->players[0].boardPosition;
	sf::Vector2f maxPos = m_data->players[0].boardPosition;

	for (const auto& player : m_data->players)
	{
		minPos.x = std::min(minPos.x, player.boardPosition.x);
		minPos.y = std::min(minPos.y, player.boardPosition.y);
		maxPos.x = std::max(maxPos.x, player.boardPosition.x);
		maxPos.y = std::max(maxPos.y, player.boardPosition.y);
	}

	// Calculer le centre
	sf::Vector2f center = (minPos + maxPos) / 2.0f;

	m_data->camera.SetCenter(center);
	m_data->camera.SetZoom(1.0f);
}

void BaseGame::UpdateCameraFollowPlayer(float _deltaTime)
{
	if (m_data->players.empty() || m_data->state == START)
		return;

	// Position cible : le joueur actuel (sans offset)
	sf::Vector2f targetPos = m_data->players[m_data->currentPlayerIndex].boardPosition;

	// Position actuelle de la caméra
	sf::Vector2f currentPos = m_data->camera.GetCenter();

	// Interpolation fluide (lerp) pour un mouvement doux
	const float smoothSpeed = 3.0f;
	sf::Vector2f newPos;
	newPos.x = currentPos.x + (targetPos.x - currentPos.x) * smoothSpeed * _deltaTime;
	newPos.y = currentPos.y + (targetPos.y - currentPos.y) * smoothSpeed * _deltaTime;

	m_data->camera.SetCenter(newPos);
}

void BaseGame::BonusMalusLuck(bool _malus)
{
	int chance = randmt::RandomInt(1, 100);

	if (_malus)
	{
		Malus(chance);
	}
	else
	{
		Bonus(chance);
	}
}

void BaseGame::Bonus(int _chance)
{
	if (_chance <= 50)
	{
		int rando = randmt::RandomInt(1, 3);
		auto& player = m_data->players[m_data->currentPlayerIndex];

		std::cout << "Avance de : " << rando << "!" << std::endl;

		// Initialiser le mouvement restant pour le déplacement case par case
		player.pendingMovement = rando;

		// Calculer la première case du déplacement
		const int posCaseCount = static_cast<int>(m_data->posCase.size());
		int nextIndex = mathp::ModuloPositiveI(player.currentCaseIndex + 1, posCaseCount);

		SetBoardState(DEPLACEMENT_ACTION, nextIndex);
	}
	else if (_chance <= 80)
	{
		std::cout << "Imunite au Malus" << std::endl;

		m_data->players[m_data->currentPlayerIndex].state = StatePlayer::IMMUN;
		SetBoardState(CASE_ACTION_END);
	}
	else if (_chance <= 100)
	{
		SwapPlayers();
	}
}

void BaseGame::Malus(int _chance)
{
	if (_chance <= 35)
	{
		int rando = randmt::RandomInt(1, 3);
		auto& player = m_data->players[m_data->currentPlayerIndex];

		std::cout << "Recule de : " << rando << "!" << std::endl;

		// Initialiser le mouvement restant pour le déplacement case par case
		player.pendingMovement = rando;

		// Calculer la première case du déplacement (en arrière)
		const int posCaseCount = static_cast<int>(m_data->posCase.size());
		int nextIndex = mathp::ModuloPositiveI(player.currentCaseIndex - 1, posCaseCount);

		// Retourner le sprite pour indiquer le recul
		player.sprite.setScale({ -1.f, 1.f });

		SetBoardState(DEPLACEMENT_ACTION_BACK, nextIndex);
	}
	else if (_chance <= 60)
	{
		if (randmt::Chance(0.5f))
		{
			std::cout << "Passe sont tour" << std::endl;
			m_data->players[m_data->currentPlayerIndex].state = StatePlayer::CANT_PLAY;
			SetBoardState(CASE_ACTION_END);
		}
		else
		{
			std::cout << "Infecter pas de bonus " << std::endl;
			m_data->players[m_data->currentPlayerIndex].state = StatePlayer::INFEC;
			SetBoardState(CASE_ACTION_END);
		}
	}
	else if (_chance <= 70)
	{
		SwapPlayers();
	}
	else if (_chance <= 100)
	{
		if (randmt::Chance(0.75f))
		{
			std::cout << "Confus" << std::endl;
			m_data->players[m_data->currentPlayerIndex].state = StatePlayer::CONFUSED;
		}
		else
		{
			std::cout << "Confus evites" << std::endl;
		}
		SetBoardState(CASE_ACTION_END);
	}
}

bool BaseGame::HasPathChoice(int caseIndex)
{
	const MapObject& currentCase = m_data->posCase[caseIndex];
	if (currentCase.GetPropertyByName("type") == nullptr)
	{
		return false;
	}
	std::string caseType = currentCase.GetPropertyByName("type")->GetStringValue();
	return (caseType == "split");
}

std::vector<int> BaseGame::GetAvailablePaths(int caseIndex)
{
	std::vector<int> paths;
	const MapObject& currentCase = m_data->posCase[caseIndex];
	std::string caseType = "";
	if (currentCase.GetPropertyByName("type") != nullptr)
	{
		caseType = currentCase.GetPropertyByName("type")->GetStringValue();
	}


	if (caseType == "split")
	{
		// Récupérer les deux chemins possibles
		std::string path1Str = currentCase.GetPropertyByName("nextPath1")->GetStringValue();
		std::string path2Str = currentCase.GetPropertyByName("nextPath2")->GetStringValue();

		if (!path1Str.empty())
			paths.push_back(std::stoi(path1Str));
		if (!path2Str.empty())
			paths.push_back(std::stoi(path2Str));
	}
	else if (caseType == "merge")
	{
		// Une seule sortie après convergence
		std::string nextPathStr = currentCase.GetPropertyByName("nextPath")->GetStringValue();
		if (!nextPathStr.empty())
			paths.push_back(std::stoi(nextPathStr));
	}
	else
	{
		// Chemin normal : case suivante
		std::string nextPathStr = currentCase.GetPropertyByName("nextPath")->GetStringValue();
		if (!nextPathStr.empty())
		{
			paths.push_back(std::stoi(nextPathStr));
		}
		else
		{
			// Fallback : index suivant (modulo pour boucler)
			paths.push_back(mathp::ModuloPositiveI(caseIndex + 1, m_data->posCase.size()));
		}
	}

	return paths;
}

std::vector<int> BaseGame::GetAvailablePathsBack(int caseIndex)
{
	std::vector<int> paths;
	const MapObject& currentCase = m_data->posCase[caseIndex];

	// Chemin normal : case suivante
	std::string nextPathStr = currentCase.GetPropertyByName("backPath")->GetStringValue();
	if (!nextPathStr.empty())
	{
		paths.push_back(std::stoi(nextPathStr));
	}
	else
	{
		// Fallback : index suivant (modulo pour boucler)
		paths.push_back(mathp::ModuloPositiveI(caseIndex + 1, m_data->posCase.size()));
	}

	return paths;
}

int BaseGame::GetNextCaseIndex(int currentIndex, int pathChoice)
{
	const MapObject& currentCase = m_data->posCase[currentIndex];

	std::string caseType = "";
	if (currentCase.GetPropertyByName("type") != nullptr)
	{
		caseType = currentCase.GetPropertyByName("type")->GetStringValue();
	}

	if (caseType == "split")
	{
		// Choix entre deux chemins
		if (pathChoice == 0)
		{
			std::string path1Str = currentCase.GetPropertyByName("nextPath1")->GetStringValue();
			return !path1Str.empty() ? std::stoi(path1Str) : -1;
		}
		else
		{
			std::string path2Str = currentCase.GetPropertyByName("nextPath2")->GetStringValue();
			return !path2Str.empty() ? std::stoi(path2Str) : -1;
		}
	}
	else if (caseType == "merge")
	{
		// Une seule sortie
		std::string nextPathStr = currentCase.GetPropertyByName("nextPath")->GetStringValue();
		return !nextPathStr.empty() ? std::stoi(nextPathStr) : -1;
	}
	else
	{
		// Chemin normal
		std::string nextPathStr = currentCase.GetPropertyByName("nextPath")->GetStringValue();
		if (!nextPathStr.empty())
			return std::stoi(nextPathStr);
		else
			return mathp::ModuloPositiveI(currentIndex + 1, m_data->posCase.size());
	}
}

void BaseGame::ProcessPathChoice(int choiceIndex)
{
	auto& player = m_data->players[m_data->currentPlayerIndex];
	const MapObject& currentCase = m_data->posCase[player.currentCaseIndex];

	// Récupérer le prochain index
	int nextIndex = GetNextCaseIndex(player.currentCaseIndex, choiceIndex);

	if (nextIndex == -1)
	{
		std::cout << "Erreur : chemin invalide!" << std::endl;
		SetBoardState(CASE_ACTION_END);
		return;
	}

	// Mettre à jour le pathId si on est sur une bifurcation
	std::string caseType = "";
	if (currentCase.GetPropertyByName("type") != nullptr)
	{
		caseType = currentCase.GetPropertyByName("type")->GetStringValue();
	}

	if (caseType == "split")
	{
		std::string pathIdKey = (choiceIndex == 0) ? "pathId1" : "pathId2";
		if (currentCase.GetPropertyByName(pathIdKey) != nullptr)
		{
			std::string pathIdStr = currentCase.GetPropertyByName(pathIdKey)->GetStringValue();
			player.currentPathId = !pathIdStr.empty() ? std::stoi(pathIdStr) : -1;

			std::cout << "Joueur a choisi le chemin " << player.currentPathId << std::endl;
		}
	}

	std::cout << "Mouvement restant avant animation : " << player.pendingMovement << std::endl;

	m_data->pathChoices.clear();
}

void BaseGame::ProcessBridgeRoll()
{
	int diceRoll = randmt::RandomInt(1, 6);
	auto& player = m_data->players[m_data->currentPlayerIndex];

	std::cout << "Lancer de dé pour le pont : " << diceRoll << std::endl;

	if (diceRoll > 3)
	{
		// Réussite : le joueur peut traverser le pont
		std::cout << "Traversée réussie !" << std::endl;
		player.waitingBridgeRoll = false;

		// CORRECTION : Décrémenter le mouvement AVANT de calculer la prochaine case
		//player.pendingMovement--;

		std::cout << "Mouvement restant après traversée : " << player.pendingMovement << std::endl;

		// Si le joueur a encore du mouvement, continuer
		if (player.pendingMovement > 0)
		{
			SetBoardState(DEPLACEMENT_BRIGE);
		}
		else
		{
			// Plus de mouvement : terminer sur la case du pont
			SetBoardState(CASE_ACTION);
		}
	}
	else
	{
		// Échec : le joueur reste bloqué et perd son tour
		std::cout << "Échec ! Vous ne pouvez pas traverser le pont." << std::endl;
		player.waitingBridgeRoll = false;
		player.pendingMovement = 0;  // Annuler le mouvement restant

		SetBoardState(CASE_ACTION);
	}
}

void BaseGame::SwapPlayers()
{
	int swapIndex = randmt::RandomInt(0, (int)m_data->players.size() - 1);
	while (swapIndex == m_data->currentPlayerIndex)
	{
		swapIndex = randmt::RandomInt(0, (int)m_data->players.size() - 1);
	}

	std::cout << "Swap de place avec : Player " << swapIndex << std::endl;

	auto& player1 = m_data->players[m_data->currentPlayerIndex];
	auto& player2 = m_data->players[swapIndex];

	std::swap(m_data->players[m_data->currentPlayerIndex].currentCaseIndex, m_data->players[swapIndex].currentCaseIndex);
	std::swap(m_data->players[m_data->currentPlayerIndex].boardPosition, m_data->players[swapIndex].boardPosition);

	CreateSmokeEffect(player1);
	CreateSmokeEffect(player2);

	SetBoardState(CASE_ACTION_END);

}

void BaseGame::CreateSmokeEffect(Player& _player)
{
	for (int i = 0; i < 1002; i++)
	{
		//Creer un effet visuel ici 
		if (i % 3 == 0)
		{
			sf::Vector2f posEffect = _player.boardPosition;

			sf::Vector2u bit = _player.texture.GetTexture().getSize();

			posEffect.y -= bit.y * 3 / 12;

			Effect effect(m_data->smokeTp, posEffect + sf::Vector2f(-20 + randmt::RandomInt(0, 40), -20 + randmt::RandomInt(0, 40)), randmt::RandomFloat(1.f, 2.f), 360 * randmt::RandomFloat(0, 360));
			m_data->effects.push_back(effect);
		}
		else if (i % 2 == 0)
		{
			sf::Vector2f posEffect = _player.boardPosition;

			sf::Vector2u bit = _player.texture.GetTexture().getSize();

			posEffect.y -= bit.y * 2 / 12;

			Effect effect(m_data->smokeTp, posEffect + sf::Vector2f(-20 + randmt::RandomInt(0, 40), -20 + randmt::RandomInt(0, 40)), randmt::RandomFloat(1.f, 2.f), 360 * randmt::RandomFloat(0, 360));
			m_data->effects.push_back(effect);
		}
		else
		{
			Effect effect(m_data->smokeTp, _player.boardPosition + sf::Vector2f(-20 + randmt::RandomInt(0, 40), -20 + randmt::RandomInt(0, 40)), randmt::RandomFloat(1.f, 2.f), 360 * randmt::RandomFloat(0, 360));
			m_data->effects.push_back(effect);
		}
	}
}
