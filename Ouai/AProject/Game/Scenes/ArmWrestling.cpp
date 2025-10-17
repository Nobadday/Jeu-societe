#include "ArmWrestling.hpp"

static std::vector<ArmWrestlingPlayer*> allPlayers;

short nextID = 0;

void ArmWrestling::Load(void)
{
	m_data = new SceneData();
	m_data->timer = 0.0f;
	m_data->titleText = new sf::Text();
	m_data->titleText->setFont(StringFormat::GetDefaultFont());
	m_data->titleText->setCharacterSize(24);
	m_data->titleText->setFillColor(sf::Color::White);
	m_data->titleText->setString("Arm Wrestling Mini-Game");
	m_data->titleText->setPosition(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 11);
	m_data->titleText->setOrigin(m_data->titleText->getLocalBounds().width / 2, m_data->titleText->getLocalBounds().height / 2);

	m_data->timeText = new sf::Text();
	m_data->timeText->setFont(StringFormat::GetDefaultFont());
	m_data->timeText->setCharacterSize(15);
	m_data->timeText->setFillColor(sf::Color::White);
	m_data->timeText->setString("Timer: 0/30");
	m_data->timeText->setPosition(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 8);
	m_data->timeText->setOrigin(m_data->timeText->getLocalBounds().width / 2, m_data->timeText->getLocalBounds().height / 2);

	m_data->fillBar = new sf::RectangleShape(sf::Vector2f(200.0f, 30.0f));
	m_data->fillBar->setFillColor(sf::Color::Transparent);
	m_data->fillBar->setOutlineColor(sf::Color::White);
	m_data->fillBar->setOutlineThickness(1.0f);
	m_data->fillBar->setOrigin(m_data->fillBar->getSize().x / 2, m_data->fillBar->getSize().y / 2);
	m_data->fillBar->setPosition(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 5);

	m_data->player1 = new ArmWrestlingPlayer(sf::Vector2f(m_data->fillBar->getPosition().x-50.f, m_data->fillBar->getPosition().y), sf::Color::Red, 1.0f,0);
	m_data->player2 = new ArmWrestlingPlayer(sf::Vector2f(m_data->fillBar->getPosition().x + 50.f, m_data->fillBar->getPosition().y), sf::Color::Green, -1.0f,1);
}

void ArmWrestling::Unload(void)
{
	delete m_data->titleText;
	delete m_data->timeText;
	delete m_data->fillBar;
	m_data->titleText = nullptr;
	m_data->timeText = nullptr;
	m_data->fillBar = nullptr;
	delete m_data->player1;
	delete m_data->player2;
	m_data->player1 = nullptr;
	m_data->player2 = nullptr;
	delete m_data;
	m_data = nullptr;
	nextID = 0;
}

void ArmWrestling::PollEvent(sf::Event& _event)
{
}

void ArmWrestling::Update(float _deltaTime)
{
	m_data->timer += _deltaTime;
	m_data->timeText->setString(StringFormat::Format("Timer: %d/30", (short)m_data->timer));
	m_data->timeText->setOrigin(m_data->timeText->getLocalBounds().width / 2, m_data->timeText->getLocalBounds().height / 2);
	ArmWrestlingPlayer::UpdateAllPlayers(_deltaTime);
}

void ArmWrestling::Draw(sf::RenderWindow& _renderWindow)
{
	_renderWindow.draw(*m_data->titleText);
	_renderWindow.draw(*m_data->timeText);
	_renderWindow.draw(*m_data->fillBar);
	ArmWrestlingPlayer::DrawAllPlayers(_renderWindow);
}

ArmWrestlingPlayer::ArmWrestlingPlayer(sf::Vector2f _pos, sf::Color _color,float _scale, short _id)
{
	id = _id;
	force = 1;
	scale = _scale;
	shape = new sf::RectangleShape(sf::Vector2f(100.0f, 30.0f));
	shape->setFillColor(_color);
	shape->setOrigin(shape->getSize().x / 2, shape->getSize().y / 2);
	shape->setPosition(_pos);
	shape->setScale(_scale, 1);
	nameText = new sf::Text();
	nameText->setFont(StringFormat::GetDefaultFont());
	nameText->setCharacterSize(30);
	nameText->setFillColor(sf::Color::White);
	nameText->setString(StringFormat::Format("Player %d", id + 1));
	sf::Vector2f nameTextSize = sf::Vector2f(nameText->getLocalBounds().width, nameText->getLocalBounds().height);
	nameText->setPosition(_pos.x - (_scale * shape->getSize().x) - (_scale*nameTextSize.x), _pos.y);
	nameText->setOrigin(nameTextSize.x / 2, nameTextSize.y / 2);
	allPlayers.push_back(this);

}

ArmWrestlingPlayer::~ArmWrestlingPlayer()
{
	delete shape;
	shape = nullptr;
	delete nameText;
	nameText = nullptr;
	for (auto it = allPlayers.begin(); it != allPlayers.end(); ++it)
	{
		if (*it == this)
		{
			allPlayers.erase(it);
			nextID--;
			break;
		}
	}
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

ArmWrestlingPlayer& ArmWrestlingPlayer::GetPlayerByID(short _id)
{
	for (ArmWrestlingPlayer* player : allPlayers)
	{
		if (player->GetID() == _id)
		{
			return *player;
		}
	}
	throw std::runtime_error("Player with ID " + std::to_string(_id) + " not found.");
}

void ArmWrestlingPlayer::UpdateAllPlayers(float _dt)
{
	for (ArmWrestlingPlayer* player : allPlayers)
	{
		player->Update(_dt);
	}
}

void ArmWrestlingPlayer::DrawAllPlayers(sf::RenderWindow& _renderWindow)
{
	for (ArmWrestlingPlayer* player : allPlayers)
	{
		player->Draw(_renderWindow);
	}
}

void ArmWrestlingPlayer::Update(float _dt)
{
	// Si le bouton A est pressé pour ce joueur
	if (GetGamePadPressed(GAMEPAD_A, id, true))
	{
		short otherID = GetOtherPlayerID(id);
		ArmWrestlingPlayer& other = GetPlayerByID(otherID);

		shape->setSize(sf::Vector2f(shape->getSize().x + force, shape->getSize().y));
		other.shape->setSize(sf::Vector2f(other.shape->getSize().x - force, other.shape->getSize().y));
	}
}

void ArmWrestlingPlayer::Draw(sf::RenderWindow& _renderWindow)
{
	_renderWindow.draw(*shape);
	_renderWindow.draw(*nameText);
}

short ArmWrestlingPlayer::GetOtherPlayerID(short _callerID)
{
	// Si moins de deux joueurs, il n'y a pas d'adversaire
	if (allPlayers.size() < 2)
	{
		throw std::runtime_error("Other player not available.");
	}

	// Retourne l'ID du premier joueur dont l'ID diffère de callerID
	for (ArmWrestlingPlayer* player : allPlayers)
	{
		if (player->GetID() != _callerID)
		{
			return player->GetID();
		}
	}

	throw std::runtime_error("Other player not found.");
}