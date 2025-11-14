#include "Common.hpp"
#include "./Animation/Graphics.hpp"
#include "./Utilities/BetterWindow/BW.hpp"
#include "./Utilities/Camera/Camera.hpp"

typedef struct MainData
{
	sfMod::RenderWindow renderWindow;
	sf::Clock clock;
	SceneHandler scenes;

	TextAnimated tp;
	GameData gameData;
	sf::Texture tstTex;

	Camera cam;
} MainData;


int main(void);

void MainDataLoad(MainData& _mainData);

void PollEvent(MainData& _mainData);
void Update(MainData& _mainData);
void Draw(MainData& _mainData);


int main(void)
{
	random::SetSeedPID();
	
	MainData mainData;
	MainDataLoad(mainData);

	sf::Font font;
	font.loadFromFile("./Assets/Fonts/OMORI_GAME2.ttf");
	mainData.tstTex.loadFromFile("./Assets/Images/Placeholder.png");
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
	
	mainData.cam.Reset(mainData.renderWindow);
	mainData.cam.SetLimitations(sf::FloatRect(0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT));
	mainData.cam.SetZoom(1.5f);

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
	
	while (_mainData.renderWindow.pollEvent(event))
	{
		switch (event.type)
		{
			case sf::Event::Closed:
				_mainData.renderWindow.close();
				return;
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
						_mainData.renderWindow.ToggleFullscreen();
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
	
	sf::RectangleShape test(sf::Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT));
	test.setTexture(&_mainData.tstTex, true);
	test.setFillColor(sf::Color(51, 215, 110));
	_mainData.renderWindow.draw(test);

	_mainData.scenes.Draw(_mainData.renderWindow);

	_mainData.renderWindow.draw(_mainData.tp);

	_mainData.renderWindow.display();
}

