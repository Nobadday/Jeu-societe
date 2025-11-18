#include "Common.hpp"
#include "./Animation/Graphics.hpp"
#include "./Utilities/BetterWindow/BetterWindow.hpp"
#include "./Utilities/Camera/Camera.hpp"

typedef struct MainData
{
	sfMod::RenderWindow renderWindow;
	sf::Clock clock;
	SceneHandler scenes;

	TextAnimated tp;
	GameData gameData;
} MainData;


int main(void);

void MainDataLoad(MainData& _mainData);

void PollEvent(MainData& _mainData);
void Update(MainData& _mainData);
void Draw(MainData& _mainData);


int main(void)
{
	random::SetSeedPID();
	randmt::SetSeedPID();
	
	MainData mainData;
	MainDataLoad(mainData);

	sf::Font font;
	font.loadFromFile("./Assets/Fonts/OMORI_GAME2.ttf");

	mainData.renderWindow.setIcon("./Assets/Images/Placeholder.png");
	
	mainData.tp.setFont(font);
	mainData.tp.setString("CLAP BONJOUR ! C'est Amixem, le sexy, je sais pas. Très long string now. évitezdefairedesphrasessansespaceslesenfants!");
	mainData.tp.setCharacterSize(60u);
	mainData.tp.setFillColor(sf::Color::White);
	mainData.tp.setPosition(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f);
	mainData.tp.setOrigin(sf::Vector2f(0.5f, 0.5f));
	mainData.tp.SetCharactersPerLine(20);
	mainData.tp.SetAlignement(TextPlus::Alignement::CENTER);
	mainData.tp.Modify(8.5f, 24.0f, false);
	
	


	while (mainData.renderWindow.isOpen())
	{
		PollEvent(mainData);
		Update(mainData);
		Draw(mainData);

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::RShift))
		{
			printf("-- MANUAL FREEZE : [    /  ON] --\n");
			while (sf::Keyboard::isKeyPressed(sf::Keyboard::RShift));
			printf("-- MANUAL FREEZE : [OFF /    ] --\n");
		}
	}

	return EXIT_SUCCESS;
}

void MainDataLoad(MainData& _mainData)
{
	_mainData.renderWindow.create(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "SFML", sf::Style::Default);
	_mainData.renderWindow.setKeyRepeatEnabled(false);
	
	_mainData.scenes.SetTransferedData(&_mainData.gameData);

	// GAME DATA
	_mainData.gameData.m_renderWindow = &_mainData.renderWindow;
	

	_mainData.clock.restart();
}


void PollEvent(MainData& _mainData)
{
	sf::Event event;
	sf::Vector2i mp;
	while (_mainData.renderWindow.pollEvent(event))
	{
		switch (event.type)
		{
			case sf::Event::Closed:
				_mainData.renderWindow.close();
				return;
				break;


			case sf::Event::MouseMoved:
				mp.x = event.mouseMove.x;
				mp.y = event.mouseMove.y;
				_mainData.renderWindow.CorrectMousePos(mp);
				_mainData.tp.setPosition((sf::Vector2f)mp);
				break;
			case sf::Event::MouseButtonPressed:
			case sf::Event::MouseButtonReleased:
				printf("MB : (%d, %d)\n", event.mouseButton.x, event.mouseButton.y);
				break;

			case sf::Event::KeyPressed:
				switch (event.key.code)
				{
					case sf::Keyboard::Enter:
						if (!sf::Keyboard::isKeyPressed(sf::Keyboard::LAlt))
						{
							break;
						}
						[[fallthrough]];
					case sf::Keyboard::F11:
						_mainData.renderWindow.ToggleFullscreen(true);
						break;

					case sf::Keyboard::G:
						if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1))
						{
							_mainData.renderWindow.SetDisplayMode(sfMod::RenderWindow::STRETCH);
						}
						else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2))
						{
							_mainData.renderWindow.SetDisplayMode(sfMod::RenderWindow::PAN);
						}
						else
						{
							_mainData.renderWindow.SetDisplayMode(sfMod::RenderWindow::LETTERBOX);
						}
						break;

					default:
						break;
				}
				[[fallthrough]];

				

			default:
				_mainData.scenes.PollEvent(event);
				break;

		}
	}
}

void Update(MainData& _mainData)
{
	float deltaTime = _mainData.clock.restart().asSeconds();
	//float dtFixed = deltaTime / (1.0f / 60.0f);
	
	_mainData.scenes.Update(deltaTime);
	_mainData.tp.Update(deltaTime);
}

void Draw(MainData& _mainData)
{
	_mainData.renderWindow.clear(sf::Color::Black);

	sf::RectangleShape rect(sf::Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT));
	rect.setFillColor(sf::Color(50, 215, 100));
	_mainData.renderWindow.draw(rect);
	

	_mainData.scenes.Draw(_mainData.renderWindow);

	sf::Vector2i mp = sf::Mouse::getPosition(_mainData.renderWindow);
	_mainData.renderWindow.CorrectMousePos(mp);

	_mainData.renderWindow.draw(_mainData.tp);
	
	sf::CircleShape cir(15.0f);
	cir.setFillColor(sf::Color::Red);
	cir.setPosition((sf::Vector2f)mp);
	cir.setOrigin(15.0f, 15.0f);
	_mainData.renderWindow.draw(cir);

	_mainData.renderWindow.display();
}

