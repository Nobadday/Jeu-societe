#pragma once

#ifndef _INC_SCENEHANDLER_HPP
#define _INC_SCENEHANDLER_HPP

#include "Scene.hpp"
#include <any>

struct SceneWrap
{
	std::string name;
	SceneBase* object;
};

class SceneHandler
{
	private:
		std::vector<SceneWrap> m_scenes;
		int m_activeScene;
		void* m_transferedData;

	private:
		void CheckEvents(void);
		

	public:
		SceneHandler(void);
		~SceneHandler(void);

		void SetTransferedData(void* _data);

		// Add a scene to the scene list, if it's the first one, selects it and loads it
		// SceneExample mySceneObject;
		// SceneHandler.AddScene(mySceneObject, "My Scene");
		void AddScene(SceneBase& _scene, const std::string& _name);

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
};



#endif
// SceneHandler v1.0
// TODO :
// ~Destructor
// Historique de scene chargées, fonction previousScene() pour scenebase
// Garder les scenes chargés + param pour les afficher quand même