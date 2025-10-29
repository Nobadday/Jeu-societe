#pragma once

#ifndef _INC_SCENE_HPP
#define _INC_SCENE_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <vector>
#include <iostream>


struct SceneEvent
{
	enum Type
	{
		NONE,
		LOAD_STRING,
		LOAD_INDEX
	};
	bool shouldListen = false;
	Type type = NONE;

	std::string targetScene;
	int targetSceneIndex = -1;
	bool keepLoaded = false;
};

// Inherit and override me
class SceneBase
{
	private:
		SceneEvent m_sceneAction;
	
	protected:
		void* m_keptData;

	public:
		SceneEvent& FetchActions(void);
		void SetKeptData(void* _data);
		// If keepLoaded is true, the current scene should stay loaded
		void ChangeScene(const std::string& _sceneName, bool _keepLoaded = false);
		void ChangeScene(int _index, bool _keepLoaded = false);

		virtual void Load(void) = 0;
		virtual void Unload(void) = 0;
		virtual void PollEvent(sf::Event& _event) = 0;
		virtual void Update(float _deltaTime) = 0;
		virtual void Draw(sf::RenderWindow& _renderWindow) = 0;
};



//class SceneExample : public SceneBase
//{
//	private:
//		struct SceneData
//		{
//			int num = 10;
//			sf::Sprite clsS;
//		};
//		SceneData* m_data;
//
//	public:
//		virtual void Load(void)
//		{
//			m_data = new SceneData;
//			m_data->clsS.setPosition(100, 0);
//		}
//		virtual void Unload(void)
//		{
//			delete this->m_data;
//			this->m_data = NULL;
//		}
//
//		virtual void PollEvent(sf::Event& _event)
//		{
//		}
//		virtual void Update(float _deltaTime)
//		{
//		}
//		virtual void Draw(sf::RenderWindow& _renderWindow)
//		{
//		}
//};

#endif
// Scene v1.0