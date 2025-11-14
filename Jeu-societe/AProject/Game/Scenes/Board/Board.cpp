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
		m_data->players[i].texture.LoadFromFile("Assets/Sprites/Anim_final.anim", TextureAnimated::ANIMATION_ANIM);

		m_data->players[i].sprite.setTexture(m_data->players[i].texture);

		m_data->players[i].sprite.setOrigin({ 0.5f,1.f });

		m_data->players[i].boardPosition = m_data->posCase[0].GetPosition();
		m_data->players[i].currentCaseIndex = 0;
		m_data->players[i].startRandom = 0;
		m_data->players[i].state = StatePlayer::NONE;
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
	// Lambda pour factoriser le comportement du lancer de dé
	auto processDiceRoll = [this](int rando)
		{
			std::cout << "Roll Dice: " << rando << std::endl;
			std::cout << "Player startRandom: " << m_data->players[m_data->currentPlayerIndex].startRandom << std::endl;

			const int posCaseCount = static_cast<int>(m_data->posCase.size());

			if (m_data->state != START)
			{
				auto& player = m_data->players[m_data->currentPlayerIndex];
				int newIndex = 0;

				if (player.state != StatePlayer::CONFUSED)
				{
					newIndex = mathp::ModuloPositiveI(player.currentCaseIndex + rando, posCaseCount);
				}
				else
				{
					newIndex = mathp::ModuloPositiveI(player.currentCaseIndex - rando, posCaseCount);
				}

				SetBoardState(DEPLACEMENT, newIndex);
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

		// Mise à jour de la caméra pour suivre le joueur actif
		UpdateCameraFollowPlayer(_deltaTime);

		if (m_data->players[m_data->currentPlayerIndex].state != NONE)
		{
			m_data->players[m_data->currentPlayerIndex].tourstate += 1;
			if (m_data->players[m_data->currentPlayerIndex].tourstate >= MAX_TOUR_EFFECT)
			{
				m_data->players[m_data->currentPlayerIndex].tourstate = 0;
				m_data->players[m_data->currentPlayerIndex].state = StatePlayer::NONE;
			}
		}
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
	m_data->tile.DrawMapLayers(_renderWindow, referenceView.getCenter());

	// Affichage des joueurs
	for (auto& player : m_data->players)
	{
		player.sprite.setPosition(player.boardPosition);
		_renderWindow.draw(player.sprite);
	}
}

void BaseGame::CaseAction()
{
	const std::string& caseType = m_data->posCase[m_data->players[m_data->currentPlayerIndex].currentCaseIndex].GetType();

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
		SetBoardState(BATTLE_ACTION, 0);
		break;

	default:
	{
		/*int sameCase = OnSameCase();
		if (sameCase != -1)
		{
			SetBoardState(DUEL, 0);
		}
		else
		{*/
			SetBoardState(PLAY, 0);
		/*}*/
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
		m_data->players[m_data->currentPlayerIndex].sprite.SetAnimation("Idle");
		m_data->currentPlayerIndex = (m_data->currentPlayerIndex + 1) % m_data->players.size();
		break;
	case WIN:
		m_data->players[m_data->currentPlayerIndex].sprite.SetAnimation("Idle");
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
		m_data->players[m_data->currentPlayerIndex].sprite.SetAnimation("Right_Walk");
	}
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
	std::cout << "Setting WIN_DEPLACEMENT state." << std::endl;

	int winnerIndex = m_gameData->m_winIndex[0];
	int loserIndex = m_gameData->m_winIndex[m_gameData->m_winIndex.size() - 1];

	std::cout << "player winner CurrentCaseIndex before move: " << m_data->players[winnerIndex].currentCaseIndex << std::endl;
	std::cout << "player loser CurrentCaseIndex before move: " << m_data->players[loserIndex].currentCaseIndex << std::endl;

	m_data->players[winnerIndex].currentCaseIndex += _newIndex;
	std::cout << "player winner CurrentCaseIndex after move: " << m_data->players[winnerIndex].currentCaseIndex << std::endl;

	m_data->players[loserIndex].currentCaseIndex -= _newIndex;
	std::cout << "player loser CurrentCaseIndex after move: " << m_data->players[loserIndex].currentCaseIndex << std::endl;

	m_data->players[winnerIndex].currentCaseIndex %= m_data->posCase.size();
	m_data->players[loserIndex].currentCaseIndex %= m_data->posCase.size();

	// Animation du gagnant
	sf::Vector2f startPos = m_data->players[winnerIndex].boardPosition;
	sf::Vector2f endPos = m_data->posCase[m_data->players[winnerIndex].currentCaseIndex].GetPosition();
	m_data->animator.SetGoTo(startPos, endPos);
	m_data->animator.Restart();
	m_data->players[winnerIndex].sprite.SetAnimation("Right_Walk");

	// Animation du perdant
	startPos = m_data->players[loserIndex].boardPosition;
	endPos = m_data->posCase[m_data->players[loserIndex].currentCaseIndex].GetPosition();
	m_data->animator2.SetGoTo(startPos, endPos);
	m_data->animator2.Restart();
	m_data->players[loserIndex].sprite.SetAnimation("Right_Walk");
}

void BaseGame::BoardStateUpdate(float _dt)
{
	switch (m_data->state)
	{
	case START:
		SortStart();
		break;
	case DEPLACEMENT:
		m_data->players[m_data->currentPlayerIndex].boardPosition = m_data->animator.GetGoTo();
		if (m_data->animator.IsFinished())
			SetBoardState(CASE_ACTION);
		break;

	case DEPLACEMENT_ACTION:
		m_data->players[m_data->currentPlayerIndex].boardPosition = m_data->animator.GetGoTo();
		if (m_data->animator.IsFinished())
			SetBoardState(CASE_ACTION_END);
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
		m_data->timeWin -= _dt; // Approximation pour 60 FPS
		if (m_data->timeWin <= 0)
		{
			m_data->timeWin = TIME_WIN_DISPLAY;
			SetBoardState(WIN_DEPLACEMENT, 1);
		}
		break;

	case WIN_DEPLACEMENT:
	{
		if (!m_gameData->m_winIndex.empty())
		{
			int winnerIndex = m_gameData->m_winIndex[0];
			int loserIndex = m_gameData->m_winIndex[m_gameData->m_winIndex.size() - 1];

			m_data->players[winnerIndex].boardPosition = m_data->animator.GetGoTo();
			m_data->players[loserIndex].boardPosition = m_data->animator2.GetGoTo();

			if (m_data->animator.IsFinished() && m_data->animator2.IsFinished())
			{
				m_data->players[winnerIndex].sprite.SetAnimation("Idle");
				m_data->players[loserIndex].sprite.SetAnimation("Idle");
				SetBoardState(CASE_ACTION_END);
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
		int randomIndex = random::RandomInt(0, (int)sameCasePlayersIndex.size() - 1);
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
		int currentIndex = m_data->players[m_data->currentPlayerIndex].currentCaseIndex;
		int newIndex;

		std::cout << "Avance de : " << rando << "!" << std::endl;

		newIndex = (currentIndex + rando) % m_data->posCase.size();

		SetBoardState(DEPLACEMENT_ACTION, newIndex);
	}
	else if (_chance <= 80)
	{
		std::cout << "Imunite au Malus" << std::endl;

		m_data->players[m_data->currentPlayerIndex].state = StatePlayer::IMMUN;
		SetBoardState(CASE_ACTION_END);
	}
	else if (_chance <= 100)
	{
		int swapIndex = randmt::RandomInt(0, (int)m_data->players.size() - 1);
		while (swapIndex == m_data->currentPlayerIndex)
		{
			swapIndex = randmt::RandomInt(0, (int)m_data->players.size() - 1);
		}

		std::cout << "Swap de place avec : Player " << swapIndex << std::endl;

		std::swap(m_data->players[m_data->currentPlayerIndex].currentCaseIndex, m_data->players[swapIndex].currentCaseIndex);
		std::swap(m_data->players[m_data->currentPlayerIndex].boardPosition, m_data->players[swapIndex].boardPosition);

		SetBoardState(CASE_ACTION_END);
	}
}

void BaseGame::Malus(int _chance)
{
	if (_chance <= 35)
	{
		int rando = randmt::RandomInt(1, 3);
		int currentIndex = m_data->players[m_data->currentPlayerIndex].currentCaseIndex;
		int newIndex;

		std::cout << "Recule de : " << rando << "!" << std::endl;

		newIndex = mathp::ModuloPositiveI((currentIndex - rando), m_data->posCase.size());

		SetBoardState(DEPLACEMENT_ACTION, newIndex);
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
			std::cout << "Infecter les prochaine bonus (2tours) " << std::endl;
			m_data->players[m_data->currentPlayerIndex].state = StatePlayer::INFEC;
			SetBoardState(CASE_ACTION_END);
		}
	}
	else if (_chance <= 70)
	{
		int swapIndex = randmt::RandomInt(0, (int)m_data->players.size() - 1);
		while (swapIndex == m_data->currentPlayerIndex)
		{
			swapIndex = randmt::RandomInt(0, (int)m_data->players.size() - 1);
		}

		std::cout << "Swap de place avec : Player " << swapIndex << std::endl;

		std::swap(m_data->players[m_data->currentPlayerIndex].currentCaseIndex, m_data->players[swapIndex].currentCaseIndex);
		std::swap(m_data->players[m_data->currentPlayerIndex].boardPosition, m_data->players[swapIndex].boardPosition);

		SetBoardState(CASE_ACTION_END);
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
