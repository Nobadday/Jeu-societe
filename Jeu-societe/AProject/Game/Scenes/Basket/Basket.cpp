#include "Basket.hpp"

// Basket Scene Implementation
void Basket::Load(void)
{
	m_data = new SceneData();
	m_data->gameData = (GameData*)this->m_keptData;
	
	m_data->gameData->m_assetManager->LoadManifest("Manifests/Basket.json", "basket");

	m_data->gameData->m_assetManager->GetAsset<sf::Font>()
	m_data->timerText.setFont(*m_data->gameData->m_assetManager->GetAsset<sf::Font>("BasketFont"));
	m_data->timerText.setCharacterSize(25);
	m_data->timerText.setFillColor(sf::Color::Yellow);
	m_data->timerText.setString("Basket Scene");
	m_data->timerText.setOrigin(m_data->timerText.getLocalBounds().width / 2.f, 0);
	m_data->timerText.setPosition(SCREEN_WIDTH / 2.f, 0);

	m_data->winnerText.setFont(*m_data->gameData->m_assetManager->GetAsset<sf::Font>("BasketFont"));
	m_data->winnerText.setCharacterSize(30);
	m_data->winnerText.setFillColor(sf::Color::Green);

	m_data->state = STATE_PLAYING;
	m_data->timer = 15.f;
	m_data->winnerCount = 0;  // NOUVEAU: Initialiser le compteur

	if (((GameData*)this->m_keptData)->m_gonnaPlayIndex.size() == 0)
	{
		((GameData*)this->m_keptData)->m_gonnaPlayIndex.push_back(0);
		((GameData*)this->m_keptData)->m_gonnaPlayIndex.push_back(1);
	}

	// Créer les joueurs basés sur m_gonnaPlayIndex
	for (size_t i = 0; i < m_data->gameData->m_gonnaPlayIndex.size(); ++i)
	{
		int playerIndex = m_data->gameData->m_gonnaPlayIndex[i];
		
		BasketPlayer player;
		player.m_id = playerIndex;
		player.m_won = false;
		player.m_winOrder = -1;  // NOUVEAU: -1 = n'a pas encore gagné
		player.m_aimLine = sf::VertexArray(sf::Lines, 2);

		player.m_ballSprite.setTexture(*m_data->gameData->m_assetManager->GetAsset<sf::Texture>("ball"));
		player.m_hoopSprite.setTexture(*m_data->gameData->m_assetManager->GetAsset<sf::Texture>("hoop"));

		float spacing = SCREEN_WIDTH / ((float)m_data->gameData->m_gonnaPlayIndex.size() + 1);
		short randY = std::rand() % 400;

		player.m_ballSprite.setPosition((i + 1) * spacing, SCREEN_HEIGHT - 300.f);
		player.m_ballSprite.setOrigin(player.m_ballSprite.getLocalBounds().width / 2.f, 
			player.m_ballSprite.getLocalBounds().height / 2.f);

		player.m_hoopSprite.setPosition((i + 1) * spacing, (float)randY);
		player.m_hoopSprite.setOrigin(player.m_hoopSprite.getLocalBounds().width / 2.f, 
			player.m_hoopSprite.getLocalBounds().height / 2.f);

		player.m_ballShape.setRadius(player.m_ballSprite.getLocalBounds().width / 2.f);
		player.m_ballShape.setOrigin(player.m_ballShape.getRadius(), player.m_ballShape.getRadius());
		player.m_ballShape.setPosition(player.m_ballSprite.getPosition());
		player.m_ballShape.setFillColor(sf::Color::Transparent);

		m_data->players.push_back(player);
	}
}

void Basket::Unload(void)
{
	m_data->gameData->m_assetManager->DeleteContainer("basket");
	delete m_data;
	m_data = nullptr;

}

void Basket::PollEvent(sf::Event& _event)
{
	if (m_data->state == STATE_PLAYING)
	{
		// Les joueurs peuvent jouer pendant la phase de jeu
	}
	else if (m_data->state == STATE_GAMEOVER)
	{
		if (_event.type == sf::Event::JoystickButtonPressed)
		{
			if (_event.joystickButton.button == GAMEPAD_A)
			{
				// Retour au Board avec les résultats
				ChangeScene("Board", false);
			}
		}
	}
}

void Basket::Update(float _deltaTime)
{
	char buffer[100];
	std::snprintf(buffer, 100, "Timer %.2f", m_data->timer);
	m_data->timerText.setString(buffer);

	if (m_data->state == STATE_PLAYING)
	{
		m_data->timer -= _deltaTime;

		if (m_data->timer <= 0.f)
		{
			m_data->timer = 0.f;
			m_data->state = STATE_GAMEOVER;

			// Trier les joueurs par ordre de victoire et mettre à jour m_winIndex
			m_data->gameData->m_winIndex.clear();
			
			// Créer un vecteur de paires (winOrder, playerID) pour trier
			std::vector<std::pair<int, int>> winOrderList;
			
			for (BasketPlayer& player : m_data->players)
			{
				if (player.m_won)
				{
					// Les gagnants ont un winOrder positif
					winOrderList.push_back({player.m_winOrder, player.m_id});
				}
				else
				{
					// Les perdants ont un winOrder très élevé (fin de liste)
					winOrderList.push_back({9999, player.m_id});
				}
			}
			
			// Trier par ordre de victoire (1er, 2ème, 3ème, puis perdants)
			std::sort(winOrderList.begin(), winOrderList.end());
			
			// Remplir m_winIndex dans l'ordre
			for (const auto& pair : winOrderList)
			{
				m_data->gameData->AddPlayerWin(pair.second);
			}

			// Mise à jour du texte gagnant
			std::string winners;
			bool hasWinner = false;
			
			// Afficher par ordre de victoire
			std::vector<BasketPlayer*> sortedWinners;
			for (BasketPlayer& player : m_data->players)
			{
				if (player.m_won)
					sortedWinners.push_back(&player);
			}
			std::sort(sortedWinners.begin(), sortedWinners.end(), 
				[](BasketPlayer* a, BasketPlayer* b) { return a->m_winOrder < b->m_winOrder; });
			
			for (BasketPlayer* player : sortedWinners)
			{
				if (hasWinner) winners += ", ";
				winners += std::to_string(player->m_id + 1);
				hasWinner = true;
			}
			
			if (!hasWinner)
			{
				winners = "nobody";
			}

			std::snprintf(buffer, 100, "Winners: Player %s\nPress A to Continue", winners.c_str());
			m_data->winnerText.setString(buffer);
			m_data->winnerText.setOrigin(m_data->winnerText.getLocalBounds().width / 2.f, 
				m_data->winnerText.getLocalBounds().height / 2.f);
			m_data->winnerText.setPosition(SCREEN_WIDTH / 2.f, SCREEN_HEIGHT / 2.f);
		}

		// Mise à jour de tous les joueurs
		for (size_t i = 0; i < m_data->players.size(); ++i)
		{
			BasketPlayer& player = m_data->players[i];

			if (player.m_won)
			{
				player.m_aimLine[0].color = sf::Color::Green;
				player.m_aimLine[1].color = sf::Color::Green;
				player.m_ballShape.setFillColor(sf::Color::Green);
				continue;
			}

			// Vérifier si le joueur participe
			if (!m_data->gameData->IsPlayerParticipating(m_data->players[i].m_id))
				continue;

			float joy = GetAxis2D(m_data->gameData->m_playerDataList[m_data->players[i].m_id].m_joystickId).y;
			float force = ABS(joy);
			float forceCircle = (short)(force / 0.00392f);
			float targetY = Rescale(player.m_hoopSprite.getPosition().y, player.m_ballSprite.getPosition().y, 0);

			player.m_ballShape.setFillColor(sf::Color(255, 0, 0, (sf::Uint8)forceCircle));

			player.m_aimLine[0].position = player.m_ballSprite.getPosition();
			player.m_aimLine[0].color = sf::Color::White;
			player.m_aimLine[1].position = sf::Vector2f(player.m_ballSprite.getPosition().x, 
				player.m_ballSprite.getPosition().y - (force * (player.m_ballSprite.getPosition().y - targetY)));
			player.m_aimLine[1].color = sf::Color::White;

			if (GetGamePadPressed(GAMEPAD_A, m_data->gameData->m_playerDataList[m_data->players[i].m_id].m_joystickId, true))
			{
				if (ABS(force - targetY) <= 0.015f)
				{
					// NOUVEAU: Enregistrer l'ordre de victoire
					player.m_won = true;
					m_data->winnerCount++;
					player.m_winOrder = m_data->winnerCount;
					
					std::cout << "Player " << player.m_id + 1 << " won! (Order: " << player.m_winOrder << ")" << std::endl;
				}
			}
		}
	}
}

void Basket::Draw(sf::RenderWindow& _renderWindow)
{
	_renderWindow.draw(m_data->timerText);

	for (BasketPlayer& player : m_data->players)
	{
		_renderWindow.draw(player.m_ballSprite);
		_renderWindow.draw(player.m_hoopSprite);
		_renderWindow.draw(player.m_ballShape);
	}

	if (m_data->state == STATE_GAMEOVER)
	{
		_renderWindow.draw(m_data->winnerText);
	}
}

void Basket::ResetGame(void)
{
	m_data->timer = 15.f;
	m_data->state = STATE_PLAYING;
	m_data->winnerCount = 0;

	for (BasketPlayer& player : m_data->players)
	{
		player.m_won = false;
		player.m_winOrder = -1;
	}
}