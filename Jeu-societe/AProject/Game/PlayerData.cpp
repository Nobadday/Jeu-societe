#include "PlayerData.hpp"

const std::string textureNameTab[PlayerData::PLAYERSKINS_COUNT]{
	"CHARACTER_1_1",
	"CHARACTER_2_1",
	"CHARACTER_3_1",
	"CHARACTER_4_1",

	"CHARACTER_1_2",
	"CHARACTER_2_2",
	"CHARACTER_3_2",
	"CHARACTER_4_2"
};

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

const std::string& PlayerData::GetTextureName(PlayerData::PlayerSkin _skin)
{
	return textureNameTab[_skin];
}
