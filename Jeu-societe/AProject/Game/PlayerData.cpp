#include "PlayerData.hpp"


PlayerData::PlayerData():
m_joystickId(-1),
m_playerSkin(CHARACTER_1_1)
{
}

PlayerData::~PlayerData()
{
}

void PlayerData::SetJoystickID(int _id)
{
	m_joystickId = _id;
}

int PlayerData::GetJoystickId()
{
	return m_joystickId;
}

void PlayerData::SetPlayerSkin(PlayerSkin _skin)
{
	m_playerSkin = _skin;
}

PlayerData::PlayerSkin PlayerData::GetPlayerSkin()
{
	return m_playerSkin;
}