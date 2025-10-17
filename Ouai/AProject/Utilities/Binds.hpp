#ifndef BIND_H
#define BIND_H

#include "Common.hpp"

typedef enum BindList
{
	ARMWRESTLINGR,
	ARMWRESTLINGL,
	GAME_KEY,
	GAME_OVER_KEY,
	MAX_KEY_BIND_LIST
}BindList;

typedef enum DebugBindList
{
	DEBUG_KEY_1,
	MAX_DEBUG_LIST_BIND
}DebugBindList;

typedef enum DirBindList
{
	RIGHT_DIR,
	LEFT_DIR,
	UP_DIR,
	DOWN_DIR,
	FORWARD_DIR,
	BACKWARD_DIR,
	MAX_DIR_KEY_CODE
}DirBindList;

typedef enum GamePadBindList
{
	GAMEPAD_A,
	GAMEPAD_B,
	GAMEPAD_X,
	GAMEPAD_Y,
	GAMEPAD_LB,
	GAMEPAD_RB,
	GAMEPAD_BACK,
	GAMEPAD_START,
	GMEPAD_LEFT_STICK,
	GAMEPAD_RIGHT_STICK,
	MAX_GAMEPAD_BIND_LIST
}GamePadBindList;

class Binds
{
public:
	Binds(void);
	~Binds(void);
	static void UpdateBind(float _dt);
};

sf::Vector2f GetAxis2D(unsigned int _id);
sf::Vector3f GetAxis3D(unsigned int _id);
float GetLookAxis2D(unsigned int _id);
sf::Vector3f GetLookAxis3D(unsigned int _id);
bool GetBindPressed(const BindList _keyList, bool _pressOneTime);
bool GetBindPressed(const BindList _keyList);
bool GetGamePadPressed(const GamePadBindList _bindList, unsigned int _id, bool _pressOneTime);
bool GetGamePadPressed(const GamePadBindList _bindList, unsigned int _id);
bool GetBindDirectionPressed2D(const DirBindList _dir);
bool GetBindDirectionPressed3D(const DirBindList _dir);
bool GetBindDebugPressed(const DebugBindList _debugBind, bool _pressOneTime);
bool GetBindDebugPressed(const DebugBindList _debugBind);
sf::Keyboard::Key GetKeyBind(BindList _bind);
sf::Keyboard::Key GetDirKeyBind(DirBindList _dir);
void SetNewKeyToBind(BindList _bind, sf::Keyboard::Key _newKey);
void SetNewKeyToBindDirection(DirBindList _bind, sf::Keyboard::Key _newKey);
bool GetKeyWhiteList(sf::Keyboard::Key _key);
const char* GetKeyName(sf::Keyboard::Key _key);
bool IsAzerty();
void SetKeyboardLayout(bool _azerty);
sf::Vector2f GetKeyboardAxis2D(void);
sf::Vector3f GetKeyboardAxis3D(void);
sf::Vector2f GetJoystickAxis2D(unsigned int _id);
sf::Vector3f GetJoystickAxis3D(unsigned int _id);
#endif
