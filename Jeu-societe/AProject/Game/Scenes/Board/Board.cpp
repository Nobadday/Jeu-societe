#include "Board.hpp"

void BaseGame::Load(void)
{
	m_data = new SceneData;
	m_gameData = (GameData*)this->m_keptData;

	m_data->tile.InitTiled("Assets/Map/map.json");
	m_data->camera.Reset(m_gameData->m_renderWindow->getDefaultView());

	MapLayer layer = m_data->tile.GetMapLayer("point");
	m_data->posCase = layer.GetObjects();

	m_data->players.resize(m_gameData->m_playerDataList.size());
	m_data->state = START;

	// Initialisation des joueurs
	for (size_t i = 0; i < m_data->players.size(); i++)
	{
		m_data->players[i].texture.loadFromFile("Assets/Images/Placeholder.png");
		m_data->players[i].sprite.setTexture(m_data->players[i].texture);

		//sf::FloatRect spriteBounds = m_data->players[i].sprite.getLocalBounds();

		//m_data->players[i].sprite.setOrigin(spriteBounds.width / 2.0f, spriteBounds.height / 2.0f);

		m_data->players[i].boardPosition = m_data->posCase[0].GetPosition();
		m_data->players[i].currentCaseIndex = 0;
		m_data->players[i].startRandom = 0;
	}

	m_data->currentPlayerIndex = 0;

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
	delete this->m_data;
	this->m_data = NULL;
}

void BaseGame::PollEvent(sf::Event& _event)
{
	// Gestion des entrées joystick
	if (_event.type == sf::Event::JoystickButtonPressed)
	{
		if (m_gameData->m_playerDataList[m_data->currentPlayerIndex].m_joystickId == _event.joystickButton.joystickId)
		{
			if (_event.joystickButton.button == 0 && m_data->animator.IsFinished())
			{
				int rando = random::RandomInt(1, 6);
				std::cout << "Roll Dice: " << rando << std::endl;
				std::cout << "Playe : " << m_data->players[m_data->currentPlayerIndex].startRandom << std::endl;

				if (m_data->state != START and m_data->state != WIN_DEPLACEMENT)
				{
					int newIndex = (m_data->players[m_data->currentPlayerIndex].currentCaseIndex + rando) % m_data->posCase.size();
					SetBoardState(DEPLACEMENT, newIndex);
				}
				else
				{
					m_data->players[m_data->currentPlayerIndex].startRandom = rando;
					std::cout << "Place: " << rando << std::endl;
				}
			}
		}
	}

	// Gestion des entrées clavier (DEBUG)
	if (_event.type == sf::Event::KeyPressed)
	{
		if (_event.key.code == sf::Keyboard::Space && m_data->animator.IsFinished())
		{
			int rando = random::RandomInt(1, 6);
			std::cout << "Roll Dice: " << rando << std::endl;
			std::cout << "Playe : " << m_data->players[m_data->currentPlayerIndex].startRandom << std::endl;

			if (m_data->state != START and m_data->state != WIN_DEPLACEMENT)
			{
				int newIndex = (m_data->players[m_data->currentPlayerIndex].currentCaseIndex + rando) % m_data->posCase.size();
				SetBoardState(DEPLACEMENT, newIndex);
			}
			else
			{
				m_data->players[m_data->currentPlayerIndex].startRandom = rando;
			}
		}
	}
}

void BaseGame::Update(float _deltaTime)
{
	// Mise à jour des animations
	m_data->animator.Update(_deltaTime);
	m_data->animator2.Update(_deltaTime);

	// Mise à jour de la logique du plateau
	BoardStateUpdate();

	// Mise à jour de la caméra pour suivre le joueur actif
	UpdateCameraFollowPlayer(_deltaTime);
}

void BaseGame::Draw(sf::RenderWindow& _renderWindow)
{
	const sf::View& referenceView = m_data->camera.GetView();
	_renderWindow.setView(referenceView);
	m_data->tile.DrawMapLayers(_renderWindow, referenceView.getCenter());

	// Affichage des joueurs
	for (auto& player : m_data->players)
	{
		// Utiliser directement la position du board sans conversion
		player.sprite.setPosition(player.boardPosition);
		_renderWindow.draw(player.sprite);
	}
}

void BaseGame::CaseAction()
{
	const std::string& caseType = m_data->posCase[m_data->players[m_data->currentPlayerIndex].currentCaseIndex].GetType();

	if (caseType == "Bonus")
	{
		std::cout << "Landed on a Bonus case!" << std::endl;
		int rando = random::RandomInt(1, 3);
		int newIndex = (m_data->players[m_data->currentPlayerIndex].currentCaseIndex + rando) % m_data->posCase.size();
		SetBoardState(DEPLACEMENT_ACTION, newIndex);
	}
	else if (caseType == "Malus")
	{
		std::cout << "Landed on a Malus case!" << std::endl;
		int rando = random::RandomInt(1, 3);
		int newIndex = (m_data->players[m_data->currentPlayerIndex].currentCaseIndex - rando) % m_data->posCase.size();
		SetBoardState(DEPLACEMENT_ACTION, newIndex);
	}
	else if (caseType == "Luck")
	{
		std::cout << "Landed on a Luck case!" << std::endl;
		int rando = random::RandomInt(1, 3);

		if (random::RandomInt(0, 2) == 0)
			rando = -rando;

		int newIndex = (m_data->players[m_data->currentPlayerIndex].currentCaseIndex + rando) % m_data->posCase.size();
		SetBoardState(DEPLACEMENT_ACTION, newIndex);
	}
	else if (caseType == "Battle")
	{
		std::cout << "Landed on a Battle case!" << std::endl;
		SetBoardState(BATTLE_ACTION, 0);
	}
	else
	{
		int sameCase = OnSameCase();
		if (sameCase != -1)
		{
			SetBoardState(DUEL, 0);
		}
		else
		{
			SetBoardState(PLAY, 0);
			m_data->currentPlayerIndex = (m_data->currentPlayerIndex + 1) % m_data->players.size();
		}
	}
}

void BaseGame::SetBoardState(State _state, int _newIndex)
{
	m_data->state = _state;

	switch (m_data->state)
	{
	case START:
		break;

	case DEPLACEMENT:
		[[fallthrough]];
	case DEPLACEMENT_ACTION:
	{
		sf::Vector2f startPos = m_data->players[m_data->currentPlayerIndex].boardPosition;
		sf::Vector2f endPos = m_data->posCase[_newIndex].GetPosition();

		m_data->animator.SetGoTo(startPos, endPos);
		m_data->animator.Restart();

		m_data->players[m_data->currentPlayerIndex].currentCaseIndex = _newIndex;
	}
	break;

	case WIN:
	{
		int winnerIndex = m_gameData->m_winIndex[0];
		int loserIndex = m_gameData->m_winIndex[m_gameData->m_winIndex.size() - 1];

		m_data->players[winnerIndex].currentCaseIndex += _newIndex;
		m_data->players[loserIndex].currentCaseIndex -= _newIndex;

		// Animation du gagnant
		sf::Vector2f startPos = m_data->players[winnerIndex].boardPosition;
		sf::Vector2f endPos = m_data->posCase[m_data->players[winnerIndex].currentCaseIndex].GetPosition();
		m_data->animator.SetGoTo(startPos, endPos);
		m_data->animator.Restart();

		// Animation du perdant
		startPos = m_data->players[loserIndex].boardPosition;
		endPos = m_data->posCase[m_data->players[loserIndex].currentCaseIndex].GetPosition();
		m_data->animator2.SetGoTo(startPos, endPos);
		m_data->animator2.Restart();
	}
	break;

	case CASE_ACTION:
	case CASE_ACTION_END:
	default:
		break;
	}
}

void BaseGame::BoardStateUpdate()
{
	switch (m_data->state)
	{
	case START:
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

			m_data->state = PLAY;
		}
	}
	break;

	case DEPLACEMENT:
		m_data->players[m_data->currentPlayerIndex].boardPosition = m_data->animator.GetGoTo();
		if (m_data->animator.IsFinished())
			m_data->state = CASE_ACTION;
		break;

	case DEPLACEMENT_ACTION:
		m_data->players[m_data->currentPlayerIndex].boardPosition = m_data->animator.GetGoTo();
		if (m_data->animator.IsFinished())
			m_data->state = CASE_ACTION_END;
		break;

	case CASE_ACTION:
		CaseAction();
		break;

	case BATTLE_ACTION:
		m_gameData->InitMiniGamePlayer();
		for (size_t i = 0; i < m_data->players.size(); i++)
			m_gameData->AddPlayerPlaying(i);

		m_data->state = WIN;
		m_data->camera.Reset(m_gameData->m_renderWindow->getDefaultView());
		ChangeScene(RandomDuel(), true);
		break;

	case DUEL:
		m_gameData->InitMiniGamePlayer();
		m_gameData->AddPlayerPlaying(m_data->currentPlayerIndex);
		m_gameData->AddPlayerPlaying(OnSameCase());

		m_data->state = WIN;
		m_data->camera.Reset(m_gameData->m_renderWindow->getDefaultView());
		ChangeScene(RandomDuel(), true);
		break;

	case WIN:
		SetBoardState(WIN_DEPLACEMENT, 1);
		break;

	case WIN_DEPLACEMENT:
	{
		int winnerIndex = m_gameData->m_winIndex[0];
		int loserIndex = m_gameData->m_winIndex[m_gameData->m_winIndex.size() - 1];

		m_data->players[winnerIndex].boardPosition = m_data->animator.GetGoTo();
		m_data->players[loserIndex].boardPosition = m_data->animator2.GetGoTo();

		if (m_data->animator.IsFinished() && m_data->animator2.IsFinished())
			m_data->state = CASE_ACTION_END;
	}
	break;

	case CASE_ACTION_END:
		m_data->state = PLAY;
		m_data->currentPlayerIndex = (m_data->currentPlayerIndex + 1) % m_data->players.size();
		break;

	default:
		break;
	}
}

int BaseGame::OnSameCase()
{
	for (size_t i = 0; i < m_data->players.size(); i++)
	{
		if (m_data->players[i].currentCaseIndex == m_data->players[m_data->currentPlayerIndex].currentCaseIndex
			&& i != m_data->currentPlayerIndex)
		{
			return i;
		}
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
		"FlagGame"
	};

	// Nombre de mini-jeux disponibles
	const int miniGameCount = 4;

	// Sélection aléatoire d'un mini-jeu
	int randomIndex = random::RandomInt(0, miniGameCount - 1);

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
