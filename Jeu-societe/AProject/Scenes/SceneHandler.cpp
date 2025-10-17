#include "SceneHandler.hpp"



void SceneHandler::CheckEvents(void)
{
	if (this->m_activeScene != -1)
	{
		SceneEvent& event = this->GetCurrentScene()->FetchActions();
		if (event.shouldListen)
		{
			event.shouldListen = false;
			switch (event.type)
			{
				case SceneEvent::LOAD_STRING:
					this->SelectScene(event.targetScene, event.keepLoaded);
					break;

				case SceneEvent::LOAD_INDEX:
					this->SelectScene(event.targetSceneIndex, event.keepLoaded);
					break;

				case SceneEvent::NONE:
				default:
					break;
			}
		}
	}
}


SceneHandler::SceneHandler(void) :
m_activeScene	 (-1),
m_transferedData (NULL)
{
}
SceneHandler::~SceneHandler(void)
{
	this->m_scenes.resize(0);
}

void SceneHandler::SetTransferedData(void* _data)
{
	this->m_transferedData = _data;
}


void SceneHandler::AddScene(SceneBase& _scene, const std::string& _name)
{
	this->m_scenes.resize(this->m_scenes.size() + 1);
	SceneWrap& scene = this->m_scenes[this->m_scenes.size() - 1];

	scene.object = &_scene;
	scene.object->SetKeptData(this->m_transferedData);
	scene.name = _name;
	
	
	if (this->m_activeScene == -1)
	{
		this->SelectScene(0, false);
	}
}

void SceneHandler::SelectScene(int _index, bool _keepLoaded)
{
	if (!_keepLoaded && this->m_activeScene != -1)
	{
		SceneWrap& wrap = this->GetCurrentSceneWrap();
		wrap.object->Unload();
	}
	SceneWrap& wrap = GetSceneWrap(_index);
	wrap.object->Load();

	this->m_activeScene = _index;
}
void SceneHandler::SelectScene(const std::string& _name, bool _keepLoaded)
{
	this->SelectScene(this->FindSceneIndex(_name), _keepLoaded);
}



SceneWrap& SceneHandler::GetSceneWrap(int _index)
{
	return this->m_scenes[_index];
}
SceneWrap& SceneHandler::GetSceneWrap(const std::string& _name)
{
	return this->m_scenes[this->FindSceneIndex(_name)];
}

SceneBase*& SceneHandler::GetScene(int _index)
{
	return (SceneBase*&)GetSceneWrap(_index).object;
}

SceneWrap& SceneHandler::GetCurrentSceneWrap(void)
{
	return this->GetSceneWrap(this->m_activeScene);
}
SceneBase*& SceneHandler::GetCurrentScene(void)
{
	return this->GetScene(this->m_activeScene);
}

int SceneHandler::FindSceneIndex(const std::string& _name)
{
	for (int i = 0; i < this->m_scenes.size(); i++)
	{
		SceneWrap& wrap = this->GetSceneWrap(i);
		if (wrap.name == _name)
		{
			return i;
		}
	}
	return -1;
}




void SceneHandler::PollEvent(sf::Event& _event)
{
	if (this->m_activeScene != -1)
	{
		this->GetCurrentScene()->PollEvent(_event);
		this->CheckEvents();
	}
}
void SceneHandler::Update(float _deltaTime)
{
	if (this->m_activeScene != -1)
	{
		this->GetCurrentScene()->Update(_deltaTime);
		this->CheckEvents();
	}
}
void SceneHandler::Draw(sf::RenderWindow& _renderWindow)
{
	if (this->m_activeScene != -1)
	{
		this->GetCurrentScene()->Draw(_renderWindow);
		this->CheckEvents();
	}
}

// SceneHandler v1.0