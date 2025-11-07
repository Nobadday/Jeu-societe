#include "Basket.hpp"

static std::vector<BasketPlayer*> allPlayers;
short leftPlayerBasket = 0;

sf::Texture* ballTexture;
sf::Texture* hoopTexture;

float JoystickToTargetPercentage(float _joystickY, float _ballY, float _axisAbsMax);
bool NearlyEqual(float _a, float _b, float _tolerance);
const char* GetWinnerID();


void Basket::Load(void)
{
	m_data = new SceneData();
	ballTexture = nullptr;
	hoopTexture = nullptr;
	m_data->timerText = new sf::Text();

	m_data->timerText->setFont(StringFormat::GetDefaultFont());
	m_data->timerText->setCharacterSize(25);
	m_data->timerText->setFillColor(sf::Color::Yellow);
	m_data->timerText->setString("Basket Scene");
	m_data->timerText->setOrigin(m_data->timerText->getLocalBounds().width / 2.f, 0);
	m_data->timerText->setPosition(SCREEN_WIDTH / 2, 0);

	m_data->winnerText = new sf::Text();
	m_data->winnerText->setFont(StringFormat::GetDefaultFont());
	m_data->winnerText->setCharacterSize(30);
	m_data->winnerText->setFillColor(sf::Color::Green);
	m_data->winnerText->setOrigin(m_data->winnerText->getLocalBounds().width / 2.f, m_data->winnerText->getLocalBounds().height / 2.f);
	m_data->winnerText->setPosition(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);

	m_data->winnerPlayers = 0;
	m_data->timerToReset = 5.f;

	ballTexture = new sf::Texture();
	ballTexture->loadFromFile("Assets/Images/ball.png");

	hoopTexture = new sf::Texture();
	hoopTexture->loadFromFile("Assets/Images/Placeholder.png");

	for (short i = 0; i < 4; ++i)
	{
		if (sf::Joystick::isConnected(i))
		{
			new BasketPlayer(i);
			leftPlayerBasket++;
		}
	}
}

void Basket::Unload(void)
{
	delete ballTexture;
	delete hoopTexture;
	delete m_data->timerText;
	delete m_data->winnerText;
	ballTexture = nullptr;
	hoopTexture = nullptr;
	m_data->timerText = nullptr;
	m_data->winnerText = nullptr;

	for (BasketPlayer* player : allPlayers)
	{
		delete player;
	}

	delete m_data;
	m_data = nullptr;
}

void Basket::PollEvent(sf::Event& _event)
{
}

void Basket::Update(float _deltaTime)
{
	m_data->timerText->setString(StringFormat::Format("Timer %.2f", m_data->timerToReset));
	m_data->winnerText->setString(StringFormat::Format("Winners player: %s\nPress A to Restart", GetWinnerID()));
	m_data->winnerText->setOrigin(m_data->winnerText->getLocalBounds().width / 2.f, m_data->winnerText->getLocalBounds().height / 2.f);
	if (m_data->timerToReset <= 0.f)
	{
		m_data->timerToReset = 0.f;
		if (GetGamePadPressed(GAMEPAD_A, 0, true))
		{
			ResetBasketGame();
		}
		m_data->winnerText->setFillColor(sf::Color::Red);
		for (BasketPlayer* player : allPlayers)
		{
			if (player->GetWon())
			{
				m_data->winnerText->setFillColor(sf::Color::Green);
				return;
			}
			else
			{
                (*player->aimLine)[0].color = sf::Color::Red;
                (*player->aimLine)[1].color = sf::Color::Red;
			}
		}
		return;
	}
	m_data->timerToReset -= _deltaTime;
	BasketPlayer::UpdateAllPlayers(_deltaTime);
}

void Basket::Draw(sf::RenderWindow& _renderWindow)
{
	if (m_data && m_data->timerText) _renderWindow.draw(*m_data->timerText);
	if (m_data && m_data->winnerText && m_data->timerToReset <= 0.f) _renderWindow.draw(*m_data->winnerText);
	BasketPlayer::DrawAllPlayers(_renderWindow);
}

BasketPlayer::BasketPlayer(short _id)
	: id(_id), force(0.0f), ballSprite(nullptr), hoopSprite(nullptr)
{
	ballSprite = new sf::Sprite(*ballTexture);
	hoopSprite = new sf::Sprite(*hoopTexture);

	aimLine = new sf::VertexArray(sf::Lines, 2);

	won = false;

	float spape = 500.f;
	float rootSpace = 200.f;
	short rand = std::rand() % 400;
	ballSprite->setPosition((id+1) * rootSpace, SCREEN_HEIGHT - 300.f);
	ballSprite->setOrigin(ballSprite->getLocalBounds().width / 2.f, ballSprite->getLocalBounds().height / 2.f);

	hoopSprite->setPosition((id + 1) * rootSpace, rand);
	hoopSprite->setOrigin(hoopSprite->getLocalBounds().width / 2.f, hoopSprite->getLocalBounds().height / 2.f);

	allPlayers.push_back(this);
}

BasketPlayer::~BasketPlayer()
{
	for (auto it = std::remove(allPlayers.begin(), allPlayers.end(), this); it != allPlayers.end(); ++it)
	{
		if (*it == this)
		{
			allPlayers.erase(it);
			return;
		}
	}

	delete ballSprite;
	delete hoopSprite;
	delete aimLine;
	aimLine = nullptr;
	ballSprite = nullptr;
	hoopSprite = nullptr;
}

void BasketPlayer::UpdateAllPlayers(float _dt)
{
	for (BasketPlayer* player : allPlayers)
	{
		player->Update(_dt);
	}
}

void BasketPlayer::DrawAllPlayers(sf::RenderWindow& _renderWindow)
{
	for (BasketPlayer* player : allPlayers)
	{
		if (player) player->Draw(_renderWindow);
	}
}

short BasketPlayer::GetID(void) const
{
	return id;
}

void BasketPlayer::Update(float _dt)
{

	if (won)
	{
		(*aimLine)[0].color = sf::Color::Green;
		(*aimLine)[1].color = sf::Color::Green;
		return;
	}
	float joy = GetAxis2D(id).y;
	float force = JoystickToTargetPercentage(joy, ballSprite->getPosition().y, 1);
	float targetY = Rescale(hoopSprite->getPosition().y, ballSprite->getPosition().y, 0);

	(*aimLine)[0].position = ballSprite->getPosition();
	(*aimLine)[0].color = sf::Color::White;
	(*aimLine)[1].position = sf::Vector2f(ballSprite->getPosition().x, ballSprite->getPosition().y - (force * (ballSprite->getPosition().y - targetY)));
	(*aimLine)[1].color = sf::Color::White;

	if (GetGamePadPressed(GAMEPAD_A, id, true))
	{
		if (NearlyEqual(force, targetY, 0.015f))
		{
			won = true;
		}
	}

}

void BasketPlayer::Draw(sf::RenderWindow& _renderWindow)
{
	if (ballSprite) _renderWindow.draw(*ballSprite);
	if (hoopSprite) _renderWindow.draw(*hoopSprite);
	_renderWindow.draw(*aimLine);
}

float JoystickToTargetPercentage(float _joystickY, float _ballY, float _axisAbsMax)
{
	float raw = ABS(_joystickY);

	float inputPercent = Rescale(raw, 0.f, _axisAbsMax);

	_joystickY = inputPercent;

	return inputPercent;
}

bool NearlyEqual(float _a, float _b, float _tolerance)
{
	return ABS(_a - _b) <= ABS(_tolerance);
}

void Basket::ResetBasketGame(void)
{
	m_data->winnerPlayers = 0;
	m_data->timerToReset = 5.f;
	for (BasketPlayer* player : allPlayers)
	{
		player->SetWon(false);
	}
}

void BasketPlayer::SetWon(bool _won)
{
	won = _won;
}

bool BasketPlayer::GetWon(void) const
{
	return won;
}

const char* GetWinnerID()
{
	static std::string buffer;
	buffer.clear();

	bool any = false;
	for (BasketPlayer* player : allPlayers)
	{
		if (player && player->GetWon())
		{
			if (any) buffer += "player, ";
			buffer += std::to_string(player->GetID());
			any = true;
		}
	}

	if (!any) buffer = "nobody";

	return buffer.c_str();
}