#include "GameData.hpp"
#include "Bot/BotAI.hpp"

GameData::GameData() :
m_renderWindow(NULL),
m_assetManager(NULL),
m_audioEngine(NULL),
m_playerDataList(),
m_gonnaPlayIndex (),
m_winIndex()
{

}

void GameData::InitMiniGamePlayer() 
{
	m_gonnaPlayIndex.clear(); 
	m_winIndex.clear(); 
}

void GameData::SetPlayerPlaying(std::vector<int> _gonnaPlayeIndex) 
{
	m_gonnaPlayIndex = _gonnaPlayeIndex;
}
void GameData::AddPlayerPlaying(int _goneAPlayeIndex) 
{
	m_gonnaPlayIndex.push_back(_goneAPlayeIndex); 
}
void GameData::AddPlayerWin(int _winIndex)
{
	m_winIndex.push_back(_winIndex);
}

int GameData::GetPlayerIDFromJoystick(unsigned int _joystickId)
{
	for (int i = 0; i < m_playerDataList.size(); i++)
	{
		if (m_playerDataList[i].m_joystickId == _joystickId)
		{
			return i;
		}
	}
	return -1;
}

bool GameData::IsPlayerParticipating(int _playerID)
{
	for (int heheha : this->m_gonnaPlayIndex)
	{
		if (heheha == _playerID)
		{
			return true;
		}
	}
	return false;
}

