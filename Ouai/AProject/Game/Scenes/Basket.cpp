#include "Basket.hpp"

static short nextID;
static std::vector<BasketPlayer*> allPlayers;

sf::Texture* ballTexture;
sf::Texture* hoopTexture;

float JoystickToTargetPercentage(float _joystickY, float _ballY, float _axisAbsMax);
bool NearlyEqual(float _a, float _b, float _tolerance);


void Basket::Load(void)
{
	m_data = new SceneData();
	// Initialise les pointeurs pour éviter les déréférencements futurs
	ballTexture = nullptr;
	hoopTexture = nullptr;
	m_data->text = new sf::Text();

	m_data->text->setFont(StringFormat::GetDefaultFont());
	m_data->text->setCharacterSize(20);
	m_data->text->setFillColor(sf::Color::White);
	m_data->text->setString("Basket Scene");
	m_data->text->setOrigin(m_data->text->getLocalBounds().width / 2.f, m_data->text->getLocalBounds().height / 10.f);
	m_data->text->setPosition(SCREEN_WIDTH/2, SCREEN_HEIGHT/2);

	ballTexture = new sf::Texture();
	ballTexture->loadFromFile("Assets/Images/ball.png");

	hoopTexture = new sf::Texture();
	hoopTexture->loadFromFile("Assets/Images/Placeholder.png");

	m_data->player1 = new BasketPlayer(0);
	m_data->player2 = new BasketPlayer(1);
}

void Basket::Unload(void)
{
	delete ballTexture;
	delete hoopTexture;
	delete m_data->text;
	ballTexture = nullptr;
	hoopTexture = nullptr;
	m_data->text = nullptr;
	delete m_data->player1;
	delete m_data->player2;
	m_data->player1 = nullptr;
	m_data->player2 = nullptr;
	delete m_data;
	m_data = nullptr;
}

void Basket::PollEvent(sf::Event& _event)
{
}

void Basket::Update(float _deltaTime)
{
	BasketPlayer::UpdateAllPlayers(_deltaTime);
}

void Basket::Draw(sf::RenderWindow& _renderWindow)
{
	if (m_data && m_data->text) _renderWindow.draw(*m_data->text);
	BasketPlayer::DrawAllPlayers(_renderWindow);
}


// Implémentations de BasketPlayer manquantes

BasketPlayer::BasketPlayer(short _id)
	: id(_id >= 0 ? _id : nextID++), force(0.0f), ballSprite(nullptr), hoopSprite(nullptr)
{
	// Allouer des sprites vides pour que les méthodes Draw/Update puissent les utiliser sans vérifier l'allocation externe
	ballSprite = new sf::Sprite(*ballTexture);
	hoopSprite = new sf::Sprite(*hoopTexture);

	aimLine = new sf::VertexArray(sf::Lines, 2);

	// Positionner les sprites pour différencier les joueurs
	float scape = 500.f;
	float rootSpace = 200.f;
	short rand = std::rand() % 400;
	ballSprite->setPosition(rootSpace + id * scape, SCREEN_HEIGHT - 300.f);
	ballSprite->setOrigin(ballSprite->getLocalBounds().width / 2.f, ballSprite->getLocalBounds().height / 2.f);

	hoopSprite->setPosition(rootSpace + id * scape, rand);
	hoopSprite->setOrigin(hoopSprite->getLocalBounds().width / 2.f, hoopSprite->getLocalBounds().height / 2.f);
	

	allPlayers.push_back(this);
}

BasketPlayer::~BasketPlayer()
{
	// Retirer cette instance de la liste globale
	auto it = std::remove(allPlayers.begin(), allPlayers.end(), this);
	allPlayers.erase(it, allPlayers.end());
	nextID--;

	// Libérer les ressources locales
	delete ballSprite;
	delete hoopSprite;
	delete aimLine;
	aimLine = nullptr;
	ballSprite = nullptr;
	hoopSprite = nullptr;
}

void BasketPlayer::UpdateAllPlayers(float _dt)
{
	for (BasketPlayer* p : allPlayers)
	{
		if (p) p->Update(_dt);
	}
}

void BasketPlayer::DrawAllPlayers(sf::RenderWindow& _renderWindow)
{
	for (BasketPlayer* p : allPlayers)
	{
		if (p) p->Draw(_renderWindow);
	}
}

short BasketPlayer::GetID(void) const
{
	return id;
}

// Méthodes privées
void BasketPlayer::Update(float _dt)
{
	float joy = GetAxis2D(id).y;
	float force = JoystickToTargetPercentage(joy, ballSprite->getPosition().y, 1);
	float targetY = Rescale(hoopSprite->getPosition().y, ballSprite->getPosition().y, 0);

	// Met à jour la ligne de visée
	(*aimLine)[0].position = ballSprite->getPosition();
	(*aimLine)[0].color = sf::Color::White;
	(*aimLine)[1].position = sf::Vector2f(ballSprite->getPosition().x, ballSprite->getPosition().y - (force * (ballSprite->getPosition().y - targetY)));
	(*aimLine)[1].color = sf::Color::White;

	if (GetGamePadPressed(GAMEPAD_A, id, true))
	{
		if (NearlyEqual(force, targetY, 0.105f)) std::cout << "Player " << id << " scored!" << std::endl; else std::cout << "Player " << id << " missed!" << std::endl;
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
	// Valeur absolue du joystick (intensité)
	float raw = fabsf(_joystickY);

	// Remappe l'intensité en [0,1] selon l'échelle d'axe fournie
	float inputPercent = Rescale(raw, 0.f, _axisAbsMax);

	// Écrit le pourcentage dans la variable passée par référence
	_joystickY = inputPercent;

	// Si besoin, l'appelant peut convertir ce pourcentage en pixels :
	// float distanceRemaining = fabsf(targetY - ballY);
	// float movePixels = inputPercent * distanceRemaining;
	return inputPercent;
}

bool NearlyEqual(float _a, float _b, float _tolerance)
{
	return fabsf(_a - _b) <= fabsf(_tolerance);
}