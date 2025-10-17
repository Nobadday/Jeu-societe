#include "Game.hpp"

sf::RectangleShape* Game::barBack = nullptr;
sf::RectangleShape* Game::barFrontR = nullptr;
sf::RectangleShape* Game::barFrontL = nullptr;
short Game::powerL = 0;
short Game::powerR = 0;
sf::Font* Game::font = nullptr;
sf::Text* Game::Counter = nullptr;
float Game::timeCounter = 0.f;

Game::Game()
{
	barBack = new sf::RectangleShape(sf::Vector2f(202.0f, 22.0f));
	barBack->setFillColor(sf::Color::Transparent);
	barBack->setPosition(SCREEN_WIDTH/2, SCREEN_HEIGHT/5);
	barBack->setOutlineColor(sf::Color::Blue);
	barBack->setOutlineThickness(1.0f);
	barBack->setOrigin((barBack->getSize().x/2), (barBack->getSize().y / 2.0f));

	barFrontR = new sf::RectangleShape(sf::Vector2f(100.f, 20.0f));
	barFrontR->setFillColor(sf::Color::Green);
	barFrontR->setOrigin((0), (barFrontR->getSize().y / 2.0f));
	barFrontR->setScale(1.f, 1.f);
	barFrontR->setPosition(SCREEN_WIDTH / 2+ barFrontR->getSize().x, SCREEN_HEIGHT / 5);

	barFrontL = new sf::RectangleShape(sf::Vector2f(100.f, 20.0f));
	barFrontL->setFillColor(sf::Color::Red);
	barFrontL->setOrigin((barFrontL->getSize().x), (barFrontL->getSize().y / 2.0f));
	barFrontR->setScale(-1.f, 1.f);
	barFrontL->setPosition(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 5);

	powerL = 1;
	powerR = 1;

	font = new sf::Font();
	font->loadFromFile("Assets/Font.ttf");
	Counter = new sf::Text();
	Counter->setFont(GetFont());
	Counter->setCharacterSize(24);
	Counter->setPosition(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 8);

	timeCounter = 0.f;
}
Game::~Game()
{
	delete barBack;
	delete barFrontR;
	delete barFrontL;
	barBack = nullptr;
	barFrontR = nullptr;
	barFrontL = nullptr;

	powerL = 0;
	powerR = 0;

	delete font;
	delete Counter;
	font = nullptr;
	Counter = nullptr;

	timeCounter = 0.f;
}

void Game::Update(float _dt)
{
	if (GetBindPressed(ARMWRESTLINGL,true))
	{
		barFrontL->setSize(sf::Vector2f(barFrontL->getSize().x + powerL, barFrontL->getSize().y));
		barFrontR->setSize(sf::Vector2f(barFrontR->getSize().x - powerL, barFrontR->getSize().y));
	}
	if (GetBindPressed(ARMWRESTLINGR, true))
	{
		barFrontL->setSize(sf::Vector2f(barFrontL->getSize().x - powerR, barFrontL->getSize().y));
		barFrontR->setSize(sf::Vector2f(barFrontR->getSize().x + powerR, barFrontR->getSize().y));
	}

	if(GetBindDebugPressed(DEBUG_KEY_1, true))
	{
		powerL = 5;
	}

	timeCounter += _dt;
	Counter->setString(StringFormat::Format("Time: %d", (short)timeCounter));
	Counter->setOrigin(Counter->getLocalBounds().width / 2, Counter->getLocalBounds().height / 2);
	if(timeCounter >= 30.f)
	{
		SetGameState(MENU);
	}

	std::cout << sf::Joystick::getAxisPosition(1, sf::Joystick::Axis::Z) << std::endl;
}
void Game::Draw(sf::RenderWindow* const _renderWindow)
{
	_renderWindow->draw(*barBack);
	_renderWindow->draw(*barFrontR);
	_renderWindow->draw(*barFrontL);
	_renderWindow->draw(*Counter);
}

void Game::KeyPressed(sf::Event::KeyEvent _key, sf::RenderWindow* _renderWindow)
{
	switch (_key.code)
	{
	case sf::Keyboard::Key::Escape:
		SetGameState(MENU);
		break;
	default:
		break;
	}
}

void Game::KeyReleased(sf::Event::KeyEvent _key, sf::RenderWindow* _renderWindow)
{
}

sf::Font& Game::GetFont()
{
	return *font;
}