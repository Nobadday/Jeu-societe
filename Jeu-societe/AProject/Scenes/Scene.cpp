#include "Scene.hpp"


void SceneBase::LaunchEvent(SceneEvent::Type _type)
{
	this->m_sceneAction.type = _type;
	this->m_sceneAction.shouldListen = true;
}

SceneEvent& SceneBase::FetchActions(void)
{
	return this->m_sceneAction;
}
void SceneBase::SetKeptData(void* _data)
{
	this->m_keptData = _data;
}
void SceneBase::ChangeScene(const std::string& _sceneName, bool _keepLoaded)
{
	this->m_sceneAction.targetScene = _sceneName;
	this->m_sceneAction.keepLoaded = _keepLoaded;
	this->LaunchEvent(SceneEvent::Type::LOAD_STRING);
	
}
void SceneBase::ChangeScene(int _index, bool _keepLoaded)
{
	this->m_sceneAction.targetSceneIndex = _index;
	this->m_sceneAction.keepLoaded = _keepLoaded;
	this->LaunchEvent(SceneEvent::Type::LOAD_INDEX);
}



// Scene v1.1