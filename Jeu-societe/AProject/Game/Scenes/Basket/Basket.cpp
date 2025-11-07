#include "Basket.hpp"
#include <algorithm>

void Basket::Load()
{
	m_data = new SceneData();

	// Récupération du pointeur GameData depuis m_keptData
	m_data->gameData = (GameData*)this->m_keptData;

	m_data->font.loadFromFile("Assets/RockPaperSizor/Impact.ttf");
	
	m_data->timerText.setFont(m_data->font);
	m_data->timerText.setCharacterSize(25);
	m_data->timerText.setFillColor(sf::Color::Yellow);
	m_data->timerText.setString("Basket Scene");
	m_data->timerText.setOrigin(m_data->timerText.getLocalBounds().width / 2.f, 0);
	m_data->timerText.setPosition(SCREEN_WIDTH / 2, 0);

	m_data->winnerText.setFont(m_data->font);
	m_data->winnerText.setCharacterSize(30);
	m_data->winnerText.setFillColor(sf::Color::Green);
	m_data->winnerText.setOrigin(m_data->winnerText.getLocalBounds().width / 2.f, m_data->winnerText.getLocalBounds().height / 2.f);
	m_data->winnerText.setPosition(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);

	m_data->winnerPlayers = 0;
	m_data->timerToReset = 5.f;
	m_data->elapsedTime = 0.f;

	m_data->ballTexture.loadFromFile("Assets/Images/ball.png");
	m_data->hoopTexture.loadFromFile("Assets/Images/Placeholder.png");

	// Créer les joueurs uniquement pour ceux qui participent
	for (int playerIndex : m_data->gameData->m_gonnaPlayIndex)
	{
		m_data->players.emplace_back(playerIndex, this);
	}
}

void Basket::Unload()
{
	delete m_data;
	m_data = nullptr;
}

void Basket::PollEvent(sf::Event& _event)
{
	// Exemple d'utilisation : vérifier les événements des joueurs participants
	if (_event.type == sf::Event::JoystickButtonPressed)
	{
		int playerID = m_data->gameData->GetPlayerIDFromJoystick(_event.joystickButton.joystickId);
		if (m_data->gameData->IsPlayerParticipating(playerID))
		{
			// Traiter l'événement du joueur participant
		}
	}
}

void Basket::Update(float _deltaTime)
{
	// Incrémenter le temps total écoulé
	if (m_data->timerToReset > 0.f)
	{
		m_data->elapsedTime += _deltaTime;
	}

	char buffer[50];
	std::snprintf(buffer, 50, "Timer %.2f", m_data->timerToReset);
	m_data->timerText.setString(buffer);
	
	std::snprintf(buffer, 50, "Winners player: %s\nPress A to Restart", GetWinnerID());
	m_data->winnerText.setString(buffer);
	m_data->winnerText.setOrigin(m_data->winnerText.getLocalBounds().width / 2.f, m_data->winnerText.getLocalBounds().height / 2.f);
	
	if (m_data->timerToReset <= 0.f)
	{
		m_data->timerToReset = 0.f;
		if (GetGamePadPressed(GAMEPAD_A, 0, true))
		{
			ResetBasketGame();
		}
		m_data->winnerText.setFillColor(sf::Color::Red);
		
		// Créer une copie triée des joueurs par ordre de victoire
		std::vector<BasketPlayer*> sortedPlayers;
		for (BasketPlayer& player : m_data->players)
		{
			sortedPlayers.push_back(&player);
		}

		// Trier : les gagnants d'abord par ordre de temps de victoire, puis les perdants
		std::sort(sortedPlayers.begin(), sortedPlayers.end(), 
			[](const BasketPlayer* a, const BasketPlayer* b) {
				// Si les deux ont gagné, trier par temps de victoire
				if (a->GetWon() && b->GetWon())
				{
					return a->GetWinTime() < b->GetWinTime();
				}
				// Les gagnants avant les perdants
				if (a->GetWon() && !b->GetWon()) return true;
				if (!a->GetWon() && b->GetWon()) return false;
				// Les perdants gardent leur ordre original
				return false;
			});

		// Enregistrer les résultats dans GameData dans l'ordre trié
		bool hasWinner = false;
		for (BasketPlayer* player : sortedPlayers)
		{
			if (player->GetWon())
			{
				m_data->winnerText.setFillColor(sf::Color::Green);
				hasWinner = true;
			}
			// Ajouter tous les joueurs dans l'ordre (gagnants puis perdants)
			m_data->gameData->AddPlayerWin(player->GetID());
		}
		
		if (hasWinner)
		{
			// Retour au Board après la victoire
			ChangeScene("Board", false);
		}
		return;
	}
	m_data->timerToReset -= _deltaTime;
	
	for (BasketPlayer& player : m_data->players)
	{
		player.Update(_deltaTime);
	}
}

void Basket::Draw(sf::RenderWindow& _renderWindow)
{
	_renderWindow.draw(m_data->timerText);
	if (m_data->timerToReset <= 0.f)
	{
		_renderWindow.draw(m_data->winnerText);
	}
	
	for (BasketPlayer& player : m_data->players)
	{
		player.Draw(_renderWindow);
	}
}

void Basket::ResetBasketGame()
{
	m_data->winnerPlayers = 0;
	m_data->timerToReset = 5.f;
	m_data->elapsedTime = 0.f;
	for (BasketPlayer& player : m_data->players)
	{
		player.SetWon(false);
	}
}

float Basket::JoystickToTargetPercentage(float _joystickY, float _ballY, float _axisAbsMax)
{
	float raw = ABS(_joystickY);
	float inputPercent = Rescale(raw, 0.f, _axisAbsMax);
	return inputPercent;
}

bool Basket::NearlyEqual(float _a, float _b, float _tolerance)
{
	return ABS(_a - _b) <= ABS(_tolerance);
}

const char* Basket::GetWinnerID()
{
	static std::string buffer;
	buffer.clear();

	bool any = false;
	for (BasketPlayer& player : m_data->players)
	{
		if (player.GetWon())
		{
			if (any) buffer += ", ";
			buffer += std::to_string(player.GetID());
			any = true;
		}
	}

	if (!any) buffer = "nobody";

	return buffer.c_str();
}

// BasketPlayer Implementation
Basket::BasketPlayer::BasketPlayer(short _id, Basket* _basketScene)
	: id(_id), force(0.0f), won(false), winTime(-1.0f), basketScene(_basketScene), aimLine(sf::Lines, 2)
{
	ballSprite.setTexture(basketScene->m_data->ballTexture);
	hoopSprite.setTexture(basketScene->m_data->hoopTexture);

	float rootSpace = 200.f;
	short rand = std::rand() % 400;
	ballSprite.setPosition((id + 1) * rootSpace, SCREEN_HEIGHT - 300.f);
	ballSprite.setOrigin(ballSprite.getLocalBounds().width / 2.f, ballSprite.getLocalBounds().height / 2.f);

	hoopSprite.setPosition((id + 1) * rootSpace, rand);
	hoopSprite.setOrigin(hoopSprite.getLocalBounds().width / 2.f, hoopSprite.getLocalBounds().height / 2.f);
}

Basket::BasketPlayer::~BasketPlayer()
{
}

short Basket::BasketPlayer::GetID() const
{
	return id;
}

void Basket::BasketPlayer::Update(float _dt)
{
	if (won)
	{
		aimLine[0].color = sf::Color::Green;
		aimLine[1].color = sf::Color::Green;
		return;
	}
	
	float joy = GetAxis2D(id).y;
	float force = basketScene->JoystickToTargetPercentage(joy, ballSprite.getPosition().y, 1);
	float targetY = Rescale(hoopSprite.getPosition().y, ballSprite.getPosition().y, 0);

	aimLine[0].position = ballSprite.getPosition();
	aimLine[0].color = sf::Color::White;
	aimLine[1].position = sf::Vector2f(ballSprite.getPosition().x, ballSprite.getPosition().y - (force * (ballSprite.getPosition().y - targetY)));
	aimLine[1].color = sf::Color::White;

	if (GetGamePadPressed(GAMEPAD_A, id, true))
	{
		if (basketScene->NearlyEqual(force, targetY, 0.015f))
		{
			won = true;
			// Enregistrer le temps de victoire
			winTime = basketScene->m_data->elapsedTime;
		}
	}
}

void Basket::BasketPlayer::Draw(sf::RenderWindow& _renderWindow)
{
	_renderWindow.draw(ballSprite);
	_renderWindow.draw(hoopSprite);
	_renderWindow.draw(aimLine);
}

void Basket::BasketPlayer::SetWon(bool _won)
{
	won = _won;
	if (!_won)
	{
		winTime = -1.0f;
	}
}

bool Basket::BasketPlayer::GetWon() const
{
	return won;
}

float Basket::BasketPlayer::GetWinTime() const
{
	return winTime;
}