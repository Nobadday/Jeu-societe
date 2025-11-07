#include "GameData.hpp"

void GameData::InitMiniGamePlayer() 
{
	m_gonnaPlayIndex.clear(); 
	m_WinIndex.clear(); 
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
	m_WinIndex.push_back(_winIndex);
}

