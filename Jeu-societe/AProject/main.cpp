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
	randmt::SetSeedPID();
	
	//if (!std::fstream("./Assets/Data/Papaya.papaya").good())
	//{
	//	system("start \"\" \"https://youtu.be/rtL5oMyBHPs?si=ZyJ4CO1T_Tw3I7MV&t=84\"");
	//}

	MainData mainData;
	MainDataLoad(mainData);

	sf::Font font;
	font.loadFromFile("./Assets/Fonts/tf2cprofessor.ttf");
	
	mainData.renderWindow.setIcon("./Assets/Images/Placeholder.png");
	
	mainData.tp.setFont(font);
	mainData.tp.setString("CLAP BONJOUR ! C'est Louis, le sexy, je sais pas. Très long string now. évitezdefairedesphrasessansespaceslesenfants!");
	mainData.tp.setCharacterSize(60u);
	mainData.tp.setFillColor(sf::Color::White);
	mainData.tp.setPosition(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f);
	mainData.tp.setOrigin(sf::Vector2f(0.5f, 0.5f));
	mainData.tp.SetCharactersPerLine(20);
	mainData.tp.SetAlignement(TextPlus::Alignement::CENTER);
	mainData.tp.Modify(8.5f, 24.0f, false);
	
	mainData.cam.Reset(mainData.renderWindow);
	
	mainData.renderWindow.SetFullscreenPrefered(true);
	const sf::ContextSettings& aaa = mainData.renderWindow.getSettings();
	printf("AA : %u || BBP : %u || Flags : %u || Stencil : %u\n", aaa.antialiasingLevel, aaa.depthBits, aaa.attributeFlags, aaa.stencilBits);

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


			case sf::Event::MouseMoved:
				//_mainData.tp.setPosition(event.mouseMove.x, event.mouseMove.y);
				break;

			case sf::Event::MouseButtonPressed:
			case sf::Event::MouseButtonReleased:
				_mainData.tp.setPosition(_mainData.renderWindow.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y)));
				break;

			case sf::Event::KeyPressed:
				switch (event.key.code)
				{
					case sf::Keyboard::G:
						if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1))
						{
							_mainData.renderWindow.SetScaleMode(sfMod::RenderWindow::STRETCH);
						}
						else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2))
						{
							_mainData.renderWindow.SetScaleMode(sfMod::RenderWindow::PAN);
						}
						else
						{
							_mainData.renderWindow.SetScaleMode(sfMod::RenderWindow::LETTERBOX);
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
	float dtFixed = deltaTime / (1.0f / 60.0f);
	
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
	{
		_mainData.cam.Move(-5.0f * dtFixed, 0.0f);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		_mainData.cam.Move(5.0f * dtFixed, 0.0f);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
	{
		_mainData.cam.Move(0.0f, -5.0f * dtFixed);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		_mainData.cam.Move(0.0f, 5.0f * dtFixed);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		_mainData.cam.ZoomAdd(-0.1f * dtFixed);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::E))
	{
		_mainData.cam.ZoomAdd(0.1f * dtFixed);
	}

	_mainData.scenes.Update(deltaTime);
	_mainData.tp.Update(deltaTime);
}

void Draw(MainData& _mainData)
{
	_mainData.renderWindow.clear(sf::Color::Black);

	_mainData.renderWindow.setView(_mainData.cam);
	sf::RectangleShape rect(sf::Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT));
	rect.setFillColor(sf::Color(50, 215, 100));
	_mainData.renderWindow.draw(rect);
	

	_mainData.scenes.Draw(_mainData.renderWindow);

	sf::Vector2i mp = sf::Mouse::getPosition(_mainData.renderWindow);

	_mainData.renderWindow.draw(_mainData.tp);
	
	sf::CircleShape cir(15.0f);
	cir.setFillColor(sf::Color::Red);
	cir.setPosition(_mainData.renderWindow.mapPixelToCoords(mp));
	cir.setOrigin(15.0f, 15.0f);
	_mainData.renderWindow.draw(cir);

	_mainData.renderWindow.display();
}

