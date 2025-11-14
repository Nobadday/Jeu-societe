#include "ArmWrestling.hpp"

void ArmWrestling::Load(void)
{
	m_data = new SceneData();
	m_data->timer = 0.0f;

	((GameData*)this->m_keptData)->m_assetManager->LoadManifest("Manifests/ArmWrestling.json", "ArmWrestling");

	if (((GameData*)this->m_keptData)->m_gonnaPlayIndex.size() == 0)
	{
		((GameData*)this->m_keptData)->m_gonnaPlayIndex.push_back(0);
		((GameData*)this->m_keptData)->m_gonnaPlayIndex.push_back(1);
	}

	// Title text (membre par valeur)
	m_data->titleText.setFont(*((GameData*)this->m_keptData)->m_assetManager->GetAsset<sf::Font>("Font"));
	m_data->titleText.setCharacterSize(24);
	m_data->titleText.setFillColor(sf::Color::White);
	m_data->titleText.setString("Arm Wrestling Mini-Game");
	m_data->titleText.setPosition(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 11);
	m_data->titleText.setOrigin(m_data->titleText.getLocalBounds().width / 2, m_data->titleText.getLocalBounds().height / 2);

	// Time text (membre par valeur)
	m_data->timeText.setFont(*((GameData*)this->m_keptData)->m_assetManager->GetAsset<sf::Font>("ArmWrestlingFont"));
	m_data->timeText.setCharacterSize(15);
	m_data->timeText.setFillColor(sf::Color::White);
	m_data->timeText.setString("Timer: 0/15");
	m_data->timeText.setPosition(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 8);
	m_data->timeText.setOrigin(m_data->timeText.getLocalBounds().width / 2, m_data->timeText.getLocalBounds().height / 2);

	// Fill bar (membre par valeur)
	m_data->fillBar = sf::RectangleShape(sf::Vector2f(200.0f, 30.0f));
	m_data->fillBar.setFillColor(sf::Color::Transparent);
	m_data->fillBar.setOutlineColor(sf::Color::White);
	m_data->fillBar.setOutlineThickness(1.0f);
	m_data->fillBar.setOrigin(m_data->fillBar.getSize().x / 2, m_data->fillBar.getSize().y / 2);
	m_data->fillBar.setPosition(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 5);

	// Initialiser la liste et nextID dans SceneData
	m_data->allPlayers.clear();
	m_data->nextID = 0;

	// Créer les joueurs dans la vector (par valeur), pas de new / pas de placement new
	m_data->allPlayers.emplace_back(sf::Vector2f(m_data->fillBar.getPosition().x - 50.f, m_data->fillBar.getPosition().y), sf::Color::Red, 1.0f, m_data->nextID++);
	m_data->allPlayers[0].SetFont(*((GameData*)this->m_keptData)->m_assetManager->GetAsset<sf::Font>("ArmWrestlingFont"));
	m_data->allPlayers.emplace_back(sf::Vector2f(m_data->fillBar.getPosition().x + 50.f, m_data->fillBar.getPosition().y), sf::Color::Green, -1.0f, m_data->nextID++);
	m_data->allPlayers[1].SetFont(*((GameData*)this->m_keptData)->m_assetManager->GetAsset<sf::Font>("ArmWrestlingFont"));

	// Liaison au GameData passé dans m_keptData (comme dans RockPaperScissors)
	m_data->gameData = (GameData*)this->m_keptData;

	// Préparer le texte de résultat (vide pour l'instant)
	m_data->resultText.setFont(*((GameData*)this->m_keptData)->m_assetManager->GetAsset<sf::Font>("ArmWrestlingFont"));
	m_data->resultText.setCharacterSize(20);
	m_data->resultText.setFillColor(sf::Color::White);
	m_data->resultText.setString("");
	m_data->resultText.setPosition(SCREEN_WIDTH / 2, SCREEN_HEIGHT * 0.7f);
	m_data->resultText.setOrigin(m_data->resultText.getLocalBounds().width / 2, m_data->resultText.getLocalBounds().height / 2);

	m_data->finished = false;
	m_data->endDelay = 0.0f;
}

void ArmWrestling::Unload(void)
{
	if (!m_data) return;

	// Nettoyer la liste des joueurs
	m_data->allPlayers.clear();

	delete m_data;
	m_data = nullptr;
}

void ArmWrestling::PollEvent(sf::Event& _event)
{
	(void)_event;
}

void ArmWrestling::Update(float _deltaTime)
{
	if (!m_data) return;

	// Si on est déjà en phase de résultat, décrémenter le délai avant changement de scène
	if (m_data->finished)
	{
		m_data->endDelay -= _deltaTime;
		if (m_data->endDelay <= 0.0f)
		{
			ChangeScene("Board", false);
		}
		return;
	}

	m_data->timer += _deltaTime;
	char buffer[50];
	std::snprintf(buffer, 50, "Timer: %d/15", (short)m_data->timer);
	m_data->timeText.setString(buffer);
	m_data->timeText.setOrigin(m_data->timeText.getLocalBounds().width / 2, m_data->timeText.getLocalBounds().height / 2);

	// Appeler Update sur chaque joueur (comme RockPaperScissors fait pour ses données)
	for (auto& player : m_data->allPlayers)
	{
		player.Update(_deltaTime, m_data->allPlayers);
	}

	// Conditions de fin : timer >= 30 OU un bras atteint la largeur de la barre OU un bras <= 0
	bool timeUp = (m_data->timer >= 15.0f);
	bool barMaxMin = false;
	for (const auto& player : m_data->allPlayers)
	{
		if (player.GetArmWidth() >= m_data->fillBar.getSize().x || player.GetArmWidth() <= 0.0f)
		{
			barMaxMin = true;
			break;
		}
	}

	if (timeUp || barMaxMin)
	{
		// Déterminer gagnant/perdant (suppose 2 joueurs)
		if (m_data->allPlayers.size() < 2)
		{
			// Pas assez de joueurs, retour direct
			ChangeScene("Board", false);
			return;
		}

		const auto& p0 = m_data->allPlayers[0];
		const auto& p1 = m_data->allPlayers[1];

		int winnerIndexInPlayers = (p0.GetArmWidth() >= p1.GetArmWidth()) ? 0 : 1;
		int loserIndexInPlayers = 1 - winnerIndexInPlayers;

		short winnerID = m_data->allPlayers[winnerIndexInPlayers].GetID();
		short loserID = m_data->allPlayers[loserIndexInPlayers].GetID();

		// Enregistrer les résultats dans GameData comme RockPaperScissors
		if (m_data->gameData)
		{
			// Ajout de la paire gagnant/perdant (même convention que RPS)
			m_data->gameData->AddPlayerWin(winnerID);
			m_data->gameData->AddPlayerWin(loserID);
		}

		// Préparer l'affichage du résultat
		char buffer[64];
		std::snprintf(buffer, sizeof(buffer), "Player %d wins\nPlayer %d loses", winnerID + 1, loserID + 1);
		m_data->resultText.setString(buffer);
		m_data->resultText.setOrigin(m_data->resultText.getLocalBounds().width / 2, m_data->resultText.getLocalBounds().height / 2);

		// Mettre en attente avant retour au plateau
		m_data->finished = true;
		m_data->endDelay = 2.0f; // 2 secondes d'affichage du résultat
	}
}

void ArmWrestling::Draw(sf::RenderWindow& _renderWindow)
{
	if (!m_data) return;

	_renderWindow.draw(m_data->titleText);
	_renderWindow.draw(m_data->timeText);
	_renderWindow.draw(m_data->fillBar);

	for (auto& player : m_data->allPlayers)
	{
		player.Draw(_renderWindow);
	}

	if (m_data->finished)
	{
		_renderWindow.draw(m_data->resultText);
	}
}







ArmWrestlingPlayer::ArmWrestlingPlayer(sf::Vector2f _pos, sf::Color _color, float _scale, short _id)
{
	id = _id;
	force = 1;

	// shape est un membre par valeur
	shape = sf::RectangleShape(sf::Vector2f(100.0f, 30.0f));
	shape.setFillColor(_color);
	shape.setOrigin(shape.getSize().x / 2, shape.getSize().y / 2);
	shape.setPosition(_pos);
	shape.setScale(_scale, 1);

	// nameText est un membre par valeur
	nameText = sf::Text();
	nameText.setCharacterSize(30);
	nameText.setFillColor(sf::Color::White);
	char buffer[50];
	std::snprintf(buffer, 50, "Player %d", id + 1);
	nameText.setString(buffer);
	sf::Vector2f nameTextSize = sf::Vector2f(nameText.getLocalBounds().width, nameText.getLocalBounds().height);
	nameText.setPosition(_pos.x - (_scale * shape.getSize().x) - (_scale * nameTextSize.x), _pos.y);
	nameText.setOrigin(nameTextSize.x / 2, nameTextSize.y / 2);
}

short ArmWrestlingPlayer::GetID(void) const
{
	return id;
}

short ArmWrestlingPlayer::GetForce(void) const
{
	return force;
}

void ArmWrestlingPlayer::SetForce(short _force)
{
	force = _force;
}

void ArmWrestlingPlayer::Update(float _dt, std::vector<ArmWrestlingPlayer>& allPlayers)
{
	(void)_dt;
	if (GetGamePadPressed(GAMEPAD_A, id, true))
	{
		// Trouver l'autre joueur dans la liste fournie
		for (auto& player : allPlayers)
		{
			if (player.GetID() != id)
			{
				// Augmenter la taille du bras du joueur courant et diminuer celle de l'autre
				shape.setSize(sf::Vector2f(shape.getSize().x + force, shape.getSize().y));
				player.shape.setSize(sf::Vector2f(player.shape.getSize().x - force, player.shape.getSize().y));
				break;
			}
		}
	}
}

void ArmWrestlingPlayer::Draw(sf::RenderWindow& _renderWindow)
{
	_renderWindow.draw(shape);
	_renderWindow.draw(nameText);
}

short ArmWrestlingPlayer::GetOtherPlayerID(short _callerID, const std::vector<ArmWrestlingPlayer>& allPlayers) const
{
	if (allPlayers.size() < 2)
	{
		throw std::runtime_error("Other player not available.");
	}

	for (const auto& player : allPlayers)
	{
		if (player.GetID() != _callerID)
		{
			return player.GetID();
		}
	}

	throw std::runtime_error("Other player not found.");
}

// Implémentation du getter ajouté
float ArmWrestlingPlayer::GetArmWidth() const
{
	return shape.getSize().x;
}

void ArmWrestlingPlayer::SetFont(sf::Font _font)
{
	this->nameText.setFont(_font);
}
