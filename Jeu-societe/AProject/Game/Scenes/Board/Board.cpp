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
	m_audioEngine = (AudioEngine*)m_gameData->m_audioEngine;

	progress.store(0.1f);

	m_data->state = INTRO;

	// Configuration des éléments UI
	m_data->HudLBM.text.setFont(*m_gameData->m_assetManager->GetAsset<sf::Font>("BoardFont", AssetManager::AssetType::FONT));
	m_data->HudLBM.sprite.setTexture(*m_gameData->m_assetManager->GetAsset<TextureAnimated>("Anim_card", AssetManager::AssetType::TEXTURE_ANIMATED));
	m_data->HudLBM.state = NONELBM;
	m_data->HudLBM.active = false;
	progress.store(0.4f);

	// Chargement de la carte
	//m_gameData->m_tile.InitTiled("Assets/Map/map.json");
	m_gameData->m_renderWindow->ResetView();
	m_data->camera.Reset(m_gameData->m_renderWindow->getDefaultView());
	progress.store(0.5f);

	MapLayer layer = m_gameData->m_tile->GetMapLayer("point");
	m_data->posCase = layer.GetObjects();

	m_data->players.resize(m_gameData->m_playerDataList.size());

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
	m_data->timerEnd = TIME_END_DISPLAY;

	// NOUVEAU : Initialisation de la vidéo du dé
	m_data->diceAnimationPlaying = false;
	m_data->diceResult = 0;
	m_data->dicePosition = sf::Vector2f(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f);

	// NOUVEAU : Pré-charger toutes les vidéos des dés
	m_data->diceVideos.clear(); // S'assurer que le vecteur est vide
	m_data->diceVideos.resize(8); // Initialiser avec des nullptr

	// Configuration simple sans taille personnalisée
	HighResConfig config;
	config.sizeMode = VideoSizeMode::Original;
	config.enableLoop = false;

	for (int i = 0; i < 6; i++)
	{
		try
		{
			m_data->diceVideos[i] = new HighResVideoPlayer(config);
			std::string videoPath = "Assets/Video/De" + std::to_string(i + 1) + ".mov";
			if (!m_data->diceVideos[i]->loadFromFile(videoPath))
			{
				std::cout << "Erreur : Impossible de charger la video du de: " << videoPath << std::endl;
				delete m_data->diceVideos[i];
				m_data->diceVideos[i] = nullptr;
			}
		}
		catch (const std::exception& e)
		{
			std::cout << "Exception lors du chargement vidéo : " << e.what() << std::endl;
			if (m_data->diceVideos[i] != nullptr)
			{
				delete m_data->diceVideos[i];
				m_data->diceVideos[i] = nullptr;
			}
		}
	}

	try
	{
		m_data->diceVideos[TRANSITION_1] = new HighResVideoPlayer(config);
		m_data->diceVideos[TRANSITION_2] = new HighResVideoPlayer(config);
		m_data->diceVideos[TRANSITION_1]->loadFromFile("Assets/Video/TRANSITION_1_LOUIS_VERSION.mp4");
		m_data->diceVideos[TRANSITION_2]->loadFromFile("Assets/Video/TRANSITION_2_LOUIS_VERSION.mp4");
	}
	catch (const std::exception& e)
	{
		std::cout << "Exception lors du chargement des transitions : " << e.what() << std::endl;
	}

	m_data->currentDiceVideo = m_data->diceVideos[TRANSITION_1];
	if (m_data->currentDiceVideo != nullptr)
	{
		m_data->currentDiceVideo->play();
		m_data->currentDiceVideo->update(1);
	}

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
		m_data->chromaKeyShader.setUniform("threshold", 0.7f); // Ajuster selon vos besoins
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
		m_data->players[i].v.setString(sf::String(L"\u2193"));
		m_data->players[i].boardPosition = m_data->posCase[0].GetPosition() + sf::Vector2f{ -40.f * i + 50 ,0.f };
		m_data->players[i].posIcone = PosIcone(i);

		sf::FloatRect textBounds = m_data->players[i].v.getLocalBounds();
		m_data->players[i].v.setOrigin({ 0.5f,0.5f });
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

		m_data->players[i].diceNumber.setOrigin({ 0.5, 1 });
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
			posMin = (mapObject.GetPosition() + sf::Vector2f(50, 0));
		}
		if (i == m_data->posCase.size() - 1)
		{
			posMax = mapObject.GetPosition();
		}
	}

	CreateSmokeEffectAnotherPart(posMin, posMax);
	progress.store(0.95f);

	// Configuration des animateurs
	m_data->animator.Modify(1.0f, 240.f, false, 1.0f);
	//m_data->animator.SetSpeed(0.5f);

	m_data->animator2.Modify(1.0f, 240.f, false, 1.0f);
	m_data->animator.SetAnimationEasing(anim::Animator::GOTO, anim::Easing::LINEAR);
	m_data->animator2.SetAnimationEasing(anim::Animator::GOTO, anim::Easing::LINEAR);
	m_data->animator.End();
	m_data->animator2.End();

	m_data->arrow.setTexture(*m_gameData->m_assetManager->GetAsset<sf::Texture>("Arrow", AssetManager::AssetType::TEXTURE));

	m_data->arrow.setOrigin({ 0 , m_data->arrow.getTexture()->getSize().y / 2.f });

	for (int i = 0; i < m_data->posCase.size(); i++)
	{
		auto& mapObject = m_data->posCase[i];
		if (mapObject.GetName() == "6")
		{
			m_data->posArrow[0] = (mapObject.GetPosition() + sf::Vector2f(150, -22));
		}
		if (mapObject.GetName() == "23")
		{
			m_data->posArrow[1] = (mapObject.GetPosition() + sf::Vector2f(150, -22));
		}

	}

	m_data->arrow.setScale({ 0.5f,0.5f });

	m_data->HudLBM.text.SetCharactersPerLine(25);

	m_data->texteDisplay.displayTime = 3.0f;
	m_data->texteDisplay.currentTime = 0.0f;
	m_data->texteDisplay.isActive = false;

	// NOUVEAU : Initialisation du fade
	m_data->texteDisplay.fadeState = TexteDisplay::FADE_NONE;
	m_data->texteDisplay.fadeInDuration = 0.5f;   // 0.5 seconde pour apparaître
	m_data->texteDisplay.fadeOutDuration = 0.5f;  // 0.5 seconde pour disparaître
	m_data->texteDisplay.fadeTimer = 0.0f;

	m_data->song1 = false;

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
	if (m_data != nullptr)
	{
		// Libérer les textes
		for (auto* texte : m_data->texteDisplay.texte)
		{
			delete texte;
		}
		m_data->texteDisplay.texte.clear();

		// 1. Arrêter l'audio stream AVANT de libérer les vidéos
		if (m_data->currentDiceVideo != nullptr)
		{
			m_data->currentDiceVideo->setPaused(true);
		}

		// 2. Nullifier currentDiceVideo pour éviter un double appel à close()
		m_data->currentDiceVideo = nullptr;

		// 3. Libérer TOUTES les vidéos du vecteur
		for (auto* video : m_data->diceVideos)
		{
			if (video != nullptr)
			{
				video->close();  // Ferme proprement les ressources FFmpeg
				delete video;    // Libère l'objet C++
			}
		}
		m_data->diceVideos.clear();

		// 4. Libérer les effets qui contiennent des sprites/textures
		m_data->effectSwap.clear();
		m_data->effectsMap.clear();
	}

	// 5. Supprimer le conteneur d'assets AVANT m_data
	if (m_gameData != nullptr && m_gameData->m_assetManager != nullptr)
	{
		m_gameData->m_assetManager->DeleteContainer("Board");
	}

	// 6. Libérer m_data en dernier (destructeur de SceneData sera appelé)
	delete this->m_data;
	this->m_data = nullptr;
	this->m_gameData = nullptr;
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
			//m_data->currentDiceVideo->update(1);
			m_data->diceAnimationPlaying = true;

			// Le numéro sera affiché après l'animation
			player.startRandom = rando;
			//player.diceNumber.setString(std::to_string(player.startRandom));
		}

		std::cout << "Place: " << rando << std::endl;
	}
}

// NOUVEAU : Méthode pour afficher un message à l'écran
void BaseGame::ShowTextDisplay(const std::string& message, float duration)
{
	// Nettoyer les textes précédents
	for (auto* texte : m_data->texteDisplay.texte)
	{
		delete texte;
	}
	m_data->texteDisplay.texte.clear();

	// Séparer le message en lignes
	std::vector<std::string> lines;
	std::string messageModified = message;
	size_t pos = 0;

	while ((pos = messageModified.find("\n")) != std::string::npos)
	{
		lines.push_back(messageModified.substr(0, pos));
		messageModified.erase(0, pos + 1);
	}
	lines.push_back(messageModified);

	float startY = SCREEN_HEIGHT / 2.0f - ((float)lines.size() - 1.0f) * 60.0f;
	const sf::Font& font = *m_gameData->m_assetManager->GetAsset<sf::Font>("BoardFont", AssetManager::AssetType::FONT);

	for (size_t i = 0; i < lines.size(); i++)
	{
		TextPlus* texte = new TextPlus(); // Allouer dynamiquement
		texte->setFont(font);
		texte->setCharacterSize(90);
		texte->setFillColor(sf::Color(255, 255, 255, 0));
		texte->setOutlineColor(sf::Color(0, 0, 0, 0));
		texte->setOutlineThickness(3.f);
		texte->setOrigin({ 0.5f, 0.5f });
		texte->setPosition(SCREEN_WIDTH / 2.0f, startY + (float)i * 120.0f);
		//texte->SetAlignement(TextPlus::Alignement::CENTER);
		texte->SetCharactersPerLine(40);
		texte->setString(lines[i]);

		m_data->texteDisplay.texte.push_back(texte);
	}

	m_data->texteDisplay.displayTime = duration;
	m_data->texteDisplay.currentTime = 0.0f;
	m_data->texteDisplay.isActive = true;
	m_data->texteDisplay.fadeState = TexteDisplay::FADE_IN;
	m_data->texteDisplay.fadeTimer = 0.0f;
}

void BaseGame::PollEvent(sf::Event& _event)
{
	// ==================== DEBUG KEYS ====================
	if (_event.type == sf::Event::KeyPressed)
	{
		// DEBUG: Touche R - Swap aléatoire (déjà existant)
		if (_event.key.code == sf::Keyboard::R)
		{
			int swapIndex = randmt::RandomInt(0, (int)m_data->players.size() - 1);
			while (swapIndex == m_data->currentPlayerIndex)
			{
				swapIndex = randmt::RandomInt(0, (int)m_data->players.size() - 1);
			}

			SwapPlayers(swapIndex);
		}

		// DEBUG: Touche Num1 - Lancer Rock Paper Scissors
		if (_event.key.code == sf::Keyboard::Numpad1)
		{
			std::cout << "[DEBUG] Lancement Rock Paper Scissors" << std::endl;
			m_gameData->InitMiniGamePlayer();
			m_gameData->AddPlayerPlaying(m_data->currentPlayerIndex);
			m_gameData->AddPlayerPlaying((m_data->currentPlayerIndex + 1) % m_data->players.size());
			m_gameData->m_nextScene = "rockPaperSizor";
			m_gameData->m_renderWindow->ResetView();
			m_data->song1 = false;
			ChangeScene("Warmup", true);
			return;
		}

		// DEBUG: Touche Num2 - Lancer Arm Wrestling
		if (_event.key.code == sf::Keyboard::Numpad2)
		{
			std::cout << "[DEBUG] Lancement Arm Wrestling" << std::endl;
			m_gameData->InitMiniGamePlayer();
			m_gameData->AddPlayerPlaying(m_data->currentPlayerIndex);
			m_gameData->AddPlayerPlaying((m_data->currentPlayerIndex + 1) % m_data->players.size());
			m_gameData->m_nextScene = "ArmWrestling";
			m_gameData->m_renderWindow->ResetView();
			m_data->song1 = false;
			ChangeScene("Warmup", true);
			return;
		}

		// DEBUG: Touche Num3 - Lancer Flag Game
		if (_event.key.code == sf::Keyboard::Numpad3)
		{
			std::cout << "[DEBUG] Lancement Flag Game" << std::endl;
			m_gameData->InitMiniGamePlayer();
			for (int i = 0; i < m_data->players.size(); i++)
				m_gameData->AddPlayerPlaying(i);
			m_gameData->m_nextScene = "FlagGame";
			m_gameData->m_renderWindow->ResetView();
			m_data->song1 = false;
			ChangeScene("Warmup", true);
			return;
		}

		// DEBUG: Touche Num4 - Lancer Rand Card
		if (_event.key.code == sf::Keyboard::Numpad4)
		{
			std::cout << "[DEBUG] Lancement Rand Card" << std::endl;
			m_gameData->InitMiniGamePlayer();
			for (int i = 0; i < m_data->players.size(); i++)
				m_gameData->AddPlayerPlaying(i);
			m_gameData->m_nextScene = "RandCard";
			m_gameData->m_renderWindow->ResetView();
			m_data->song1 = false;
			ChangeScene("Warmup", true);
			return;
		}

		// DEBUG: Touche Num5 - Lancer Roulette Russe
		if (_event.key.code == sf::Keyboard::Numpad5)
		{
			std::cout << "[DEBUG] Lancement Roulette Russe" << std::endl;
		 m_gameData->InitMiniGamePlayer();
			for (int i = 0; i < m_data->players.size(); i++)
				m_gameData->AddPlayerPlaying(i);
			m_gameData->m_nextScene = "RuRoul";
			m_gameData->m_renderWindow->ResetView();
			m_data->song1 = false;
			ChangeScene("Warmup", true);
			return;
		}

		// DEBUG: Touche Num6 - Forcer un 6 au prochain lancer (fonctionne aussi pour bridge et end)
		if (_event.key.code == sf::Keyboard::Numpad6)
		{
			std::cout << "[DEBUG] Prochain lancer forcé à 6" << std::endl;
			if (m_data->state != DICE_ANIMATION && m_data->state != WIN && 
				m_data->state != WAITING_BRIDGE_ROLL && m_data->state != WAITING_FIN_ROLL &&
				m_data->state != DUEL && m_data->state != BATTLE_ACTION)
			{
				ProcessDiceRoll(6);
			}
			else if (m_data->state == WAITING_BRIDGE_ROLL)
			{
				ProcessBridgeRollD();
			}
			else if (m_data->state == WAITING_FIN_ROLL)
			{
				ProcessFinRollD();
			}
			return;
		}

		// DEBUG: Touche T - Téléporter après le pont (case 19)
		if (_event.key.code == sf::Keyboard::T)
		{
			std::cout << "[DEBUG] Téléportation après le pont" << std::endl;
			auto& player = m_data->players[m_data->currentPlayerIndex];

			// Trouver la case après le pont (case 19)
			for (int i = 0; i < m_data->posCase.size(); i++)
			{
				auto& mapObject = m_data->posCase[i];
				if (mapObject.GetName() == "20") // Case juste après le pont
				{
					player.currentCaseIndex = i;
					player.boardPosition = mapObject.GetPosition() + sf::Vector2f{ 0.f, 0.f };
					player.pendingMovement = 0;
					player.currentPathId = -1;

					// CORRECTION : Activer les effets de fumée
					m_data->smokeOff = true;

					// Transformer le personnage si ce n'est pas déjà fait
					switch (m_gameData->m_playerDataList[m_data->currentPlayerIndex].GetPlayerSkin())
					{
					case PlayerData::CHARACTER_1_1:
						m_gameData->m_playerDataList[m_data->currentPlayerIndex].SetPlayerSkin(PlayerData::CHARACTER_1_2);
						player.texture = *m_gameData->m_assetManager->GetAsset<TextureAnimated>("Perso1-2", AssetManager::AssetType::TEXTURE_ANIMATED);
						break;
					case PlayerData::CHARACTER_2_1:
						m_gameData->m_playerDataList[m_data->currentPlayerIndex].SetPlayerSkin(PlayerData::CHARACTER_2_2);
						player.texture = *m_gameData->m_assetManager->GetAsset<TextureAnimated>("Perso2-2", AssetManager::AssetType::TEXTURE_ANIMATED);
						break;
					case PlayerData::CHARACTER_3_1:
						m_gameData->m_playerDataList[m_data->currentPlayerIndex].SetPlayerSkin(PlayerData::CHARACTER_3_2);
						player.texture = *m_gameData->m_assetManager->GetAsset<TextureAnimated>("Perso3-2", AssetManager::AssetType::TEXTURE_ANIMATED);
						break;
					case PlayerData::CHARACTER_4_1:
						m_gameData->m_playerDataList[m_data->currentPlayerIndex].SetPlayerSkin(PlayerData::CHARACTER_4_2);
						player.texture = *m_gameData->m_assetManager->GetAsset<TextureAnimated>("Perso4-2", AssetManager::AssetType::TEXTURE_ANIMATED);
						break;
					default:
						break;
					}

					player.sprite.setTexture(player.texture);
					player.sprite.SetAnimation("Idle");

					//ShowTextDisplay("DEBUG: Teleported after bridge!", 2.0f);
					UpdateCameraToShowAllPlayers();
					break;
				}
			}
			return;
		}

		// DEBUG: Touche B - Téléporter directement sur le pont
		if (_event.key.code == sf::Keyboard::B)
		{
			std::cout << "[DEBUG] Téléportation sur le pont" << std::endl;
			auto& player = m_data->players[m_data->currentPlayerIndex];

			// Trouver la case du pont (case 19)
			for (int i = 0; i < m_data->posCase.size(); i++)
			{
				auto& mapObject = m_data->posCase[i];
				if (mapObject.GetName() == "19")
				{
					player.currentCaseIndex = i;
					player.boardPosition = mapObject.GetPosition() + sf::Vector2f{ 0.f, 0.f };
					player.pendingMovement = 0;
					player.currentPathId = -1;
					player.sprite.SetAnimation("Idle");

					//ShowTextDisplay("DEBUG: Teleported to bridge!", 2.0f);
					UpdateCameraToShowAllPlayers();
					break;
				}
			}
			return;
		}

		// DEBUG: Touche F - Téléporter à la ligne d'arrivée
		if (_event.key.code == sf::Keyboard::F)
		{
			std::cout << "[DEBUG] Téléportation à la ligne d'arrivée" << std::endl;
			auto& player = m_data->players[m_data->currentPlayerIndex];

			// Trouver la case de fin
			for (int i = 0; i < m_data->posCase.size(); i++)
			{
				auto& mapObject = m_data->posCase[i];
				if (mapObject.GetPropertyByName("type") != nullptr)
				{
					if (mapObject.GetPropertyByName("type")->GetStringValue() == "end")
					{
						player.currentCaseIndex = i;
						player.boardPosition = mapObject.GetPosition() + sf::Vector2f{ 0.f, 0.f };
						player.pendingMovement = 0;
						player.currentPathId = -1;
						player.sprite.SetAnimation("Idle");

						//ShowTextDisplay("DEBUG: Teleported to finish line!", 2.0f);
						UpdateCameraToShowAllPlayers();
						break;
					}
				}
			}
			return;
		}
	}
	// ==================== FIN DEBUG KEYS ====================

	// Gestion simplifiée des états d'attente de lancer
	if (m_data->state == WAITING_FIN_ROLL and !m_data->texteDisplay.isActive)
	{
		bool shouldRoll = false;
		if (CheckPlayerInput(_event, shouldRoll) && shouldRoll)
		{
			ProcessFinRoll();
		}
		return;
	}

	if (m_data->state == WAITING_BRIDGE_ROLL and !m_data->texteDisplay.isActive)
	{
		bool shouldRoll = false;
		if (CheckPlayerInput(_event, shouldRoll) && shouldRoll)
		{
			ProcessBridgeRoll();
		}
		return;
	}

	// Si on attend un choix de chemin
	if (m_data->state == WAITING_PATH_CHOICE and !m_data->texteDisplay.isActive)
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
						m_data->arrow.setRotation(325);
						std::cout << "Top path selected (joystick)" << std::endl;
					}
					else if (_event.joystickMove.position > 50.0f) // Bas
					{
						ProcessPathChoice(1);
						m_data->arrow.setRotation(5);
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
				m_data->arrow.setRotation(325);
				std::cout << "Top path selected (keyboard)" << std::endl;
			}
			else if (_event.key.code == sf::Keyboard::Down || _event.key.code == sf::Keyboard::S)
			{
				ProcessPathChoice(1);
				m_data->arrow.setRotation(5);
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
	if (_event.type == sf::Event::JoystickButtonPressed and !m_data->texteDisplay.isActive)
	{
		if (m_gameData->m_playerDataList[m_data->currentPlayerIndex].m_joystickId == _event.joystickButton.joystickId 
			&& m_data->animator.IsFinished() && m_data->currentDiceVideo->isFinish() &&
			m_data->state != WAITING_BRIDGE_ROLL && m_data->state != WAITING_FIN_ROLL &&
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
	if (_event.type == sf::Event::KeyPressed and !m_data->texteDisplay.isActive)
	{
		if (_event.key.code == sf::Keyboard::Space && m_data->animator.IsFinished() && m_data->currentDiceVideo->isFinish()
			&& m_data->state != WAITING_BRIDGE_ROLL && m_data->state != WAITING_FIN_ROLL &&
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
	// CORRECTION : Toujours mettre à jour les transitions audio
	//m_audioEngine->UpdateMusicTransition(_deltaTime);

	// Gestion des transitions musicales entre Plato1 et Plato2
	// Condition : joueur actuel a dépassé le pont (position X > case 19 + 100)
	if (m_data->players[m_data->currentPlayerIndex].boardPosition.x > m_data->posCase[19].GetPosition().x + 100 )
	{
		// Ne déclencher la transition que si elle est terminée ET que la musique actuelle n'est pas déjà "Plato2"
		if (m_audioEngine->IsTransitionFinished() && m_audioEngine->GetMusicName() != "Plato2")
		{
			std::cout << "Transition vers Plato2 depuis : " << m_audioEngine->GetMusicName() << std::endl;
			m_audioEngine->PlayMusicTransition("Plato2", true, false, 5.0f, FADED_MIX);

			// Transformation du personnage
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
				break;
			}

			m_data->smokeOff = true;
			m_data->song1 = false;
		}
	}
	// Condition : joueur actuel est avant ou sur le pont (position X <= case 19 + 100)
	else if (!m_data->song1 && m_data->players[m_data->currentPlayerIndex].boardPosition.x <= m_data->posCase[19].GetPosition().x + 100)
	{
		// Ne déclencher la transition que si elle est terminée ET que la musique actuelle n'est pas déjà "Plato1"
		if (m_audioEngine->IsTransitionFinished() && m_audioEngine->GetMusicName() != "Plato1")
		{
			std::cout << "Transition vers Plato1 depuis : " << m_audioEngine->GetMusicName() << std::endl;
			m_audioEngine->PlayMusicTransition("Plato1", true, false, 5.0f, FADED_MIX);
			m_data->song1 = true;
		}
	}
	std::cout << "Transition vers Plato1 depuis : " << m_audioEngine->GetMusicName() << std::endl;
	// Mise à jour des animations
	UpdateLBM(_deltaTime);

	// NOUVEAU : Mise à jour du texte d'affichage avec fade
	if (m_data->texteDisplay.isActive)
	{
		m_data->texteDisplay.fadeTimer += _deltaTime;

		switch (m_data->texteDisplay.fadeState)
		{
		case TexteDisplay::FADE_IN:
		{
			// Calculer l'alpha basé sur le temps écoulé
			float alpha = std::min(1.0f, m_data->texteDisplay.fadeTimer / m_data->texteDisplay.fadeInDuration);

			// Appliquer l'alpha à tous les textes
			for (auto* texte : m_data->texteDisplay.texte)
			{
				sf::Color fillColor = texte->getFillColor();
				fillColor.a = static_cast<sf::Uint8>(255 * alpha);
				texte->setFillColor(fillColor);

				sf::Color outlineColor = texte->getOutlineColor();
				outlineColor.a = static_cast<sf::Uint8>(255 * alpha);
				texte->setOutlineColor(outlineColor);
			}

			// Passer à l'état FADE_DISPLAY quand le fade in est terminé
			if (m_data->texteDisplay.fadeTimer >= m_data->texteDisplay.fadeInDuration)
			{
				m_data->texteDisplay.fadeState = TexteDisplay::FADE_DISPLAY;
				m_data->texteDisplay.currentTime = 0.0f;
			}
			break;
		}

		case TexteDisplay::FADE_DISPLAY:
		{
			// Affichage complet (alpha = 255)
			m_data->texteDisplay.currentTime += _deltaTime;

			// Démarrer le fade out quand le temps d'affichage est presque écoulé
			if (m_data->texteDisplay.currentTime >= m_data->texteDisplay.displayTime - m_data->texteDisplay.fadeOutDuration)
			{
				m_data->texteDisplay.fadeState = TexteDisplay::FADE_OUT;
				m_data->texteDisplay.fadeTimer = 0.0f;
			}
			break;
		}

		case TexteDisplay::FADE_OUT:
		{
			// Calculer l'alpha inversé (de 1.0 à 0.0)
			float alpha = 1.0f - std::min(1.0f, m_data->texteDisplay.fadeTimer / m_data->texteDisplay.fadeOutDuration);

			// Appliquer l'alpha à tous les textes
			for (auto* texte : m_data->texteDisplay.texte)
			{
				sf::Color fillColor = texte->getFillColor();
				fillColor.a = static_cast<sf::Uint8>(255 * alpha);
				texte->setFillColor(fillColor);

				sf::Color outlineColor = texte->getOutlineColor();
				outlineColor.a = static_cast<sf::Uint8>(255 * alpha);
				texte->setOutlineColor(outlineColor);
			}

			// Désactiver quand le fade out est terminé
			if (m_data->texteDisplay.fadeTimer >= m_data->texteDisplay.fadeOutDuration)
			{
				m_data->texteDisplay.isActive = false;
				m_data->texteDisplay.fadeState = TexteDisplay::FADE_NONE;
				m_data->texteDisplay.currentTime = 0.0f;
				m_data->texteDisplay.fadeTimer = 0.0f;
				m_data->texteDisplay.texte.clear();
			}
			break;
		}

		case TexteDisplay::FADE_NONE:
		default:
			break;
		}
	}

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
		m_data->animator.SetSpeed(1.f);
		m_data->animator.SetDuration(1.f);

		std::cout << "Mouvement restant : " << player.pendingMovement << std::endl;

		if (player.pendingMovement > 0)
		{
			// CORRECTION : Ne pas vérifier les conditions immédiatement
			// Initier directement le prochain mouvement
			std::vector<int> availablePaths;

			// Déterminer les chemins selon l'état
			if (state == DEPLACEMENT_BACK || state == DEPLACEMENT_ACTION_BACK)
			{
				availablePaths = GetAvailablePathsBack(player.currentCaseIndex);
			}
			else
			{
				availablePaths = GetAvailablePaths(player.currentCaseIndex);
			}

			if (availablePaths.empty())
			{
				std::cout << "Erreur : aucun chemin disponible!" << std::endl;
				SetBoardState(CASE_ACTION_END);
				return;
			}

			// Vérifier s'il y a un choix de chemin SEULEMENT si le personnage s'arrête
			const MapObject& currentCase = m_data->posCase[player.currentCaseIndex];

			// Vérifier le type de case
			std::string caseType = "";
			if (currentCase.GetPropertyByName("type") != nullptr)
			{
				caseType = currentCase.GetPropertyByName("type")->GetStringValue();
			}

			// Vérifier pont ou ligne d'arrivée
			if (caseType == "bridge" && (state != DEPLACEMENT_BACK && state != DEPLACEMENT_ACTION_BACK && state != DEPLACEMENT_ACTION))
			{
				player.sprite.SetAnimation("Idle");
				std::cout << "Pont détecté ! Lancez le dé pour traverser..." << std::endl;
				player.waitingBridgeRoll = true;
				SetBoardState(WAITING_BRIDGE_ROLL);
				return;
			}

			if (caseType == "end" && (state != DEPLACEMENT_BACK && state != DEPLACEMENT_ACTION_BACK && state != DEPLACEMENT_ACTION))
			{
				player.sprite.SetAnimation("Idle");
				std::cout << "Ligne d'arrivée détectée ! Lancez le dé pour franchir..." << std::endl;
				player.waitingBridgeRoll = true;
				SetBoardState(WAITING_FIN_ROLL);
				return;
			}

			// Vérifier choix de chemin
			if (HasPathChoice(player.currentCaseIndex) && (state != DEPLACEMENT_BACK && state != DEPLACEMENT_ACTION_BACK && state != DEPLACEMENT_ACTION))
			{
				m_data->pathChoices = availablePaths;
				SetBoardState(WAITING_PATH_CHOICE);
				std::cout << "Choix de chemin requis : " << m_data->pathChoices.size() << " options" << std::endl;
				player.sprite.SetAnimation("Idle");
				return;
			}

			// Sinon continuer le mouvement automatiquement
			int nextIndex = availablePaths[0];

			// Si un chemin spécifique est sélectionné
			if (player.currentPathId != -1 && availablePaths.size() > 1)
			{
				nextIndex = availablePaths[player.currentPathId - 1];
			}

			bool backwards = (state == DEPLACEMENT_BACK || state == DEPLACEMENT_ACTION_BACK);
			InitiateMovement(nextIndex, backwards);
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

	if (m_data->state == WAITING_PATH_CHOICE)
	{
		for (size_t i = 0; i < 2; i++)
		{
			m_data->arrow.setPosition(m_data->posArrow[i]);
			mod->draw(m_data->arrow);
		}
	}

	m_gameData->m_renderWindow->ResetView();
	for (int i = 0; i < m_data->players.size(); i++)
	{
		DrawIconePlayer(*mod, i);
	}

	// NOUVEAU : Afficher le texte d'information
	if (m_data->texteDisplay.isActive)
	{
		for (const auto* texte : m_data->texteDisplay.texte)
		{
			mod->draw(*texte);
		}
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
	m_data->players[m_data->currentPlayerIndex].sprite.SetAnimation("Idle");
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
		ShowTextDisplay("Roll the dice to determine turn order!\nPress A to roll it", 4.0f);
		break;

	case PLAY:
		m_data->players[m_data->currentPlayerIndex].sprite.setScale({ 1.f,1.f });
		m_data->players[m_data->currentPlayerIndex].sprite.SetAnimation("Idle");
		m_data->pathChoices.clear();
		m_data->currentPlayerIndex = (m_data->currentPlayerIndex + 1) % m_data->players.size();
		//m_data->arrow.setRotation(315);
		ShowTextDisplay(m_data->players[m_data->currentPlayerIndex].playeur.getString() + " turn!\nPress A to roll the dice", 2.5f);
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
			SetBoardState(WAITING_PATH_CHOICE);
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
		ShowTextDisplay("Choose your path!\nMove joystick Up or Down", 3.0f);
		break;

	case WAITING_BRIDGE_ROLL:
		ShowTextDisplay("Bridge ahead!\nRoll the dice to cross\n(Need 4 or more)", 3.5f);
		break;

	case WAITING_FIN_ROLL:
		ShowTextDisplay("Finish line!\nRoll the dice to win\n(Need 5 or 6)", 3.5f);
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

	// NOUVEAU : Animation du dé sur le pont avec logique du PathManagement
	case DICE_ANIMATION_BRIDGE:
	{
		m_data->currentDiceVideo->update(_dt);
		if (m_data->currentDiceVideo->isFinish())
		{
			m_data->timeDice -= _dt;
			if (m_data->timeDice <= 0)
			{
				m_data->diceAnimationPlaying = false;
				auto& player = m_data->players[m_data->currentPlayerIndex];

				// UTILISE LA MÊME LOGIQUE QUE PathManagement.cpp
			   // Le joueur peut continuer si le dé est > 3 (comme dans ProcessBridgeRoll original)
				if (m_data->diceResult > 3)
				{
					std::cout << "Traversée réussie !" << std::endl;
					ShowTextDisplay("Bridge crossed successfully!\nTransformation...", 2.0f);
					m_data->smokeOff = true;

					m_data->animator.SetSpeed(0.35f);
					//m_data->animator.SetDuration(0.5f);

					// Change le skin du personnage (transformation)
					switch (m_gameData->m_playerDataList[m_data->currentPlayerIndex].GetPlayerSkin())
					{
					case PlayerData::CHARACTER_1_1:
						[[fallthrough]];
					case PlayerData::CHARACTER_1_2:
						m_gameData->m_playerDataList[m_data->currentPlayerIndex].SetPlayerSkin(PlayerData::CHARACTER_1_2);
						m_data->players[m_data->currentPlayerIndex].texture = *m_gameData->m_assetManager->GetAsset<TextureAnimated>("Perso1-2", AssetManager::AssetType::TEXTURE_ANIMATED);
						break;
					case PlayerData::CHARACTER_2_1:
						[[fallthrough]];
					case PlayerData::CHARACTER_2_2:
						m_gameData->m_playerDataList[m_data->currentPlayerIndex].SetPlayerSkin(PlayerData::CHARACTER_2_2);
						m_data->players[m_data->currentPlayerIndex].texture = *m_gameData->m_assetManager->GetAsset<TextureAnimated>("Perso2-2", AssetManager::AssetType::TEXTURE_ANIMATED);
						break;
					case PlayerData::CHARACTER_3_1:
						[[fallthrough]];
					case PlayerData::CHARACTER_3_2:
						m_gameData->m_playerDataList[m_data->currentPlayerIndex].SetPlayerSkin(PlayerData::CHARACTER_3_2);
						m_data->players[m_data->currentPlayerIndex].texture = *m_gameData->m_assetManager->GetAsset<TextureAnimated>("Perso3-2", AssetManager::AssetType::TEXTURE_ANIMATED);
						break;
					case PlayerData::CHARACTER_4_1:
						[[fallthrough]];
					case PlayerData::CHARACTER_4_2:
						m_gameData->m_playerDataList[m_data->currentPlayerIndex].SetPlayerSkin(PlayerData::CHARACTER_4_2);
						m_data->players[m_data->currentPlayerIndex].texture = *m_gameData->m_assetManager->GetAsset<TextureAnimated>("Perso4-2", AssetManager::AssetType::TEXTURE_ANIMATED);
						break;
					default:
						m_gameData->m_playerDataList[m_data->currentPlayerIndex].SetPlayerSkin(PlayerData::CHARACTER_1_2);
						m_data->players[m_data->currentPlayerIndex].texture = *m_gameData->m_assetManager->GetAsset<TextureAnimated>("Perso1-1", AssetManager::AssetType::TEXTURE_ANIMATED);
						break;
					}

					// Applique la nouvelle texture au sprite
					m_data->players[m_data->currentPlayerIndex].sprite.setTexture(m_data->players[m_data->currentPlayerIndex].texture);

					// Continue le déplacement si le joueur a encore du mouvement
					if (player.pendingMovement > 0)
					{
						SetBoardState(DEPLACEMENT_BRIGE);
					}
					else
					{
						SetBoardState(CASE_ACTION);
					}
				}
				else
				{
					std::cout << "Échec ! Vous ne pouvez pas traverser le pont." << std::endl;
					ShowTextDisplay("Failed to cross the bridge!\nTry again next turn", 2.5f);
					if (!player.firstTime)
					{
						player.pendingMovement = 0;
						player.firstTime = true;
						SetBoardState(CASE_ACTION);
					}
					else
					{
						player.pendingMovement = 0;
						SetBoardState(CASE_ACTION_END);
					}
				}

				m_data->timeDice = TIME_DIS_DISPLAY;
			}
		}
	}
	break;

	// NOUVEAU : Animation du dé sur la ligne d'arrivée avec logique du PathManagement
	case DICE_ANIMATION_END:
	{
		m_data->currentDiceVideo->update(_dt);
		if (m_data->currentDiceVideo->isFinish())
		{
			m_data->timeDice -= _dt;
			if (m_data->timeDice <= 0)
			{
				m_data->diceAnimationPlaying = false;
				auto& player = m_data->players[m_data->currentPlayerIndex];

				// UTILISE LA MÊME LOGIQUE QUE PathManagement.cpp
			   // Le joueur gagne si le dé est > 4 (comme dans ProcessFinRoll original)
				if (m_data->diceResult > 4)
				{
					std::cout << "VICTOIRE !" << std::endl;
					ShowTextDisplay("Crossed the finish line!\nPlayer " + std::to_string(m_data->currentPlayerIndex + 1) + " advances!", 3.0f);

					// Ajoute le joueur à la liste des gagnants
					m_gameData->m_gonnaPlayIndex.push_back(m_data->currentPlayerIndex);

					// Prépare la liste des joueurs triés par position X décroissante
					std::vector<std::pair<int, float>> playerPositions;
					for (int i = 0; i < m_data->players.size();
						i++)
					{
						playerPositions.push_back({ i, m_data->players[i].boardPosition.x });
					}

					// Trier par position X décroissante (les plus avancés en premier)
					std::sort(playerPositions.begin(), playerPositions.end(),
						[](const std::pair<int, float>& a, const std::pair<int, float>& b) {
							return a.second > b.second;
						});

					// Remplir avec le gagnant en premier, puis les autres
					m_gameData->m_gonnaPlayIndex.clear();
					m_gameData->m_gonnaPlayIndex.push_back(m_data->currentPlayerIndex);

					for (const auto& playerPos : playerPositions)
					{
						if (playerPos.first != m_data->currentPlayerIndex)
						{
							m_gameData->m_gonnaPlayIndex.push_back(playerPos.first);
						}
					}

					// Vérifier s'il reste des joueurs
					// Tous les joueurs sauf un ont fini
					SetBoardState(END);
				}
				else
				{
					std::cout << "Échec !" << std::endl;
					ShowTextDisplay("Not enough to finish!\nTry again", 2.5f);
					if (!player.firstTime)
					{
						player.pendingMovement = 0;
						player.firstTime = true;
						SetBoardState(CASE_ACTION);
					}
					else
					{
						player.pendingMovement = 0;
						SetBoardState(CASE_ACTION_END);
					}
				}

				m_data->timeDice = TIME_WIN_DISPLAY;
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
			m_data->song1 = false;
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
			m_data->song1 = false;
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
		m_data->timerEnd -= _dt;
		if (m_data->timerEnd <= 0)
		{
			if (!m_data->endActive)
			{
				m_data->endActive = true;
				m_data->currentDiceVideo = m_data->diceVideos[TRANSITION_2];
				m_data->currentDiceVideo->play();
			}

			m_data->currentDiceVideo->update(_dt);
			if (m_data->currentDiceVideo->isFinish())
			{
				m_gameData->m_renderWindow->ResetView();
				ChangeScene("Podium", false);
			}
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
	//m_gameData->m_nextScene = "RandCard";

	return "Warmup";
}

