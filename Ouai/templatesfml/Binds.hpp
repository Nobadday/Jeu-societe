#ifndef BIND_H
#define BIND_H

#include "Common.hpp"

typedef enum BindList
{
	A_KEY,
	X_KEY,
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

class Binds
{
public:
	Binds(void);
	~Binds(void);
	static void UpdateBind(float _dt);
};

sf::Vector2f GetAxis2D(unsigned int _id);
sf::Vector3f GetAxis3D(unsigned int _id);
bool GetBindPressed(const BindList _keyList, bool _pressOneTime);
bool GetBindPressed(const BindList _keyList);
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
