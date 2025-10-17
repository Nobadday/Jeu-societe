#include "Binds.hpp"
#include "Windows.h"

sf::Keyboard::Key keyCode[MAX_KEY_BIND_LIST];
sf::Keyboard::Key move2D[MAX_DIR_KEY_CODE];
sf::Keyboard::Key move3D[MAX_DIR_KEY_CODE];
sf::Keyboard::Key debug[MAX_DEBUG_LIST_BIND];
bool keyPreviouslyPressed[MAX_KEY_BIND_LIST] = { false };
bool debugKeyPreviouslyPressed[MAX_DEBUG_LIST_BIND] = { false };
bool gamePadPreviouslyPressed[MAX_GAMEPAD_BIND_LIST] = { false };

Binds::Binds(void)
{
	keyCode[ARMWRESTLINGR] = sf::Keyboard::Key::D;

	keyCode[ARMWRESTLINGL] = sf::Keyboard::Key::Q;

	debug[DEBUG_KEY_1] = sf::Keyboard::Key::F1;

	SetKeyboardLayout(IsAzerty());


}

Binds::~Binds(void)
{
	for (usi i = 0; i < MAX_KEY_BIND_LIST; i++)
	{
		keyCode[i] = sf::Keyboard::Key::Unknown;
	}
}

void Binds::UpdateBind(float _dt)
{
	/*for (usi i = 0; i < MAX_DIR_KEY_CODE; i++)
	{
		if (sf::Keyboard::isKeyPressed(move[i]))
		{
			
		}
	}*/

}

sf::Vector2f GetAxis2D(unsigned int _id)
{
	sf::Vector2f axis = GetKeyboardAxis2D();
	if (axis == sf::Vector2f(0, 0))
	{
		axis = GetJoystickAxis2D(_id);
	}
	return axis;
}

sf::Vector3f GetAxis3D(unsigned int _id)
{
	sf::Vector3f axis = GetKeyboardAxis3D();
	if (axis == sf::Vector3f(0, 0, 0))
	{
		axis = sf::Vector3f(GetJoystickAxis3D(_id).x, GetJoystickAxis3D(_id).y, GetJoystickAxis3D(_id).z);
	}
	return axis;
}

float GetLookAxis2D(unsigned int _id)
{
	float axis = 0.f;
	const float deadZone = 0.2f;
	if (ABS(sf::Joystick::getAxisPosition(_id, sf::Joystick::Axis::U) / 100.f) > deadZone)
	{
		axis = -sf::Joystick::getAxisPosition(_id, sf::Joystick::Axis::U) / 100.f;
	}
	return axis;
}

sf::Vector3f GetLookAxis3D(unsigned int _id)
{
	sf::Vector3f axis(0,0,0);
	const float deadZone = 0.2f;
	if (ABS(sf::Joystick::getAxisPosition(_id, sf::Joystick::Axis::U) / 100.f) > deadZone)
	{
		axis.y = -sf::Joystick::getAxisPosition(_id, sf::Joystick::Axis::U) / 100.f;
	}
	if (ABS(sf::Joystick::getAxisPosition(_id, sf::Joystick::Axis::V) / 100.f) > deadZone)
	{
		axis.x = -sf::Joystick::getAxisPosition(_id, sf::Joystick::Axis::V) / 100.f;
	}
	return axis;
}

bool GetBindPressed(const BindList _keyList, bool _pressOneTime)
{
	if (_pressOneTime)
	{
		if (sf::Keyboard::isKeyPressed(keyCode[_keyList]))
		{
			if (keyPreviouslyPressed[_keyList])
			{
				return false;
			}
			else
			{
				keyPreviouslyPressed[_keyList] = true;
				return true;
			}
		}
		else
		{
			keyPreviouslyPressed[_keyList] = false;
		}
	}
	else
	{
		if (sf::Keyboard::isKeyPressed(keyCode[_keyList]))
		{
			return true;
		}
	}
	return false;
}

bool GetBindPressed(const BindList _keyList)
{
	return GetBindPressed(_keyList, false);
}

bool GetGamePadPressed(const GamePadBindList _bindList, unsigned int _id, bool _pressOneTime)
{
	if (_pressOneTime)
	{
		if (sf::Joystick::isButtonPressed(_id, _bindList))
		{
			if (gamePadPreviouslyPressed[_bindList])
			{
				return false;
			}
			else
			{
				gamePadPreviouslyPressed[_bindList] = true;
				return true;
			}
		}
		else
		{
			gamePadPreviouslyPressed[_bindList] = false;
		}
	}
	else
	{
		if (sf::Joystick::isButtonPressed(_id, _bindList)) return true;
	}
	return false;
}

bool GetGamePadPressed(const GamePadBindList _bindList, unsigned int _id)
{
	return GetGamePadPressed(_bindList, _id, false);
}

bool GetBindDirectionPressed2D(const DirBindList _dir)
{
	return sf::Keyboard::isKeyPressed(move2D[_dir]) ? true : false;
}

bool GetBindDirectionPressed3D(const DirBindList _dir)
{
	return sf::Keyboard::isKeyPressed(move3D[_dir]) ? true : false;
}

bool GetBindDebugPressed(const DebugBindList _debugBind, bool _pressOneTime)
{
	if (_pressOneTime)
	{
		if (sf::Keyboard::isKeyPressed(debug[_debugBind]))
		{
			if (debugKeyPreviouslyPressed[_debugBind]) return false;
			else
			{
				debugKeyPreviouslyPressed[_debugBind] = true;
				return true;
			}
		}
		else
		{
			debugKeyPreviouslyPressed[_debugBind] = false;
		}
	}
	else
	{
		if (sf::Keyboard::isKeyPressed(debug[_debugBind])) return true;
	}
	return false;
}

bool GetBindDebugPressed(const DebugBindList _debugBind)
{
	return GetBindDebugPressed(_debugBind, false);
}

void SetNewKeyToBind(BindList _bind, sf::Keyboard::Key _newKey)
{
	keyCode[_bind] = _newKey;
}
void SetNewKeyToBindDirection(DirBindList _bind, sf::Keyboard::Key _newKey)
{
	move2D[_bind] = _newKey;
}

sf::Keyboard::Key GetKeyBind(BindList _bind)
{
	return  keyCode[_bind];
}

sf::Keyboard::Key GetDirKeyBind(DirBindList _dir)
{
	return move2D[_dir];
}

const char* GetKeyName(sf::Keyboard::Key _key)
{
	switch (_key)
	{
	case sf::Keyboard::Key::A: return "A";
	case sf::Keyboard::Key::B: return "B";
	case sf::Keyboard::Key::C: return "C";
	case sf::Keyboard::Key::D: return "D";
	case sf::Keyboard::Key::E: return "E";
	case sf::Keyboard::Key::F: return "F";
	case sf::Keyboard::Key::G: return "G";
	case sf::Keyboard::Key::H: return "H";
	case sf::Keyboard::Key::I: return "I";
	case sf::Keyboard::Key::J: return "J";
	case sf::Keyboard::Key::K: return "K";
	case sf::Keyboard::Key::L: return "L";
	case sf::Keyboard::Key::M: return "M";
	case sf::Keyboard::Key::N: return "N";
	case sf::Keyboard::Key::O: return "O";
	case sf::Keyboard::Key::P: return "P";
	case sf::Keyboard::Key::Q: return "Q";
	case sf::Keyboard::Key::R: return "R";
	case sf::Keyboard::Key::S: return "S";
	case sf::Keyboard::Key::T: return "T";
	case sf::Keyboard::Key::U: return "U";
	case sf::Keyboard::Key::V: return "V";
	case sf::Keyboard::Key::W: return "W";
	case sf::Keyboard::Key::X: return "X";
	case sf::Keyboard::Key::Y: return "Y";
	case sf::Keyboard::Key::Z: return "Z";
	case sf::Keyboard::Key::Space: return "Space";
	case sf::Keyboard::Key::Enter: return "Enter";
	case sf::Keyboard::Key::Left: return "Left Arrow";
	case sf::Keyboard::Key::Right: return "Right Arrow";
	case sf::Keyboard::Key::Up: return "Up Arrow";
	case sf::Keyboard::Key::Down: return "Down Arrow";
	default: return "Unknown";
	}
}

bool GetKeyWhiteList(sf::Keyboard::Key _key)
{
	switch (_key)
	{
	case sf::Keyboard::Key::A: return true;
	case sf::Keyboard::Key::B: return true;
	case sf::Keyboard::Key::C: return true;
	case sf::Keyboard::Key::D: return true;
	case sf::Keyboard::Key::E: return true;
	case sf::Keyboard::Key::F: return true;
	case sf::Keyboard::Key::G: return true;
	case sf::Keyboard::Key::H: return true;
	case sf::Keyboard::Key::I: return true;
	case sf::Keyboard::Key::J: return true;
	case sf::Keyboard::Key::K: return true;
	case sf::Keyboard::Key::L: return true;
	case sf::Keyboard::Key::M: return true;
	case sf::Keyboard::Key::N: return true;
	case sf::Keyboard::Key::O: return true;
	case sf::Keyboard::Key::P: return true;
	case sf::Keyboard::Key::Q: return true;
	case sf::Keyboard::Key::R: return true;
	case sf::Keyboard::Key::S: return true;
	case sf::Keyboard::Key::T: return true;
	case sf::Keyboard::Key::U: return true;
	case sf::Keyboard::Key::V: return true;
	case sf::Keyboard::Key::W: return true;
	case sf::Keyboard::Key::X: return true;
	case sf::Keyboard::Key::Y: return true;
	case sf::Keyboard::Key::Z: return true;
	case sf::Keyboard::Key::Space: return true;
	case sf::Keyboard::Key::Enter: return true;
	case sf::Keyboard::Key::Left: return true;
	case sf::Keyboard::Key::Right: return true;
	case sf::Keyboard::Key::Up: return true;
	case sf::Keyboard::Key::Down: return true;
	default: return false;
	}
}

bool IsAzerty()
{
	HKL layout = GetKeyboardLayout(0);
	switch (LOWORD(layout))
	{
	case 0x040c: // Français (France)
	case 0x080c: // Français (Belgique)
	case 0x0c0c: // Français (Canada)
		return true; // AZERTY
	default:
		return false; // QWERTY ou autre
	}
	// 0x040c = Français (France), 0x0809 = Anglais (UK), 0x0409 = Anglais (US)
}

void SetKeyboardLayout(bool _azerty)
{
	if (_azerty)
	{
		move2D[RIGHT_DIR] = sf::Keyboard::Key::D;
		move2D[LEFT_DIR] = sf::Keyboard::Key::Q;
		move2D[UP_DIR] = sf::Keyboard::Key::Z;
		move2D[DOWN_DIR] = sf::Keyboard::Key::S;
		move3D[FORWARD_DIR] = sf::Keyboard::Key::Z;
		move3D[BACKWARD_DIR] = sf::Keyboard::Key::S;
		move3D[RIGHT_DIR] = sf::Keyboard::Key::D;
		move3D[LEFT_DIR] = sf::Keyboard::Key::Q;
		move3D[UP_DIR] = sf::Keyboard::Key::E;
		move3D[DOWN_DIR] = sf::Keyboard::Key::A;
	}
	else { // QWERTY
		move2D[RIGHT_DIR] = sf::Keyboard::Key::D;
		move2D[LEFT_DIR] = sf::Keyboard::Key::A;
		move2D[UP_DIR] = sf::Keyboard::Key::W;
		move2D[DOWN_DIR] = sf::Keyboard::Key::S;
		move3D[FORWARD_DIR] = sf::Keyboard::Key::W;
		move3D[BACKWARD_DIR] = sf::Keyboard::Key::S;
		move3D[RIGHT_DIR] = sf::Keyboard::Key::D;
		move3D[LEFT_DIR] = sf::Keyboard::Key::A;
		move3D[UP_DIR] = sf::Keyboard::Key::E;
		move3D[DOWN_DIR] = sf::Keyboard::Key::Q;
	}
}

sf::Vector2f GetKeyboardAxis2D(void)
{
	sf::Vector2f axis = sf::Vector2f(0, 0);

	if (GetBindDirectionPressed2D(LEFT_DIR))
	{
		axis.x--;
	}
	else if (GetBindDirectionPressed2D(RIGHT_DIR))
	{
		axis.x++;
	}
	if (GetBindDirectionPressed2D(UP_DIR))
	{
		axis.y--;
	}
	else if (GetBindDirectionPressed2D(DOWN_DIR))
	{
		axis.y++;
	}
	return axis;
}

sf::Vector3f GetKeyboardAxis3D(void)
{
	sf::Vector3f axis = sf::Vector3f(0, 0, 0);
	if (GetBindDirectionPressed3D(LEFT_DIR))
	{
		axis.x--;
	}
	else if (GetBindDirectionPressed3D(RIGHT_DIR))
	{
		axis.x++;
	}
	if (GetBindDirectionPressed3D(UP_DIR))
	{
		axis.y--;
	}
	else if (GetBindDirectionPressed3D(DOWN_DIR))
	{
		axis.y++;
	}
	if (GetBindDirectionPressed3D(FORWARD_DIR))
	{
		axis.z++;
	}
	else if (GetBindDirectionPressed3D(BACKWARD_DIR))
	{
		axis.z--;
	}
	return axis;
}

sf::Vector2f GetJoystickAxis2D(unsigned int _id)
{
	sf::Vector2f joyAxis(0.f, 0.f);

	if (sf::Joystick::isConnected(_id))
	{
		float x = sf::Joystick::getAxisPosition(_id, sf::Joystick::Axis::X) / 100.f;
		float y = sf::Joystick::getAxisPosition(_id, sf::Joystick::Axis::Y) / 100.f;

		const float deadZone = 0.2f;
		if (std::abs(x) > deadZone)
			joyAxis.x = x;
		if (std::abs(y) > deadZone)
			joyAxis.y = y;
	}

	return joyAxis;
}

sf::Vector3f GetJoystickAxis3D(unsigned int _id)
{
	sf::Vector3f joyAxis(0.f, 0.f, 0.f);
	if (sf::Joystick::isConnected(_id))
	{
		float x = sf::Joystick::getAxisPosition(_id, sf::Joystick::Axis::X) / 100.f;
		float y = 0;
		float z = -sf::Joystick::getAxisPosition(_id, sf::Joystick::Axis::Y) / 100.f;
		if (sf::Joystick::isButtonPressed(_id, 0)) y--; else if (sf::Joystick::isButtonPressed(_id, 8)) y++;
		const float deadZone = 0.2f;
		if (std::abs(x) > deadZone)
			joyAxis.x = x;
		if (std::abs(y) > deadZone)
			joyAxis.y = y;
		if (std::abs(z) > deadZone)
			joyAxis.z = z;
	}
	return joyAxis;
}