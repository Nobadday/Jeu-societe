#include "SceneHandler.hpp"


SceneHandler::SceneHandler(void) :
m_activeScene	 (-1),
m_transferedData (NULL)
{
}
SceneHandler::~SceneHandler(void)
{
	for (int i = ((int)this->m_scenes.size()) - 1; i >= 0; i--)
	{
		SceneWrap& wrap = this->GetSceneWrap(i);
		if (wrap.isLoaded)
		{
			(wrap.object.get())->Unload();
		}
	}
	this->m_scenes.clear();
	this->m_scenes.shrink_to_fit();
}

void SceneHandler::SetTransferedData(void* _data)
{
	this->m_transferedData = _data;
	for (int i = 0; i < this->m_scenes.size(); i++)
	{
		this->GetScene(i)->SetKeptData(_data);
	}
}

void SceneHandler::SelectScene(int _index, bool _keepLoaded)
{
	// UNLOAD LAST SCENE
	if ((this->m_activeScene != -1) && !_keepLoaded)
	{
		SceneWrap& wrap = this->GetCurrentSceneWrap();
		if (wrap.isLoaded)
		{
			wrap.object->Unload();
			wrap.isLoaded = false;
		}
	}

	// LOAD NEW SCENE
	SceneWrap& wrap = this->GetSceneWrap(_index);
	if (!wrap.isLoaded)
	{
		wrap.object->Load();
		wrap.isLoaded = true;
	}

	this->m_activeScene = _index;
}
void SceneHandler::SelectScene(const std::string& _name, bool _keepLoaded)
{
	int index = this->FindSceneIndex(_name);
	if (index != -1)
	{
		this->SelectScene(index, _keepLoaded);
	}
	else
	{
		printf("[WARNING] | SceneHandler : Scene \"%s\" not found !\n", _name.c_str());
	}
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

// SceneHandler v1.2.2