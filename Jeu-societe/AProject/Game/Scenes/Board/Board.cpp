 #include "Board.hpp"

constexpr unsigned int hash(const char* str, int h);

constexpr unsigned int hash(const char* str, int h = 0)
{
	return !str[h] ? 5381 : (hash(str, h + 1) * 33) ^ str[h];
}

// Ajouter cette nouvelle méthode avant BaseGame::Load
void BaseGame::LoadAsync(std::atomic<float>& progress)
{
	m_data = new SceneData;
	m_gameData = (GameData*)this->m_keptData;

	progress.store(0.1f);

	// CHANGEMENT : Ne plus charger le manifest ici car il est déjà chargé par LoadingScreen
	// m_gameData->m_assetManager->LoadManifest("Manifests/Board.json", "Board");
	progress.store(0.3f);

	// Configuration des éléments UI
	m_data->HudLBM.text.setFont(*m_gameData->m_assetManager->GetAsset<sf::Font>("BoardFont", AssetManager::AssetType::FONT));
	m_data->HudLBM.sprite.setTexture(*m_gameData->m_assetManager->GetAsset<TextureAnimated>("Anim_card", AssetManager::AssetType::TEXTURE_ANIMATED));
	m_data->HudLBM.state = NONELBM;
	m_data->HudLBM.active = false;
	progress.store(0.4f);

	// Chargement de la carte
	//m_gameData->m_tile.InitTiled("Assets/Map/map.json");
	m_gameData->m_renderWindow->ResetView();
	m_data->camera.Reset(m_gameData->m_renderWindow->getView());
	progress.store(0.5f);

	MapLayer layer = m_gameData->m_tile->GetMapLayer("point");
	m_data->posCase = layer.GetObjects();

	m_data->players.resize(m_gameData->m_playerDataList.size());
	m_data->state = INTRO;

	m_data->icone.setTexture(*m_gameData->m_assetManager->GetAsset<TextureAnimated>("Icone", AssetManager::AssetType::TEXTURE_ANIMATED));
	m_data->icone.setOrigin({ 0.5f,1 });

	m_data->iconeState.setTexture(*m_gameData->m_assetManager->GetAsset<TextureAtlas>("Effect", AssetManager::AssetType::TEXTURE_ATLAS));
	m_data->iconeState.setOrigin({ 0.5f,1 });

	m_data->iconeAura.setTexture(*m_gameData->m_assetManager->GetAsset<TextureAnimated>("IconeAura", AssetManager::AssetType::TEXTURE_ANIMATED));
	m_data->iconeAura.setOrigin({ 0.5f,1 });

	m_data->timeWin = TIME_WIN_DISPLAY;
	m_data->timeLBM = TIME_LBM_DISPLAY;
	m_data->timeDice = TIME_DIS_DISPLAY;
	m_data->timeStart = TIME_START_DISPLAY;

	// NOUVEAU : Initialisation de la vidéo du dé
	m_data->diceAnimationPlaying = false;
	m_data->diceResult = 0;
	m_data->dicePosition = sf::Vector2f(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f);

	// NOUVEAU : Pré-charger toutes les vidéos des dés
	m_data->diceVideos.resize(8);
	// Configuration simple sans taille personnalisée
	HighResConfig config;
	config.sizeMode = VideoSizeMode::Original; // Garder la résolution originale
	config.enableLoop = false;

	for (int i = 0; i < 6; i++)
	{
		m_data->diceVideos[i] = new HighResVideoPlayer(config);
		std::string videoPath = "Assets/Video/De" + std::to_string(i + 1) + ".mov";
		if (!m_data->diceVideos[i]->loadFromFile(videoPath))
		{
			std::cout << "Erreur : Impossible de charger la video du de: " << videoPath << std::endl;
		}
	}

	m_data->diceVideos[TRANSITION_1] = new HighResVideoPlayer(config);
	m_data->diceVideos[TRANSITION_2] = new HighResVideoPlayer(config);
	m_data->diceVideos[TRANSITION_1]->loadFromFile("Assets/Video/TRANSITION_1.mp4"); // Vidéo finale optionnelle
	m_data->diceVideos[TRANSITION_2]->loadFromFile("Assets/Video/TRANSITION_2.mp4"); // Vidéo finale optionnelle


	m_data->currentDiceVideo = m_data->diceVideos[TRANSITION_1];
	m_data->currentDiceVideo->play();
	m_data->currentDiceVideo->update(1);

	progress.store(0.7f);

	// NOUVEAU : Charger le shader pour la transparence
	if (!m_data->chromaKeyShader.loadFromMemory(
		R"(
        uniform sampler2D texture;
        uniform vec3 keyColor; // Couleur à rendre transparente (ex: vert)
        uniform float threshold; // Seuil de tolérance

        void main()
        {
            vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);
            float dist = distance(pixel.rgb, keyColor);
            
            // Si la couleur est proche de keyColor, rendre transparent
            if (dist < threshold)
            {
                pixel.a = 0.0;
            }
            
            gl_FragColor = pixel * gl_Color;
        }
        )", sf::Shader::Fragment))
	{
		std::cout << "Erreur : Impossible de charger le shader chroma key" << std::endl;
	}
	else
	{
		// Définir la couleur à rendre transparente (vert dans cet exemple)
		m_data->chromaKeyShader.setUniform("keyColor", sf::Glsl::Vec3(0.0f, 1.0f, 0.0f)); // RGB vert
		m_data->chromaKeyShader.setUniform("threshold", 0.5f); // Ajuster selon vos besoins
	}

	progress.store(0.6f);

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
		m_data->players[i].v.setString(L"↓");
		m_data->players[i].boardPosition = m_data->posCase[0].GetPosition() + sf::Vector2f{ -40.f * i + 50 ,0.f };
		m_data->players[i].posIcone = PosIcone(i);

		sf::FloatRect textBounds = m_data->players[i].v.getLocalBounds();
		m_data->players[i].v.setOrigin({ textBounds.width / 2.f, textBounds.height / 2.f });
		m_data->players[i].v.setPosition(m_data->players[i].boardPosition + sf::Vector2f{ 0.f,-100.f });
		m_data->players[i].v.setOutlineColor(sf::Color::Black);
		m_data->players[i].v.setOutlineThickness(2.0f);

		m_data->players[i].playeur.setFont(*m_gameData->m_assetManager->GetAsset<sf::Font>("BoardFont", AssetManager::AssetType::FONT));
		m_data->players[i].playeur.setString("P" + std::to_string(i + 1));
		m_data->players[i].playeur.setOutlineColor(sf::Color::Black);
		m_data->players[i].playeur.setOutlineThickness(2.0f);

		m_data->players[i].playeur.setOrigin({ 0.5, 0.5 });
		m_data->players[i].playeur.setPosition(m_data->players[i].boardPosition + sf::Vector2f{ 0.f,-120.f });

		// NOUVEAU : Initialisation du texte pour le numéro du dé
		m_data->players[i].diceNumber.setFont(*m_gameData->m_assetManager->GetAsset<sf::Font>("BoardFont", AssetManager::AssetType::FONT));
		m_data->players[i].diceNumber.setString("");
		m_data->players[i].diceNumber.setCharacterSize(40);
		m_data->players[i].diceNumber.setFillColor(sf::Color::Yellow);
		m_data->players[i].diceNumber.setOutlineColor(sf::Color::Black);
		m_data->players[i].diceNumber.setOutlineThickness(2.0f);
		
		m_data->players[i].diceNumber.setOrigin({0.5, 1});
		m_data->players[i].diceNumber.setPosition(m_data->players[i].boardPosition - sf::Vector2f{ 0.f,700.f });

		m_data->players[i].currentCaseIndex = 0;
		m_data->players[i].startRandom = 0;
		m_data->players[i].state = StatePlayer::NONE;
		m_data->players[i].pendingMovement = 0;
		m_data->players[i].currentPathId = -1;

		progress.store(0.6f + (0.2f * (i + 1) / m_data->players.size()));
	}

	m_data->currentPlayerIndex = 0;
	m_data->smokeOff = false;
	m_data->smoke.loadFromFile("Assets/Sprites/Board/smoke-export.png");
	progress.store(0.85f);

	UpdateCameraToShowAllPlayers();

	sf::Vector2f posMin;
	sf::Vector2f posMax;

	for (int i = 0; i < m_data->posCase.size(); i++)
	{
		auto& mapObject = m_data->posCase[i];
		if (mapObject.GetName() == "19")
		{
			posMin = mapObject.GetPosition() + sf::Vector2f(100.f, 0);
		}
		if (i == m_data->posCase.size() - 1)
		{
			posMax = mapObject.GetPosition();
		}
	}

	CreateSmokeEffectAnotherPart(posMin, posMax);
	progress.store(0.95f);

	// Configuration des animateurs
	m_data->animator.Modify(1.0f, 60.0f, false, 1.0f);
	m_data->animator2.Modify(1.0f, 60.0f, false, 1.0f);
	m_data->animator.SetAnimationEasing(anim::Animator::GOTO, anim::Easing::INOUTSINE);
	m_data->animator2.SetAnimationEasing(anim::Animator::GOTO, anim::Easing::INOUTSINE);
	m_data->animator.End();
	m_data->animator2.End();

	m_data->arrow.setTexture(*m_gameData->m_assetManager->GetAsset<sf::Texture>("Arrow", AssetManager::AssetType::TEXTURE));

	m_data->arrow.setOrigin({ 0 , m_data->arrow.getTexture()->getSize().y / 2.f });

	for (int i = 0; i < m_data->posCase.size(); i++)
	{
		auto& mapObject = m_data->posCase[i];
		if (mapObject.GetName() == "6")
		{
			m_data->arrow.setPosition(mapObject.GetPosition() + sf::Vector2f(150, 0));
		}

	}

	m_data->arrow.setScale({ 0.5f,0.5f });

	m_data->arrow.setRotation(315);


	m_data->HudLBM.text.SetCharactersPerLine(25);


	progress.store(1.0f);
}

void BaseGame::Load(void)
{
	// Version synchrone pour compatibilité
	std::atomic<float> dummyProgress{ 0.0f };
	LoadAsync(dummyProgress);
}

void BaseGame::Unload(void)
{
	m_gameData->m_assetManager->DeleteContainer("Board");
	this->m_gameData = NULL;
	delete this->m_data;
	this->m_data = NULL;
}

// NOUVEAU : Méthode helper pour vérifier les entrées joueur
bool BaseGame::CheckPlayerInput(sf::Event& _event, bool& shouldRoll)
{
	shouldRoll = false;

	// Gestion joystick
	if (_event.type == sf::Event::JoystickButtonPressed)
	{
		if (m_gameData->m_playerDataList[m_data->currentPlayerIndex].m_joystickId == _event.joystickButton.joystickId)
		{
			if (_event.joystickButton.button == 0)
			{
				shouldRoll = true;
				return true;
			}
		}
	}

	// Gestion clavier (DEBUG)
	if (_event.type == sf::Event::KeyPressed)
	{
		if (_event.key.code == sf::Keyboard::Space)
		{
			shouldRoll = true;
			return true;
		}
	}

	return false;
}


// NOUVEAU : Méthode helper pour traiter le lancer de dé
void BaseGame::ProcessDiceRoll(int rando)
{
	std::cout << "Roll Dice: " << rando << std::endl;

	if (m_data->state != START)
	{
		auto& player = m_data->players[m_data->currentPlayerIndex];

		m_data->diceResult = rando;

		if (rando >= 1 && rando <= 6)
		{
			m_data->currentDiceVideo = m_data->diceVideos[rando - 1];
			m_data->currentDiceVideo->play();
			m_data->diceAnimationPlaying = true;
			SetBoardState(DICE_ANIMATION);
		}
	}
	else if (m_data->players[m_data->currentPlayerIndex].startRandom == 0)
	{
		// MODIFICATION : Lancer la vidéo du dé pendant START
		auto& player = m_data->players[m_data->currentPlayerIndex];
		
		m_data->diceResult = rando;
		
		if (rando >= 1 && rando <= 6)
		{
			// Lancer la vidéo du dé
			m_data->currentDiceVideo = m_data->diceVideos[rando - 1];
			m_data->currentDiceVideo->play();
			m_data->currentDiceVideo->update(1);
			m_data->diceAnimationPlaying = true;
			
			// Le numéro sera affiché après l'animation
			player.startRandom = rando;
			//player.diceNumber.setString(std::to_string(player.startRandom));
		}
		
		std::cout << "Place: " << rando << std::endl;
	}
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

	// Gestion simplifiée des états d'attente de lancer
	if (m_data->state == WAITING_FIN_ROLL)
	{
		bool shouldRoll = false;
		if (CheckPlayerInput(_event, shouldRoll) && shouldRoll)
		{
			ProcessFinRoll();
		}
		return;
	}

	if (m_data->state == WAITING_BRIDGE_ROLL)
	{
		bool shouldRoll = false;
		if (CheckPlayerInput(_event, shouldRoll) && shouldRoll)
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
						m_data->arrow.setRotation(315);
						std::cout << "Top path selected (joystick)" << std::endl;
					}
					else if (_event.joystickMove.position > 50.0f) // Bas
					{
						ProcessPathChoice(1);
						m_data->arrow.setRotation(25);
						std::cout << "Bottom path selected (joystick)" << std::endl;
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
				m_data->arrow.setRotation(315);
				std::cout << "Top path selected (keyboard)" << std::endl;
			}
			else if (_event.key.code == sf::Keyboard::Down || _event.key.code == sf::Keyboard::S)
			{
				ProcessPathChoice(1);
				m_data->arrow.setRotation(25);
				std::cout << "Bottom path selected (keyboard)" << std::endl;
			}

			if (_event.key.code == sf::Keyboard::Space)
			{
				m_data->players[m_data->currentPlayerIndex].sprite.SetAnimation("");
				SetBoardState(DEPLACEMENT_SPLIT);
			}
		}

		return; // Ne pas traiter d'autres événements pendant le choix
	}

	// Gestion du lancer de dé principal
	if (_event.type == sf::Event::JoystickButtonPressed)
	{
		if (m_gameData->m_playerDataList[m_data->currentPlayerIndex].m_joystickId == _event.joystickButton.joystickId &&
			m_data->state != WIN_DEPLACEMENT && m_data->state != WIN && m_data->state != STATE &&
			m_data->state != DICE_ANIMATION && m_data->state != DUEL && m_data->state != BATTLE_ACTION
			&& m_data->state != INTRO)
		{
			if (_event.joystickButton.button == 0 && m_data->animator.IsFinished())
			{
				int rando = randmt::RandomInt(1, 6);
				ProcessDiceRoll(rando);
			}
		}
	}

	// Gestion des entrées clavier (DEBUG)
	if (_event.type == sf::Event::KeyPressed)
	{
		if (_event.key.code == sf::Keyboard::Space && m_data->animator.IsFinished() &&
			m_data->state != WIN_DEPLACEMENT && m_data->state != WIN && m_data->state != STATE &&
			m_data->state != DICE_ANIMATION && m_data->state != DUEL && m_data->state != BATTLE_ACTION
			&& m_data->state != INTRO)
		{
			int rando = randmt::RandomInt(1, 6);
			ProcessDiceRoll(rando);
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

		for (int i = (int)m_data->effectSwap.size() - 1; i >= 0; i--)
		{
			auto& effect = m_data->effectSwap[i];
			effect.Update(_deltaTime);

			if (!effect.IsActive())
			{
				effect = m_data->effectSwap.back();
				m_data->effectSwap.pop_back();
			}
		}

		for (int i = (int)m_data->effectsMap.size() - 1; i >= 0; i--)
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

		// MODIFICATION : Mettre à jour aussi la position du texte du dé
		for (auto& player : m_data->players)
		{
			player.v.setPosition(player.boardPosition + sf::Vector2f{ 0.f,-250.f });
			player.playeur.setPosition(player.boardPosition + sf::Vector2f{ 0.f,-275.f });
			player.diceNumber.setPosition(player.boardPosition + sf::Vector2f{ 0.f,-300.f }); // NOUVEAU
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

// NOUVEAU : Méthode helper pour initier un mouvement
void BaseGame::InitiateMovement(int nextIndex, bool backwards)
{
	auto& player = m_data->players[m_data->currentPlayerIndex];

	sf::Vector2f startPos = player.boardPosition;
	sf::Vector2f endPos = m_data->posCase[nextIndex].GetPosition() +
		sf::Vector2f{ randmt::RandomFloat(-10, 10), randmt::RandomFloat(-10, 10) };

	m_data->animator.SetGoTo(startPos, endPos);
	m_data->animator.Restart();

	player.currentCaseIndex = nextIndex;
	player.sprite.SetAnimation("Right_Walk");

	if (backwards)
	{
		player.sprite.setScale(-1, 1);
	}

	// Vérifier si on arrive sur une convergence
	const MapObject& nextCase = m_data->posCase[nextIndex];
	if (nextCase.GetType() == "merge")
	{
		player.currentPathId = -1; // Retour au chemin principal
	}
}

// NOUVEAU : Méthode helper pour gérer les états de mouvement
void BaseGame::HandleMovementState(State state, float _dt)
{
	m_data->players[m_data->currentPlayerIndex].sprite.Update(_dt);
	m_data->players[m_data->currentPlayerIndex].boardPosition = m_data->animator.GetGoTo();

	if (m_data->animator.IsFinished())
	{
		auto& player = m_data->players[m_data->currentPlayerIndex];
		player.pendingMovement--;

		std::cout << "Mouvement restant : " << player.pendingMovement << std::endl;

		if (player.pendingMovement > 0)
		{
			// Continuer le déplacement
			SetBoardState(state);
		}
		else
		{
			// Déplacement terminé
			State nextState = (state == DEPLACEMENT_ACTION || state == DEPLACEMENT_ACTION_BACK || state == DEPLACEMENT_ACTION_2)
				? CASE_ACTION_END : CASE_ACTION;
			SetBoardState(nextState);
		}
	}
}

void BaseGame::Draw(sf::RenderWindow& _renderWindow)
{
	const sf::View& referenceView = m_data->camera.GetView();
	sfMod::RenderWindow* mod = m_gameData->m_renderWindow;
	mod->setView(referenceView);

	std::string layer = "point";

	m_gameData->m_tile->DrawMapLayers(*mod, referenceView.getCenter(), layer);

	SortDrawOrder();

	m_gameData->m_tile->DrawMapLayers(*mod, referenceView.getCenter(), "point");

	for (auto& effect : m_data->effectSwap)
	{
		effect.Draw(*mod);
	}

	for (auto& effect : m_data->effectsMap)
	{
		effect.Draw(*mod);
	}

	DrawLBM(*mod);


	mod->draw(m_data->arrow);


	m_gameData->m_renderWindow->ResetView();
	for (int i = 0; i < m_data->players.size(); i++)
	{
		DrawIconePlayer(*mod, i);
	}

	if (!m_data->currentDiceVideo->isFinish())
	{
		// Centre de la vue
		sf::Vector2f cameraCenter = { SCREEN_WIDTH / 2.f, SCREEN_HEIGHT / 2.f };

		sf::Sprite videoSprite(m_data->currentDiceVideo->getSprite());

		// Obtenir la taille du sprite vidéo
		sf::FloatRect videoBounds = videoSprite.getLocalBounds();

		// Centrer l'origine du sprite
		videoSprite.setOrigin(videoBounds.width / 2.0f, videoBounds.height / 2.0f);

		// Positionner au centre de la caméra
		videoSprite.setPosition(cameraCenter);

		//// NOUVEAU : Ajuster la taille d'affichage (exemple : 2x plus grand)
		//float desiredDisplaySize = 600.0f; // Taille souhaitée en pixels à l'écran
		//float currentSize = videoBounds.width; // Taille actuelle du sprite
		//float displayScale = desiredDisplaySize / currentSize;
		//
		//videoSprite.setScale(displayScale, displayScale);

		// Dessiner avec le shader de chroma key
		mod->draw(videoSprite, &m_data->chromaKeyShader);
	}
}

// Modification de SortDrawOrder pour afficher les numéros de dé pendant START
void BaseGame::SortDrawOrder()
{
	sfMod::RenderWindow* mod = m_gameData->m_renderWindow;
	std::vector<int> indices(m_data->players.size());
	std::iota(indices.begin(), indices.end(), 0);

	std::sort(indices.begin(), indices.end(),
		[this](int a, int b) {
			return m_data->players[a].boardPosition.y < m_data->players[b].boardPosition.y;
		});

	for (int idx : indices)
	{
		m_data->players[idx].sprite.setPosition(m_data->players[idx].boardPosition);
		mod->draw(m_data->players[idx].sprite);
		
		if (idx == m_data->currentPlayerIndex)
		{
			m_data->players[idx].v.setFillColor(sf::Color::Cyan);
			m_data->players[idx].playeur.setFillColor(sf::Color::Cyan);
		}
		else
		{
			m_data->players[idx].v.setFillColor(sf::Color::White);
			m_data->players[idx].playeur.setFillColor(sf::Color::White);
		}
		
		mod->draw(m_data->players[idx].v);
		mod->draw(m_data->players[idx].playeur);
		
		// NOUVEAU : Afficher le numéro du dé si on est en phase START et que le joueur a lancé
		if (m_data->state == START && m_data->players[idx].diceNumber.getString() != '0')
		{
			mod->draw(m_data->players[idx].diceNumber);
		}
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

		m_data->HudLBM.sprite.setScale({ 0.25f , 0.25f });
		m_data->HudLBM.sprite.setOrigin({ 0.5,0.5 });
		m_data->HudLBM.sprite.setPosition({ SCREEN_WIDTH / 2  ,   SCREEN_WIDTH / 10 });
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

		m_data->HudLBM.sprite.setScale({ 0.25f , 0.25f });
		m_data->HudLBM.sprite.setOrigin({ 0.5,0.5 });
		m_data->HudLBM.sprite.setPosition({ SCREEN_WIDTH / 2  ,   SCREEN_WIDTH / 10 });
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

		m_data->HudLBM.sprite.setScale({ 0.25f , 0.25f });
		m_data->HudLBM.sprite.setOrigin({ 0.5,0.5 });
		m_data->HudLBM.sprite.setPosition({ SCREEN_WIDTH / 2  ,   SCREEN_WIDTH / 10 });
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
		m_data->arrow.setRotation(315);
		break;
	case WIN:
		m_data->players[m_data->currentPlayerIndex].sprite.SetAnimation("Idle");
		m_data->currentDiceVideo = m_data->diceVideos[TRANSITION_1];
		m_data->currentDiceVideo->play();
		break;
	case BATTLE_ACTION:
		[[fallthrough]];
	case DUEL:
		m_data->players[m_data->currentPlayerIndex].sprite.SetAnimation("Idle");
		m_data->currentDiceVideo = m_data->diceVideos[TRANSITION_2];
		m_data->currentDiceVideo->play();
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

		// Vérifier si on arrive sur un pont ou ligne d'arrivée
		const MapObject& currentCase = m_data->posCase[player.currentCaseIndex];
		std::string caseType = "";
		if (currentCase.GetPropertyByName("type") != nullptr)
		{
			caseType = currentCase.GetPropertyByName("type")->GetStringValue();
		}

		if (player.pendingMovement > 0)
		{
			if (caseType == "bridge")
			{
				player.sprite.SetAnimation("Idle");
				std::cout << "Pont détecté ! Lancez le dé pour traverser..." << std::endl;
				player.waitingBridgeRoll = true;
				SetBoardState(WAITING_BRIDGE_ROLL);
				return;
			}

			if (caseType == "end")
			{
				player.sprite.SetAnimation("Idle");
				std::cout << "Ligne d'arrivée détectée ! Lancez le dé pour franchir..." << std::endl;
				player.waitingBridgeRoll = true;
				SetBoardState(WAITING_FIN_ROLL);
				return;
			}
		}

		// Pas de choix : avancer automatiquement
		std::vector<int> availablePaths = GetAvailablePaths(player.currentCaseIndex);
		if (availablePaths.empty())
		{
			std::cout << "Erreur : aucun chemin disponible!" << std::endl;
			SetBoardState(CASE_ACTION_END);
			return;
		}

		InitiateMovement(availablePaths[0]);
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

		if (player.currentPathId != -1 && availablePaths.size() > 1)
		{
			nextIndex = availablePaths[player.currentPathId - 1];
		}

		InitiateMovement(nextIndex);
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

		InitiateMovement(availablePaths[0], true);
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

		InitiateMovement(availablePaths[0]);
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

	case DICE_ANIMATION:
		break;
	case END:
		m_data->players[m_data->currentPlayerIndex].sprite.SetAnimation("Idle");
		m_data->currentDiceVideo = m_data->diceVideos[TRANSITION_1];
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
		break;
	case INTRO:
		m_data->currentDiceVideo->update(_dt);
		if (m_data->currentDiceVideo->isFinish())
		{
			SetBoardState(START);
		}
		break;
	case START:
	{
		auto& player = m_data->players[m_data->currentPlayerIndex];
		m_data->currentDiceVideo->update(_dt);
		if (m_data->currentDiceVideo->isFinish())
		{
			m_data->diceAnimationPlaying = false;
			player.diceNumber.setString(std::to_string(player.startRandom));
			SortStart(_dt);
		}
		break;
	}

	case DICE_ANIMATION:
	{
		m_data->currentDiceVideo->update(_dt);
		if (m_data->currentDiceVideo->isFinish())
		{
			m_data->timeDice -= _dt;
			if (m_data->timeDice <= 0)
			{
				m_data->diceAnimationPlaying = false;
				auto& player = m_data->players[m_data->currentPlayerIndex];

				// Initialiser le mouvement restant avec le résultat du dé
				player.pendingMovement = m_data->diceResult;

				m_data->timeDice = TIME_DIS_DISPLAY;

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
					SetBoardState(DEPLACEMENT, 0);
				}
				else
				{
					player.tourstate = 0;
					player.state = StatePlayer::NONE;
					SetBoardState(DEPLACEMENT_BACK, 0);
				}
			}
		}
	}
	break;

	// États de mouvement simplifiés
	case DEPLACEMENT_SPLIT:
		[[fallthrough]];
	case DEPLACEMENT_BRIGE:
		[[fallthrough]];
	case DEPLACEMENT:
		HandleMovementState(DEPLACEMENT, _dt);
		break;

	case DEPLACEMENT_BACK:
		HandleMovementState(DEPLACEMENT_BACK, _dt);
		break;

	case DEPLACEMENT_ACTION:
		HandleMovementState(DEPLACEMENT_ACTION, _dt);
		break;

	case DEPLACEMENT_ACTION_BACK:
		HandleMovementState(DEPLACEMENT_ACTION_BACK, _dt);
		break;

	case DEPLACEMENT_ACTION_2:
		HandleMovementState(DEPLACEMENT_ACTION_2, _dt);
		break;

	case CASE_ACTION:
		CaseAction();
		break;

	case BATTLE_ACTION:
		m_data->currentDiceVideo->update(_dt);
		if (m_data->currentDiceVideo->isFinish())
		{
			m_gameData->InitMiniGamePlayer();
			for (int i = 0; i < m_data->players.size(); i++)
				m_gameData->AddPlayerPlaying(i);

			SetBoardState(WIN);
			m_gameData->m_renderWindow->ResetView();
			ChangeScene(RandomBattle(), true);
		}

		break;

	case DUEL:
		m_data->currentDiceVideo->update(_dt);
		if (m_data->currentDiceVideo->isFinish())
		{
			m_gameData->InitMiniGamePlayer();
			m_gameData->AddPlayerPlaying(m_data->currentPlayerIndex);
			m_gameData->AddPlayerPlaying(OnSameCase());

			SetBoardState(WIN);
			m_gameData->m_renderWindow->ResetView();
			ChangeScene(RandomDuel(), true);
		}

		break;

	case WIN:
		m_data->currentDiceVideo->update(_dt);
		if (m_data->currentDiceVideo->isFinish())
		{
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
				auto& playerWin = m_data->players[winnerIndex];
				auto& playerLose = m_data->players[loserIndex];
				playerWin.pendingMovement--;
				playerLose.pendingMovement--;

				if (playerWin.pendingMovement > 0 || playerLose.pendingMovement > 0)
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
	case END:
		m_data->currentDiceVideo->update(_dt);
		if (m_data->currentDiceVideo->isFinish())
		{
			m_gameData->m_renderWindow->ResetView();
			ChangeScene("Podium", false);
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
		/*"Basket",*/
		"FlagGame",
		"RandCard",
		"RuRoul"
	};

	// Nombre de mini-jeux disponibles
	const int miniGameCount = 5;

	// Sélection aléatoire d'un mini-jeu
	//int randomIndex = 2;
	int randomIndex = randmt::RandomInt(0, miniGameCount - 1);

	std::cout << "Random minigame selected: " << miniGames[randomIndex] << std::endl;


	//return "FlagGame";


	m_gameData->m_nextScene = miniGames[randomIndex];
	return "Warmup";
}

std::string BaseGame::RandomBattle()
{
	// Liste des mini-jeux disponibles
	const std::string miniGames[] = {
		/*"Basket",*/
		"FlagGame",
		"RandCard",
		"RuRoul"
	};

	const int miniGameCount = 3;
	int randomIndex = randmt::RandomInt(0, miniGameCount - 1);

	std::cout << "Random minigame selected: " << miniGames[randomIndex] << std::endl;

	m_gameData->m_nextScene = miniGames[randomIndex];
	return "Warmup";
}

