#include "Common.hpp"
#include "./Animation/Graphics.hpp"
#include "./Ui/Button.hpp"
#include "./Ui/HealthBar/HealthBar.hpp"
#include "./Ui/MenuSystem.hpp"
#include <iostream>

typedef struct MainData
{
	sfMod::RenderWindow renderWindow;
	sf::Clock clock;
	SceneHandler scenes;

	TextureAnimated ta;
	MenuSystem sys;

	HealthBar hb;
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
	

	mainData.sys["Testicule"]["Fuck1"];
	mainData.sys["Testicule"]["Fuck2"];
	mainData.sys["Testicule"]["Fuck3"];
	mainData.sys["Testicule"]["Fuck4"];

	for (auto& btn : mainData.sys["Testicule"])
	{
		btn.second.setTexture(mainData.ta);
		btn.second.setPosition(random::RandomFloat(0, 500), random::RandomFloat(0, 500));
	}
	mainData.sys.SetMenu("Testicule");

	mainData.hb.SetSize(sf::Vector2f(200.0f,50.0f ));
	mainData.hb.setPosition(sf::Vector2f(200.0f, 50.0f));
	mainData.hb.SetAvoidOverflow(false);

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
	_mainData.renderWindow.SetAntiAliasing(sfMod::RenderWindow::X16);
	_mainData.renderWindow.setIcon("./Assets/Images/icon.png");
	_mainData.renderWindow.create(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Cute & Cursed", sf::Style::Default);
	_mainData.renderWindow.setKeyRepeatEnabled(false);
	_mainData.renderWindow.SetFullscreenPrefered(true);
	if (DEBUG)
	{

	}
	else
	{
		_mainData.renderWindow.SetFullscreen(true);
		_mainData.renderWindow.setVerticalSyncEnabled(true);
	}
	
	_mainData.scenes.SetTransferedData(&_mainData.gameData);
	

	_mainData.renderWindow.setSize(sf::Vector2u(200u, 300u));
	// GAME DATA
	_mainData.gameData.m_renderWindow = &_mainData.renderWindow;
	

	_mainData.ta.LoadFromFile("./Assets/ButtonPlaceholder.anim", TextureAnimated::ANIMATION_ANIM);

	
	_mainData.clock.restart();
}


void PollEvent(MainData& _mainData)
{
	sf::Event event;

	while (_mainData.renderWindow.pollEvent(event))
	{
		_mainData.sys.PollEvent(event);
		switch (event.type)
		{
			case sf::Event::Closed:
				_mainData.renderWindow.close();
				return;
				break;

			case sf::Event::KeyPressed:
				_mainData.sys.AddSelection(1);
				//_mainData.btn.Click();
				break;

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


	_mainData.scenes.Update(deltaTime);


	_mainData.sys.Update(deltaTime);
	for (auto& btn : _mainData.sys["Testicule"])
	{	
		if (btn.second.HasBeenClicked())
		{
			printf("kys%d\n", random::RandomInt(0, 69));
			_mainData.hb.SetBarCompletion(_mainData.hb.GetBarCompletion() - 0.05f);
		}
	}
}

void Draw(MainData& _mainData)
{
	_mainData.renderWindow.clear(sf::Color::Black);
	
	sf::RectangleShape rect(sf::Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT));
	rect.setFillColor(sf::Color(25,210,50));
	_mainData.renderWindow.draw(rect);

	_mainData.scenes.Draw(_mainData.renderWindow);
	
	//_mainData.renderWindow.draw(_mainData.sys);
	_mainData.renderWindow.draw(_mainData.hb);

	_mainData.renderWindow.display();
}

