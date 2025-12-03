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

	m_data->HudLBM.text.setFont(*m_gameData->m_assetManager->GetAsset<sf::Font>("BoardFont", AssetManager::AssetType::FONT));
	m_data->HudLBM.sprite.setTexture(*m_gameData->m_assetManager->GetAsset<TextureAnimated>("Anim_card", AssetManager::AssetType::TEXTURE_ANIMATED));
	m_data->HudLBM.state = NONELBM;
	m_data->HudLBM.active = false;

	m_data->tile.InitTiled("Assets/Map/map.json");
	m_data->camera.Reset(m_gameData->m_renderWindow->getDefaultView());

	MapLayer layer = m_data->tile.GetMapLayer("point");
	m_data->posCase = layer.GetObjects();

	m_data->players.resize(m_gameData->m_playerDataList.size());
	m_data->state = START;

	m_data->icone.setTexture(*m_gameData->m_assetManager->GetAsset<TextureAnimated>("Icone", AssetManager::AssetType::TEXTURE_ANIMATED));
	m_data->icone.setOrigin({ 0.5f,1 });

	m_data->timeWin = TIME_WIN_DISPLAY;
	m_data->timeLBM = TIME_LBM_DISPLAY;
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
		default:
			m_data->players[i].texture = *m_gameData->m_assetManager->GetAsset<TextureAnimated>("Perso1-1", AssetManager::AssetType::TEXTURE_ANIMATED);
			break;
		}

		m_data->players[i].sprite.setTexture(m_data->players[i].texture);

		m_data->players[i].sprite.setOrigin({ 0.5f,1.f });

		m_data->players[i].v.setFont(*m_gameData->m_assetManager->GetAsset<sf::Font>("BoardFont", AssetManager::AssetType::FONT));
		m_data->players[i].v.setString(L"⌂");


		m_data->players[i].posIcone = PosIcone(i);

		sf::FloatRect textRect = m_data->players[i].v.getLocalBounds();

		m_data->players[i].v.setOrigin({ textRect.width / 2, textRect.height / 2 });

		m_data->players[i].v.setPosition(m_data->players[i].boardPosition + sf::Vector2f{ 0.f,-100.f });

		m_data->players[i].playeur.setFont(*m_gameData->m_assetManager->GetAsset<sf::Font>("BoardFont", AssetManager::AssetType::FONT));
		m_data->players[i].playeur.setString("P" + std::to_string(i + 1));

		textRect = m_data->players[i].playeur.getLocalBounds();

		m_data->players[i].playeur.setOrigin({ textRect.width / 2, textRect.height / 2 });

		m_data->players[i].playeur.setPosition(m_data->players[i].boardPosition + sf::Vector2f{ 0.f,-120.f });


		m_data->players[i].boardPosition = m_data->posCase[0].GetPosition() + sf::Vector2f{ -40.f * i ,0.f };
		m_data->players[i].currentCaseIndex = 0;
		m_data->players[i].startRandom = 0;
		m_data->players[i].state = StatePlayer::NONE;
		m_data->players[i].pendingMovement = 0;      // Nouveau
		m_data->players[i].currentPathId = -1;       // Nouveau : -1 = chemin principal
	}

	m_data->currentPlayerIndex = 0;

	m_data->smokeOff = false;

	m_data->smoke.loadFromFile("Assets/Sprites/Board/smoke-export.png");

	// Position initiale de la caméra : afficher tous les joueurs
	UpdateCameraToShowAllPlayers();

	sf::Vector2f posMin;
	sf::Vector2f posMax;

	for (int i = 0; i < m_data->posCase.size(); i++)
	{
		auto& mapObject = m_data->posCase[i];

		if (mapObject.GetName() == "10")
		{
			posMin = mapObject.GetPosition();
		}

		if (i == m_data->posCase.size() - 1)
		{
			posMax = mapObject.GetPosition();
		}
	}

	CreateSmokeEffectAnotherPart(posMin, posMax);

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
			int swapIndex = randmt::RandomInt(0, (int)m_data->players.size() - 1);
			while (swapIndex == m_data->currentPlayerIndex)
			{
				swapIndex = randmt::RandomInt(0, (int)m_data->players.size() - 1);
			}

			SwapPlayers(swapIndex);
		}
	}

	if (m_data->state == WAITING_FIN_ROLL)
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
			ProcessFinRoll();
		}

		return;
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
					if (player.tourstate > MAX_TOUR_EFFECT)
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
					SetBoardState(DEPLACEMENT_BACK, nextIndex);
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
		if (m_gameData->m_playerDataList[m_data->currentPlayerIndex].m_joystickId == _event.joystickButton.joystickId && m_data->state != WIN_DEPLACEMENT && m_data->state != WIN && m_data->state != STATE)
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
		if (_event.key.code == sf::Keyboard::Space && m_data->animator.IsFinished() && m_data->state != WIN_DEPLACEMENT && m_data->state != WIN && m_data->state != STATE)
		{
			int rando = randmt::RandomInt(1, 6);
			processDiceRoll(rando);
		}
	}

}

void BaseGame::Update(float _deltaTime)
{
	// Mise à jour des animations
	UpdateLBM(_deltaTime);
	if (m_data->players[m_data->currentPlayerIndex].state != CANT_PLAY)
	{
		m_data->animator.Update(_deltaTime);
		m_data->animator2.Update(_deltaTime);

		// Mise à jour de la logique du plateau
		BoardStateUpdate(_deltaTime);


		for (int i = m_data->effectSwap.size() - 1; i >= 0; i--)
		{
			auto& effect = m_data->effectSwap[i];

			effect.Update(_deltaTime);

			if (!effect.IsActive())
			{
				effect = m_data->effectSwap.back();
				m_data->effectSwap.pop_back();
			}
		}

		for (int i = m_data->effectsMap.size() - 1; i >= 0; i--)
		{
			auto& effect = m_data->effectsMap[i];

			if (!m_data->smokeOff)
			{
				effect.UpdateSpecial(_deltaTime);
			}
			else
			{
				effect.Update(_deltaTime);

				if (!effect.IsActive())
				{
					effect = m_data->effectsMap.back();
					m_data->effectsMap.pop_back();
				}
			}

		}


		// Mise à jour de la caméra pour suivre le joueur actif
		for (auto& player : m_data->players)
		{
			player.v.setPosition(player.boardPosition + sf::Vector2f{ 0.f,-250.f });
			player.playeur.setPosition(player.boardPosition + sf::Vector2f{ 0.f,-275.f });
		}


		UpdateCameraFollowPlayer(_deltaTime);

	}
	else
	{
		if (m_data->HudLBM.state == NONELBM)
		{
			m_data->players[m_data->currentPlayerIndex].tourstate = 0;
			m_data->players[m_data->currentPlayerIndex].state = StatePlayer::NONE;
			SetBoardState(PLAY, 0);
		}
	}
}

void BaseGame::Draw(sf::RenderWindow& _renderWindow)
{
	const sf::View& referenceView = m_data->camera.GetView();
	_renderWindow.setView(referenceView);

	std::string layer = "point";

	m_data->tile.DrawMapLayers(_renderWindow, referenceView.getCenter(), layer);

	// Créer un vecteur d'indices pour trier les joueurs par position Y
	std::vector<int> indices(m_data->players.size());
	std::iota(indices.begin(), indices.end(), 0);

	// Trier les indices par position Y croissante
	std::sort(indices.begin(), indices.end(),
		[this](int a, int b) {
			return m_data->players[a].boardPosition.y < m_data->players[b].boardPosition.y;
		});

	// Affichage des joueurs dans l'ordre trié
	for (int idx : indices)
	{
		m_data->players[idx].sprite.setPosition(m_data->players[idx].boardPosition);
		_renderWindow.draw(m_data->players[idx].sprite);
		if (idx == m_data->currentPlayerIndex)
		{
			m_data->players[idx].v.setFillColor(sf::Color::Cyan);
			m_data->players[idx].playeur.setFillColor(sf::Color::Cyan);
		}
		if (idx != m_data->currentPlayerIndex)
		{
			m_data->players[idx].v.setFillColor(sf::Color::White);
			m_data->players[idx].playeur.setFillColor(sf::Color::White);
		}
		_renderWindow.draw(m_data->players[idx].v);
		_renderWindow.draw(m_data->players[idx].playeur);
	}

	m_data->tile.DrawMapLayers(_renderWindow, referenceView.getCenter(), "point");

	for (auto& effect : m_data->effectSwap)
	{
		effect.Draw(_renderWindow);
	}

	for (auto& effect : m_data->effectsMap)
	{
		effect.Draw(_renderWindow);
	}

	DrawLBM(_renderWindow);

	for (int idx : indices)
	{
		DrawIconePlayer(_renderWindow, idx);
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

	if (caseType == "Bonus" and m_data->players[m_data->currentPlayerIndex].state == StatePlayer::INFEC)
	{
		if (m_data->HudLBM.state == NONELBM)
		{
			SetBoardState(PLAY);
			return;
		}
	}
	if (caseType == "Malus" and m_data->players[m_data->currentPlayerIndex].state == StatePlayer::IMMUN)
	{
		if (m_data->HudLBM.state == NONELBM)
		{
			SetBoardState(PLAY);
			return;
		}
	}

	sf::Vector2f size2 = m_gameData->m_renderWindow->getView().getCenter();


	switch (hash(caseType.c_str()))
	{
	case hash("Bonus"):
	{


		std::cout << "Landed on a Bonus case!" << std::endl;
		m_data->HudLBM.name = "Bonus";
		m_data->HudLBM.state = BONUS;
		m_data->HudLBM.sprite.SetAnimation(m_data->HudLBM.name);
		sf::Vector2u size = m_data->HudLBM.sprite.getTexture()->getSize();

		m_data->HudLBM.sprite.setScale({ 0.5f , 0.5f });
		m_data->HudLBM.sprite.setOrigin({ 0.5,0.5 });
		m_data->HudLBM.sprite.setPosition({ size2.x   ,  size2.y });
		m_data->timeLBM = TIME_LBM_DISPLAY;
		BonusMalusLuck(false);
	}


	break;

	case hash("Malus"):
	{


		std::cout << "Landed on a Malus case!" << std::endl;


		m_data->HudLBM.name = "Malus";
		m_data->HudLBM.state = MALUS;
		m_data->HudLBM.sprite.SetAnimation(m_data->HudLBM.name);

		sf::Vector2u size = m_data->HudLBM.sprite.getTexture()->getSize();

		m_data->HudLBM.sprite.setScale({ 0.5f , 0.5f });
		m_data->HudLBM.sprite.setOrigin({ 0.5,0.5 });
		m_data->HudLBM.sprite.setPosition({ size2.x   ,  size2.y });
		m_data->timeLBM = TIME_LBM_DISPLAY;

		BonusMalusLuck(true);

		
	}
	break;

	case hash("Luck"):
	{
		std::cout << "Landed on a Luck case!" << std::endl;

		m_data->HudLBM.name = "Luck";
		m_data->HudLBM.state = LUCKY;
		m_data->HudLBM.sprite.SetAnimation(m_data->HudLBM.name);
		sf::Vector2u size = m_data->HudLBM.sprite.getTexture()->getSize();

		m_data->HudLBM.sprite.setScale({ 0.5f , 0.5f });
		m_data->HudLBM.sprite.setOrigin({ 0.5,0.5 });
		m_data->HudLBM.sprite.setPosition({ size2.x , size2.y });
		m_data->timeLBM = TIME_LBM_DISPLAY;

		BonusMalusLuck(randmt::Chance(0.5f));

	}
	break;
	case hash("Battle"):
		std::cout << "Landed on a Battle case!" << std::endl;
		SetBoardState(BATTLE_ACTION);
		break;

	default:
	{
		if (m_data->HudLBM.state == NONELBM)
		{
			SetBoardState(PLAY);
		}
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

		if (caseType == "end" && player.pendingMovement > 0)
		{
			player.sprite.SetAnimation("Idle");
			std::cout << "Ligne d'arrivée détectée ! Lancez le dé pour franchir..." << std::endl;
			player.waitingBridgeRoll = true;
			SetBoardState(WAITING_FIN_ROLL);
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

		if (m_data->players[m_data->currentPlayerIndex].currentPathId != -1 && availablePaths.size() > 1)
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
	case DEPLACEMENT_BACK:
		[[fallthrough]];
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
	case DEPLACEMENT_ACTION_2:
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

		sf::Vector2f startPos = player.boardPosition;
		sf::Vector2f endPos = m_data->posCase[nextIndex].GetPosition() +
			sf::Vector2f{ randmt::RandomFloat(-10, 10), randmt::RandomFloat(-10, 10) };

		m_data->animator.SetGoTo(startPos, endPos);
		m_data->animator.Restart();

		player.currentCaseIndex = nextIndex;
		player.sprite.SetAnimation("Right_Walk");
		//player.sprite.setScale(-1, 1);

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
	if (!m_gameData->m_winIndex.empty())
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
	else
	{
		SetBoardState(CASE_ACTION_END);
	}
}

void BaseGame::BoardStateUpdate(float _dt)
{
	switch (m_data->state)
	{
	case STATE:

		/*std::cout << m_data->HudLBM.name << std::endl;
		std::cout << m_data->HudLBM.chosse << std::endl;
		std::cout << m_data->HudLBM.state << std::endl;
		std::cout << m_data->HudLBM.active << std::endl;
		std::cout << m_data->HudLBM.swap << std::endl;
		std::cout << m_data->HudLBM.rando << std::endl;
		std::cout << m_data->timeLBM << std::endl;

		std::cout << std::endl;*/
		break;
	case START:
		SortStart();
		break;
	case DEPLACEMENT_SPLIT:
		[[fallthrough]];
	case DEPLACEMENT_BRIGE:
		[[fallthrough]];
	case DEPLACEMENT:
		m_data->players[m_data->currentPlayerIndex].sprite.Update(_dt);
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
	case DEPLACEMENT_BACK:
		m_data->players[m_data->currentPlayerIndex].sprite.Update(_dt);
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
				SetBoardState(DEPLACEMENT_BACK);
			}
			else
			{
				// Déplacement terminé, exécuter l'action de case
				SetBoardState(CASE_ACTION);
			}
		}
		break;

	case DEPLACEMENT_ACTION:
		m_data->players[m_data->currentPlayerIndex].sprite.Update(_dt);
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
		m_data->players[m_data->currentPlayerIndex].sprite.Update(_dt);
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
	case DEPLACEMENT_ACTION_2:
		m_data->players[m_data->currentPlayerIndex].sprite.Update(_dt);
		m_data->players[m_data->currentPlayerIndex].boardPosition = m_data->animator.GetGoTo();
		if (m_data->animator.IsFinished())
		{
			// CORRECTION : Décrémenter le mouvement après avoir atteint la case
			auto& player = m_data->players[m_data->currentPlayerIndex];
			player.pendingMovement--;

			std::cout << "Mouvement restant : " << player.pendingMovement << std::endl;

			if (player.pendingMovement > 0)
			{
				SetBoardState(DEPLACEMENT_ACTION_2);
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

			m_data->players[winnerIndex].sprite.Update(_dt);
			m_data->players[loserIndex].sprite.Update(_dt);

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
		if (m_data->HudLBM.state == NONELBM)
		{
			SetBoardState(PLAY, 0);
		}
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

	return "FlagGame";
	//return miniGames[randomIndex];
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

		for (size_t i = 0; i < m_data->players.size(); i++)
		{
			std::cout << "Player : " << m_data->players[i].startRandom << " ";
			std::cout << " id manette : " << m_gameData->m_playerDataList[i].m_joystickId << " " << std::endl;
		}

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


		for (size_t i = 0; i < m_data->players.size(); i++)
		{
			std::cout << "Player : " << m_data->players[i].startRandom << " ";
			std::cout << " id manette : " << m_gameData->m_playerDataList[i].m_joystickId << " " << std::endl;
		}


		m_data->state = PLAY;
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

	return "FlagGame" ;
	//return miniGames[randomIndex];
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
	int chance = randmt::RandomInt(0, 100);

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
		m_data->HudLBM.chosse = "CasePlus";
		/*std::cout << m_data->HudLBM.chosse << std::endl;
		std::cout << m_data->HudLBM.name << std::endl;
		std::cout << m_data->HudLBM.state << std::endl;
		std::cout << m_data->HudLBM.active << std::endl;
		std::cout << m_data->HudLBM.swap << std::endl;
		std::cout << m_data->HudLBM.rando << std::endl;*/
		SetBoardState(STATE);
	}
	else if (_chance <= 80)
	{
		m_data->HudLBM.chosse = "Immunite";
		/*std::cout << m_data->HudLBM.chosse << std::endl;
		std::cout << m_data->HudLBM.name << std::endl;
		std::cout << m_data->HudLBM.state << std::endl;
		std::cout << m_data->HudLBM.active << std::endl;
		std::cout << m_data->HudLBM.swap << std::endl;
		std::cout << m_data->HudLBM.rando << std::endl;*/
		SetBoardState(STATE);
	}
	else if (_chance <= 1000)
	{
		m_data->HudLBM.chosse = "Swap";
		/*std::cout << m_data->HudLBM.chosse << std::endl;
		std::cout << m_data->HudLBM.name << std::endl;
		std::cout << m_data->HudLBM.state << std::endl;
		std::cout << m_data->HudLBM.active << std::endl;
		std::cout << m_data->HudLBM.swap << std::endl;
		std::cout << m_data->HudLBM.rando << std::endl;*/
		SetBoardState(STATE);
	}
}

void BaseGame::Malus(int _chance)
{
	if (_chance <= 35)
	{
		m_data->HudLBM.chosse = "CaseMoin";

		/*std::cout << m_data->HudLBM.chosse << std::endl;
		std::cout << m_data->HudLBM.name << std::endl;
		std::cout << m_data->HudLBM.state << std::endl;
		std::cout << m_data->HudLBM.active << std::endl;
		std::cout << m_data->HudLBM.swap << std::endl;
		std::cout << m_data->HudLBM.rando << std::endl;*/

		SetBoardState(STATE);
	}
	else if (_chance <= 60)
	{
		if (randmt::Chance(0.5f))
		{
			m_data->HudLBM.chosse = "PaseTour";
			/*std::cout << m_data->HudLBM.chosse << std::endl;
			std::cout << m_data->HudLBM.name << std::endl;
			std::cout << m_data->HudLBM.state << std::endl;
			std::cout << m_data->HudLBM.active << std::endl;
			std::cout << m_data->HudLBM.swap << std::endl;
			std::cout << m_data->HudLBM.rando << std::endl;*/
		}
		else
		{
			m_data->HudLBM.chosse = "Infection";
			/*std::cout << m_data->HudLBM.chosse << std::endl;
			std::cout << m_data->HudLBM.name << std::endl;
			std::cout << m_data->HudLBM.state << std::endl;
			std::cout << m_data->HudLBM.active << std::endl;
			std::cout << m_data->HudLBM.swap << std::endl;
			std::cout << m_data->HudLBM.rando << std::endl;*/
		}
		SetBoardState(STATE);
	}
	else if (_chance <= 70)
	{
		m_data->HudLBM.chosse = "Swap";
		/*std::cout << m_data->HudLBM.chosse << std::endl;
		std::cout << m_data->HudLBM.name << std::endl;
		std::cout << m_data->HudLBM.state << std::endl;
		std::cout << m_data->HudLBM.active << std::endl;
		std::cout << m_data->HudLBM.swap << std::endl;
		std::cout << m_data->HudLBM.rando << std::endl;*/
		SetBoardState(STATE);
	}
	else if (_chance <= 1000)
	{
		if (randmt::Chance(0.75f))
		{
			m_data->players[m_data->currentPlayerIndex].state = StatePlayer::CONFUSED;
			m_data->HudLBM.chosse = "Confus";
			//std::cout << m_data->HudLBM.chosse << std::endl;
			//std::cout << m_data->HudLBM.name << std::endl;
			//std::cout << m_data->HudLBM.state << std::endl;
			//std::cout << m_data->HudLBM.active << std::endl;
			//std::cout << m_data->HudLBM.swap << std::endl;
			//std::cout << m_data->HudLBM.rando << std::endl;
		}
		else
		{
			m_data->HudLBM.chosse = "ConfusSkip";
/*			std::cout << m_data->HudLBM.chosse << std::endl;
			std::cout << m_data->HudLBM.name << std::endl;
			std::cout << m_data->HudLBM.state << std::endl;
			std::cout << m_data->HudLBM.active << std::endl;
			std::cout << m_data->HudLBM.swap << std::endl;
			std::cout << m_data->HudLBM.rando << std::endl*/;
		}
		SetBoardState(STATE);
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

		m_data->smokeOff = true;

		switch (m_gameData->m_playerDataList[m_data->currentPlayerIndex].GetPlayerSkin())
		{
		case PlayerData::CHARACTER_1_1:
			m_gameData->m_playerDataList[m_data->currentPlayerIndex].SetPlayerSkin(PlayerData::CHARACTER_1_2);
			m_data->players[m_data->currentPlayerIndex].texture = *m_gameData->m_assetManager->GetAsset<TextureAnimated>("Perso1-2", AssetManager::AssetType::TEXTURE_ANIMATED);
			break;
		case PlayerData::CHARACTER_2_1:
			m_gameData->m_playerDataList[m_data->currentPlayerIndex].SetPlayerSkin(PlayerData::CHARACTER_2_2);
			m_data->players[m_data->currentPlayerIndex].texture = *m_gameData->m_assetManager->GetAsset<TextureAnimated>("Perso2-2", AssetManager::AssetType::TEXTURE_ANIMATED);
			break;
		case PlayerData::CHARACTER_3_1:
			m_gameData->m_playerDataList[m_data->currentPlayerIndex].SetPlayerSkin(PlayerData::CHARACTER_3_2);
			m_data->players[m_data->currentPlayerIndex].texture = *m_gameData->m_assetManager->GetAsset<TextureAnimated>("Perso3-2", AssetManager::AssetType::TEXTURE_ANIMATED);
			break;
		case PlayerData::CHARACTER_4_1:
			m_gameData->m_playerDataList[m_data->currentPlayerIndex].SetPlayerSkin(PlayerData::CHARACTER_4_2);
			m_data->players[m_data->currentPlayerIndex].texture = *m_gameData->m_assetManager->GetAsset<TextureAnimated>("Perso4-2", AssetManager::AssetType::TEXTURE_ANIMATED);
			break;
		default:
			m_gameData->m_playerDataList[m_data->currentPlayerIndex].SetPlayerSkin(PlayerData::CHARACTER_1_2);
			m_data->players[m_data->currentPlayerIndex].texture = *m_gameData->m_assetManager->GetAsset<TextureAnimated>("Perso1-1", AssetManager::AssetType::TEXTURE_ANIMATED);
			break;
		}

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

// Ajoutez cette nouvelle fonction ProcessFinRoll après ProcessBridgeRoll :
void BaseGame::ProcessFinRoll()
{
	int diceRoll = randmt::RandomInt(1, 6);
	auto& player = m_data->players[m_data->currentPlayerIndex];

	std::cout << "Lancer de dé pour la ligne d'arrivée : " << diceRoll << std::endl;

	if (diceRoll > 4)
	{
		// Réussite : le joueur a gagné !
		std::cout << "VICTOIRE ! Le joueur " << m_data->currentPlayerIndex << " a gagné !" << std::endl;
		player.waitingBridgeRoll = false;

		// Préparer la liste des joueurs triés par position X décroissante
		std::vector<std::pair<int, float>> playerPositions;
		for (int i = 0; i < m_data->players.size(); i++)
		{
			playerPositions.push_back({ i, m_data->players[i].boardPosition.x });
		}

		// Trier par position X décroissante (les plus avancés en premier)
		std::sort(playerPositions.begin(), playerPositions.end(),
			[](const std::pair<int, float>& a, const std::pair<int, float>& b) {
				return a.second > b.second;
			});

		// Remplir m_winIndex avec le gagnant en premier, puis les autres
		m_gameData->m_winIndex.clear();

		// Le gagnant actuel en premier
		m_gameData->m_winIndex.push_back(m_data->currentPlayerIndex);

		// Ajouter les autres joueurs triés par position X
		for (const auto& playerPos : playerPositions)
		{
			if (playerPos.first != m_data->currentPlayerIndex)
			{
				m_gameData->m_winIndex.push_back(playerPos.first);
			}
		}

		// Afficher l'ordre final (debug)
		std::cout << "Ordre final pour le podium : ";
		for (int idx : m_gameData->m_winIndex)
		{
			std::cout << "P" << idx << " ";
		}
		std::cout << std::endl;

		// Transition vers le podium
		//ChangeScene("Podium", false);
	}
	else
	{
		// Échec : le joueur reste bloqué et perd son tour
		std::cout << "Échec ! Vous ne pouvez pas franchir la ligne d'arrivée." << std::endl;
		player.waitingBridgeRoll = false;
		player.pendingMovement = 0;

		SetBoardState(CASE_ACTION);
	}
}

void BaseGame::SwapPlayers(int _swapIndex)
{
	std::cout << "Swap de place avec : Player " << _swapIndex << std::endl;

	auto& player1 = m_data->players[m_data->currentPlayerIndex];
	auto& player2 = m_data->players[_swapIndex];

	std::string player1Name = player1.playeur.getString();
	std::string player2Name = player2.playeur.getString();

	std::cout << " swap :" << player1Name << " et " << player2Name << std::endl;

	std::swap(m_data->players[m_data->currentPlayerIndex].currentCaseIndex, m_data->players[_swapIndex].currentCaseIndex);
	std::swap(m_data->players[m_data->currentPlayerIndex].boardPosition, m_data->players[_swapIndex].boardPosition);

	CreateSmokeEffectForSwap(player1);
	CreateSmokeEffectForSwap(player2);

	SetBoardState(CASE_ACTION_END);

}

void BaseGame::CaseAvancePlus(int _rando)
{
	auto& player = m_data->players[m_data->currentPlayerIndex];

	std::cout << "Avance de : " << _rando << "!" << std::endl;

	// Initialiser le mouvement restant pour le déplacement case par case
	player.sprite.setScale({ 1.f, 1.f });
	player.pendingMovement = _rando;

	SetBoardState(DEPLACEMENT_ACTION_2);
}

void BaseGame::ImuniteMalus()
{
	std::cout << "Imunite au Malus" << std::endl;

	m_data->players[m_data->currentPlayerIndex].state = StatePlayer::IMMUN;
	SetBoardState(CASE_ACTION_END);
}

void BaseGame::CaseMoins(int _rando)
{
	auto& player = m_data->players[m_data->currentPlayerIndex];

	std::cout << "Recule de : " << _rando << "!" << std::endl;

	// Initialiser le mouvement restant pour le déplacement case par case
	player.pendingMovement = _rando;

	// Calculer la première case du déplacement (en arrière)
	const int posCaseCount = static_cast<int>(m_data->posCase.size());
	int nextIndex = mathp::ModuloPositiveI(player.currentCaseIndex - 1, posCaseCount);

	// Retourner le sprite pour indiquer le recul
	player.sprite.setScale({ -1.f, 1.f });

	SetBoardState(DEPLACEMENT_ACTION_BACK, nextIndex);
}

void BaseGame::Infecte()
{
	std::cout << "Infectes pas de bonus " << std::endl;
	m_data->players[m_data->currentPlayerIndex].state = StatePlayer::INFEC;
	SetBoardState(CASE_ACTION_END);
}

void BaseGame::PaseTour()
{
	std::cout << "Passe son tour" << std::endl;
	m_data->players[m_data->currentPlayerIndex].state = StatePlayer::CANT_PLAY;
	SetBoardState(CASE_ACTION_END);
}

void BaseGame::CreateSmokeEffectForSwap(Player& _player)
{
	for (int i = 0; i < 63; i++)
	{
		//Creer un effet visuel ici 
		if (i % 3 == 0)
		{
			sf::Vector2f posEffect = _player.boardPosition;

			sf::Vector2u bit = _player.texture.GetTexture().getSize();

			posEffect.y -= bit.y * 2 / 6;

			Effect effect(m_data->smoke, posEffect + sf::Vector2f(-20 + randmt::RandomInt(0, 40), -20 + randmt::RandomInt(0, 40)), randmt::RandomFloat(0.5f, 1.f), 360 * randmt::RandomFloat(0, 360));
			m_data->effectSwap.push_back(effect);
		}
		else if (i % 2 == 0)
		{
			sf::Vector2f posEffect = _player.boardPosition;

			sf::Vector2u bit = _player.texture.GetTexture().getSize();

			posEffect.y -= bit.y * 1.5 / 6;

			Effect effect(m_data->smoke, posEffect + sf::Vector2f(-20 + randmt::RandomInt(0, 40), -20 + randmt::RandomInt(0, 40)), randmt::RandomFloat(0.5f, 1.f), 360 * randmt::RandomFloat(0, 360));
			m_data->effectSwap.push_back(effect);
		}
		else
		{

			sf::Vector2f posEffect = _player.boardPosition;

			sf::Vector2u bit = _player.texture.GetTexture().getSize();

			posEffect.y -= bit.y * 1 / 6;

			Effect effect(m_data->smoke, posEffect + sf::Vector2f(-20 + randmt::RandomInt(0, 40), -20 + randmt::RandomInt(0, 40)), randmt::RandomFloat(0.5f, 1.f), 360 * randmt::RandomFloat(0, 360));
			m_data->effectSwap.push_back(effect);
		}
	}
}

void BaseGame::CreateSmokeEffectAnotherPart(sf::Vector2f _posMin, sf::Vector2f _posMax)
{
	for (int i = 0; i < 1002; i++)
	{
		sf::Vector2f posEffect;

		posEffect.x = randmt::RandomFloat(_posMin.x + 512 / 2, _posMax.x);

		posEffect.y = randmt::RandomFloat(0, SCREEN_HEIGHT);

		Effect effect(m_data->smoke, posEffect + sf::Vector2f(-20 + randmt::RandomInt(0, 40), -20 + randmt::RandomInt(0, 40)), randmt::RandomFloat(0.5f, 1.f), 360 * randmt::RandomFloat(0, 360));
		m_data->effectsMap.push_back(effect);
	}
}

void BaseGame::UpdateLBM(float _dt)
{
	if (m_data->HudLBM.state != NONELBM)
	{
		m_data->timeLBM -= _dt;

		LBMDisplayUpdate(_dt);

		if (m_data->timeLBM <= 0 && m_data->HudLBM.active)
		{

			if (m_data->HudLBM.chosse == "CasePlus")
			{
				CaseAvancePlus(m_data->HudLBM.rando);
			}
			else if (m_data->HudLBM.chosse == "Immunite")
			{
				ImuniteMalus();
			}
			else if (m_data->HudLBM.chosse == "Swap")
			{
				SwapPlayers(m_data->HudLBM.swap);
				return;
			}

			if (m_data->HudLBM.chosse == "CaseMoin")
			{
				CaseMoins(m_data->HudLBM.rando);
			}
			else if (m_data->HudLBM.chosse == "Infection")
			{
				Infecte();
			}
			else if (m_data->HudLBM.chosse == "PaseTour")
			{
				PaseTour();
			}
			else if (m_data->HudLBM.chosse == "Swap")
			{
				SwapPlayers(m_data->HudLBM.swap);
			}
			else if (m_data->HudLBM.chosse == "Confus")
			{
				SetBoardState(CASE_ACTION_END);
			}
			else if (m_data->HudLBM.chosse == "ConfusSkip")
			{
				SetBoardState(CASE_ACTION_END);
			}

			m_data->HudLBM.state = NONELBM;
			m_data->timeLBM = TIME_LBM_DISPLAY;
			m_data->HudLBM.active = false;
			m_data->HudLBM.text.setString("");
			m_data->HudLBM.rando = 0;
			m_data->HudLBM.swap = 0;
			m_data->HudLBM.name = "";
			m_data->HudLBM.chosse = "";
		}
	}
}

void BaseGame::LBMDisplayUpdate(float _dt)
{

	if (m_data->timeLBM <= TIME_LBM_DISPLAY * 0.75f && !m_data->HudLBM.active)
	{
		m_data->HudLBM.sprite.SetAnimation(m_data->HudLBM.name + "Face");
		m_data->HudLBM.sprite.Update(_dt);
		sf::Vector2u size = m_data->HudLBM.sprite.getTexture()->getSize();

		sf::Vector2f size2 = m_gameData->m_renderWindow->getView().getCenter();

		m_data->HudLBM.sprite.setScale({ 0.5f , 0.5f });
		m_data->HudLBM.sprite.setOrigin({ 0.5,0.5 });
		m_data->HudLBM.sprite.setPosition({ size2.x, size2.y });

		//m_data->HudLBM.sprite.Update(0.016);


		if (m_data->HudLBM.chosse == "CasePlus")
		{
			m_data->HudLBM.rando = randmt::RandomInt(1, 3);

			m_data->HudLBM.text.setString("Avance de " + std::to_string(m_data->HudLBM.rando) + " Case ");

			sf::FloatRect s = m_data->HudLBM.text.getGlobalBounds();

			m_data->HudLBM.text.setOrigin({ s.width / 2,s.height / 2 });

			m_data->HudLBM.text.setPosition({ size2.x, size2.y });

			m_data->HudLBM.active = true;
		}
		else if (m_data->HudLBM.chosse == "Immunite")
		{
			m_data->HudLBM.text.setString("Imuniser au Malus Pendant 2 Tour");

			sf::FloatRect s = m_data->HudLBM.text.getGlobalBounds();

			m_data->HudLBM.text.setOrigin({ s.width / 2,s.height / 2 });

			m_data->HudLBM.text.setPosition({ size2.x, size2.y });

			m_data->HudLBM.active = true;
		}
		else if (m_data->HudLBM.chosse == "Swap")
		{
			m_data->HudLBM.swap = randmt::RandomInt(0, (int)m_data->players.size() - 1);
			while (m_data->HudLBM.swap == m_data->currentPlayerIndex)
			{
				m_data->HudLBM.swap = randmt::RandomInt(0, (int)m_data->players.size() - 1);
			}

			m_data->HudLBM.text.setString("Swap de Place avec le Joueur " + std::to_string(m_data->HudLBM.swap));

			sf::FloatRect s = m_data->HudLBM.text.getGlobalBounds();

			m_data->HudLBM.text.setOrigin({ s.width / 2,s.height / 2 });

			m_data->HudLBM.text.setPosition({ size2.x, size2.y });

			m_data->HudLBM.active = true;
		}

		if (m_data->HudLBM.chosse == "CaseMoin")
		{
			m_data->HudLBM.rando = randmt::RandomInt(1, 3);

			m_data->HudLBM.text.setString("Recule de " + std::to_string(m_data->HudLBM.rando) + " Case ");

			sf::FloatRect s = m_data->HudLBM.text.getGlobalBounds();

			m_data->HudLBM.text.setOrigin({ s.width / 2,s.height / 2 });

			m_data->HudLBM.text.setPosition({ size2.x, size2.y });

			m_data->HudLBM.active = true;
		}
		else if (m_data->HudLBM.chosse == "Infection")
		{
			m_data->HudLBM.text.setString("Infecte pas de Bonus pendans 2 Tours ");

			sf::FloatRect s = m_data->HudLBM.text.getGlobalBounds();

			m_data->HudLBM.text.setOrigin({ s.width / 2,s.height / 2 });

			m_data->HudLBM.text.setPosition({ size2.x, size2.y });

			m_data->HudLBM.active = true;
		}
		else if (m_data->HudLBM.chosse == "PaseTour")
		{

			m_data->HudLBM.text.setString("Passe son Tour au prochains Tour");

			sf::FloatRect s = m_data->HudLBM.text.getGlobalBounds();

			m_data->HudLBM.text.setOrigin({ s.width / 2,s.height / 2 });

			m_data->HudLBM.text.setPosition({ size2.x, size2.y });

			m_data->HudLBM.active = true;
		}
		else if (m_data->HudLBM.chosse == "Swap")
		{
			m_data->HudLBM.swap = randmt::RandomInt(0, (int)m_data->players.size() - 1);
			while (m_data->HudLBM.swap == m_data->currentPlayerIndex)
			{
				m_data->HudLBM.swap = randmt::RandomInt(0, (int)m_data->players.size() - 1);
			}

			m_data->HudLBM.text.setString("Swap de Place avec le Joueur " + m_data->players[m_data->HudLBM.swap].playeur.getString());

			sf::FloatRect s = m_data->HudLBM.text.getGlobalBounds();

			m_data->HudLBM.text.setOrigin({ s.width / 2,s.height / 2 });

			m_data->HudLBM.text.setPosition({ size2.x, size2.y });

			m_data->HudLBM.active = true;
		}
		else if (m_data->HudLBM.chosse == "Confus")
		{

			m_data->HudLBM.text.setString("Confus le Prochain Tour votre lance de Dée vous fait reculer");

			sf::FloatRect s = m_data->HudLBM.text.getGlobalBounds();

			m_data->HudLBM.text.setOrigin({ s.width / 2,s.height / 2 });

			m_data->HudLBM.text.setPosition({ size2.x, size2.y });

			m_data->HudLBM.active = true;


		}
		else if (m_data->HudLBM.chosse == "ConfusSkip")
		{
			m_data->HudLBM.text.setString("Confus Evite vous avez de la chance");

			sf::FloatRect s = m_data->HudLBM.text.getGlobalBounds();

			m_data->HudLBM.text.setOrigin({ s.width / 2,s.height / 2 });

			m_data->HudLBM.text.setPosition({ size2.x, size2.y });

			m_data->HudLBM.active = true;
		}

		m_data->HudLBM.active = true;
	}
}

void BaseGame::DrawLBM(sf::RenderWindow& _renderWindow)
{
	if (m_data->HudLBM.state != NONELBM)
	{
		_renderWindow.draw(m_data->HudLBM.sprite);
		_renderWindow.draw(m_data->HudLBM.text);
	}
}

void BaseGame::DrawIconePlayer(sf::RenderWindow& _renderWindow, int _i)
{
	sf::Vector2f size2 = m_gameData->m_renderWindow->getView().getCenter();
	switch (m_data->players[_i].posIcone)
	{
	case UP_LEFT:
		if (_i != m_data->currentPlayerIndex)
		{
			m_data->icone.setPosition({ size2.x - SCREEN_WIDTH / 2.f + 72.5f , size2.y - SCREEN_HEIGHT / 2.f + 305.f * 0.55f });
		}
		else
		{
			m_data->icone.setPosition({ size2.x - SCREEN_WIDTH / 2.f + 72.5f , size2.y - SCREEN_HEIGHT / 2.f + 305.f * 0.70f });
		}
		break;
	case UP_RIGHT:
		if (_i != m_data->currentPlayerIndex)
		{
			m_data->icone.setPosition({ size2.x + SCREEN_WIDTH / 2 - 72.5f , size2.y - SCREEN_HEIGHT / 2 + 305.f * 0.50f });
		}
		else
		{
			m_data->icone.setPosition({ size2.x + SCREEN_WIDTH / 2 - 72.5f , size2.y - SCREEN_HEIGHT / 2 + 305.f * 0.70f });
		}
		break;
	case DOWN_LEFT:
		if (_i != m_data->currentPlayerIndex)
		{
			m_data->icone.setPosition({ size2.x - SCREEN_WIDTH / 2 + 72.5f  , size2.y + SCREEN_HEIGHT / 2 - 10 });
		}
		else
		{
			m_data->icone.setPosition({ size2.x - SCREEN_WIDTH / 2 + 72.5f  , size2.y + SCREEN_HEIGHT / 2 - 10 });
		}
		break;
	case DONW_RIGHT:
		if (_i != m_data->currentPlayerIndex)
		{
			m_data->icone.setPosition({ size2.x + SCREEN_WIDTH / 2 - 72.5f  , size2.y + SCREEN_HEIGHT / 2 - 10 });
		}
		else
		{
			m_data->icone.setPosition({ size2.x + SCREEN_WIDTH / 2 - 72.5f  , size2.y + SCREEN_HEIGHT / 2 - 10 });
		}
		break;
	default:
		break;
	}

	switch (m_gameData->m_playerDataList[_i].GetPlayerSkin())
	{
	case PlayerData::CHARACTER_1_1:
		m_data->icone.SetAnimation("Perso1-1");
		break;
	case PlayerData::CHARACTER_1_2:
		m_data->icone.SetAnimation("Perso1-2");
		break;
	case PlayerData::CHARACTER_2_1:
		m_data->icone.SetAnimation("Perso2-1");
		break;
	case PlayerData::CHARACTER_2_2:
		m_data->icone.SetAnimation("Perso2-2");
		break;
	case PlayerData::CHARACTER_3_1:
		m_data->icone.SetAnimation("Perso3-1");
		break;
	case PlayerData::CHARACTER_3_2:
		m_data->icone.SetAnimation("Perso3-2");
		break;
	case PlayerData::CHARACTER_4_1:
		m_data->icone.SetAnimation("Perso4-1");
		break;
	case PlayerData::CHARACTER_4_2:
		m_data->icone.SetAnimation("Perso4-2");
		break;
	default:
		break;
	}

	
	if (_i != m_data->currentPlayerIndex)
	{
		m_data->icone.setColor({ 255,255,255,155 });
		m_data->icone.setScale({ 0.75f,0.75f });
	}

	_renderWindow.draw(m_data->icone);

	m_data->icone.setColor({ 255,255,255,255 });
	m_data->icone.setScale({ 1,1 });
}
