#include "MenuSystem.hpp"

#pragma region MENU_HOLDER
MenuHolder::MenuHolder(bool _looping = true)
	: m_selectionLooping(_looping),
	m_selection(-1)
{




}
MenuHolder::~MenuHolder(void)
{
	//Destroy list

	for (auto& [name, button] : m_buttons)
	{
		button.~Button();
		//Web3school, for each in loop example
	}
	m_buttons.clear();

	//Original
	//for (int i = PyDictGetLen(_menu->buttons) - 1; i >= 0; i--)
	//{
	//	ButtonDestroy(PyDictPopIndex(_menu->buttons, i));
	//}
	//PyDictDestroy(_menu->buttons);
	//free(_menu);
}

void MenuHolder::PollEvent(sf::Event& _event)
{
	if (_event.type == sf::Event::MouseButtonPressed)
	{
		ResetSelection();
	}

	//Banger
	for (auto& [name, button] : m_buttons)
	{
		//button::checkEvent(&event);
	}
}
void MenuHolder::Update(float _dt)
{
	int i = 0;
	for (auto& [name, button] : m_buttons)
	{
		Button& btnREF = button;
		if (i == m_selection)
		{
			//Original function
			//btnREF.stateIsON = true;
			
			//¤$£ (idk how to call you) 
			//Change texture
			btnREF.UpdateTexture();
		}
		btnREF.Update(_dt);
		i++;
	}
}
void MenuHolder::Draw(sf::RenderWindow& _renderWindow, sf::RenderStates _states)
{
	m_buttons.size();
	for (auto& [name, button] : m_buttons)
	{
		button.draw(_renderWindow, _states);
	}
}

void MenuHolder::AddButton(std::string _name, Button& _button)
{
	m_buttons.insert({ _name, _button });

	//What the hell do original function

	//int btnID = PyDictFindKeyIndex(_menu->buttons, _name);
	//if (btnID != -1)
	//{
	//	ButtonDestroy(PyDictPopIndex(_menu->buttons, btnID));
	//}
	//PyDictUpdate(_menu->buttons, _name, _button);
}
Button& MenuHolder::GetButton(std::string _name)
{
	//Ref : https://www.geeksforgeeks.org/cpp/map-find-function-in-c-stl/
	auto key = m_buttons.find(_name);

	if (key != m_buttons.end())
	{
		return m_buttons.at(key->first);
	}
	else
	{
		std::cout << "Error, you get button who no exist" << std::endl;
	}
}
Button& MenuHolder::GetButton(int _value)
{
	//I think we can erase this function, map with index is bad idea no ?
	int i = 0;
	for (auto& [name, button] : m_buttons)
	{
		if (i == _value)
		{
			return button;
		}
		i++;
	}
	std::cout << "Error, you get button who no exist" << std::endl;
}

Button& MenuHolder::PopButton(std::string _name)
{
	auto key = m_buttons.find(_name);

	if (key != m_buttons.end())
	{
		//we erase the button, or the storage in map ?
		//I think, here i return destroyed button

		Button& buttonRef = m_buttons.at(key->first);
		//Button buttonRef = m_buttons.at(key->first);
		m_buttons.erase(key->first);
		return buttonRef;
	}
	else
	{
		std::cout << "Error, you want pop button who no exist" << std::endl;
	}
}
Button& MenuHolder::PopButton(int _value)
{
	int i = 0;
	for (auto& [name, button] : m_buttons)
	{
		if (i == _value)
		{
			//Same question here, ref or copy to return
			Button& buttonRef = m_buttons.at(name);
			//Button buttonRef = m_buttons.at(name);

			m_buttons.erase(name);
			return buttonRef;
		}
		i++;
	}
	std::cout << "Error, you want pop button who no exist" << std::endl;
}

int MenuHolder::GetButtonCount(void)
{
	return (int)m_buttons.size();
}

void MenuHolder::SetSelection(int _selection)
{
	if (_selection < 0)
	{
		if (m_selectionLooping)
		{
			_selection = (int)m_buttons.size();
		}
		else
		{
			_selection = 0;
		}

	}
	else if (_selection > (int)m_buttons.size())
	{
		if (m_selectionLooping)
		{
			_selection = 0;
		}
		else
		{
			_selection = (int)m_buttons.size();
		}
	}
	if (m_selection != -1)
	{
		Button& buttonREF = GetButton(m_selection);
		//Button& buttonREF = PyDictGetValue(_menu->buttons, _menu->selection);
		//buttonREF->stateIsON = sfFalse;
		 
		//ButtonUpdateTexture(buttonREF);
		buttonREF.UpdateTexture();
	}
	m_selection = _selection;
}
void MenuHolder::AddSelection(int _value)
{
}
void MenuHolder::ResetSelection(void)
{
}
void MenuHolder::ConfirmSelection(void)
{
}
#pragma endregion

