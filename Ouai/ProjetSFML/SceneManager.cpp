#include "SceneManager.hpp"
#include "Scene1.hpp"
#include "Scene2.hpp"


SceneManager::SceneManager(sf::RenderWindow& _window)
	: m_window(_window)
{
	//m_window = _window;
}

SceneManager::~SceneManager()
{
	m_currentScene = NONE;
	if (sceneData != nullptr)
	{
		delete sceneData;
	}
}


void SceneManager::SetScene(Scene _newScene)
{
	if (m_currentScene != _newScene)
	{
		// Cleanup current scene	
		if (m_currentScene != NONE)
		{
			delete sceneData;
		}

		//Load new scene
		m_currentScene = _newScene;
		switch (_newScene)
		{			
			case NONE:
				break;
			case SCENE_1:
				sceneData = new Scene1(m_window);
				break;
			case SCENE_2:
				sceneData = new Scene2();
				break;
			case GAME_OVER:
				break;
		}

	}
}
Scene SceneManager::GetScene()
{
	return m_currentScene;
}

void SceneManager::PollEvent(sf::Event& _event)
{
	Scene tempScene = m_currentScene;


	if (_event.type == sf::Event::KeyPressed)
	{
		switch (_event.key.code)
		{
			//Game
			case sf::Keyboard::Key::Numpad8:
				SetScene(SCENE_1);				
				break;
			//Menu
			case sf::Keyboard::Key::Numpad7:
				SetScene(SCENE_2);
				break;
			
			//Game Over
			case sf::Keyboard::Key::Numpad9:
				//SetScene(GAME);				
				break;
			default:
				break;
		}
	}


	switch (m_currentScene)
	{
	case NONE:
		break;
	case SCENE_1:
		tempScene = ((Scene1*)sceneData)->PollEvent(_event);
		break;
	case SCENE_2:
		tempScene = ((Scene2*)sceneData)->PollEvent(_event);
		break;
	case GAME_OVER:
		break;
	default:
		break;
	}

	if (tempScene != m_currentScene)
	{
		SetScene(tempScene);
	}
}
void SceneManager::Update(float _dt)
{
	switch (m_currentScene)
	{
	case NONE:
		break;
	case SCENE_1:
		((Scene1*)sceneData)->Update(_dt);
		break;
	case SCENE_2:
		((Scene2*)sceneData)->Update(_dt);
		break;
	case GAME_OVER:
		break;
	}


}
void SceneManager::Draw()
{
	//m_window.clear(sf::Color::Green);

	switch (m_currentScene)
	{
	case NONE:
		break;
	case SCENE_1:
		((Scene1*)sceneData)->Draw(m_window);
		break;
	case SCENE_2:
		((Scene2*)sceneData)->Draw(m_window);
		break;
	case GAME_OVER:
		break;
	}



	//m_window.display();
}

