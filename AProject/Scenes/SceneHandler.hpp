#pragma once

#ifndef _INC_SCENEHANDLER_HPP
#define _INC_SCENEHANDLER_HPP


#include <iostream>
#include <vector>
#include "Scene.hpp"


struct SceneWrap
{
	std::string name;
	bool isLoaded;
	std::shared_ptr<SceneBase> object;
};

class SceneHandler
{
	private:
		std::vector<SceneWrap> m_scenes;
		int m_activeScene;
		void* m_transferedData;

	public:
		SceneHandler(void);
		~SceneHandler(void);

		void SetTransferedData(void* _data);


		// Add a scene to the scene list, if it's the first one, selects it and loads it
		// sceneHandler.AddScene<SceneExample>("My Scene name");
		template <typename SceneType>
		void AddScene(const std::string& _name);


		void SelectScene(int _index, bool _keepLoaded = false);
		void SelectScene(const std::string& _name, bool _keepLoaded = false);

		SceneWrap& GetSceneWrap(int _index);
		SceneWrap& GetSceneWrap(const std::string& _name);

		SceneBase*& GetScene(int _index);

		SceneWrap& GetCurrentSceneWrap(void);
		SceneBase*& GetCurrentScene(void);

		int FindSceneIndex(const std::string& _name);


		void PollEvent(sf::Event& _event);
		void Update(float _deltaTime);
		void Draw(sf::RenderWindow& _renderWindow);

	private:
		void CheckEvents(void);
};


template<typename SceneType>
inline void SceneHandler::AddScene(const std::string& _name)
{
	this->m_scenes.resize(this->m_scenes.size() + 1);
	SceneWrap& scene = this->m_scenes[this->m_scenes.size() - 1];

	scene.isLoaded = false;
	scene.name = _name;

	scene.object = std::make_shared<SceneType>();
	scene.object.get()->SetKeptData(this->m_transferedData);

	if (this->m_activeScene == -1)
	{
		this->SelectScene(0, false);
	}
}


#endif
// SceneHandler v1.2

// TODO :
// Param pour afficher les scênes déja chargés
// Event pour load scene + methode load scene(_name)
// TODO ?
// Historique de scene chargées, fonction previousScene() pour scenebase
// SceneBase : Plusieurs events