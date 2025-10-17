#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include "common.h"


class SceneManager
{
	public:
		SceneManager(sf::RenderWindow& _window);
		~SceneManager();
		void PollEvent(sf::Event& _event);
		void Update(float _dt);
		void Draw();

		void SetScene(Scene _newScene);
		Scene GetScene();
	private:
		Scene m_currentScene;
		void* sceneData;
		sf::RenderWindow& m_window;
};

#endif