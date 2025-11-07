#include "MenuSystem.hpp"

#pragma region MENU_HOLDER
MenuHolder::MenuHolder(bool _looping)
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
			//btnREF.UpdateTexture();
		}
		btnREF.Update(_dt);
		i++;
	}
}
void MenuHolder::Draw(sf::RenderWindow& _renderWindow, sf::RenderStates _states)
{
	//m_buttons.size();
	for (auto& [name, button] : m_buttons)
	{
		_renderWindow.draw(button);
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
	//Static object to return if error, BOUTTON SI SITUATION CACA
	static Button nullButton;


	//Ref : https://www.geeksforgeeks.org/cpp/map-find-function-in-c-stl/
	auto key = m_buttons.find(_name);

	if (key != m_buttons.end())
	{
		return m_buttons.at(key->first);
	}
	else
	{
		std::cout << "Error, you get button who no exist, return fake button" << std::endl;
	}
	return nullButton;;
}

Button& MenuHolder::GetButton(int _value)
{
	//Static object to return if error, BOUTTON SI SITUATION CACA
	static Button nullButton; 


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
	std::cout << "Error, you get button who no exist, return fake button" << std::endl;
	return nullButton;
}

Button& MenuHolder::PopButton(std::string _name)
{
	//Static object to return if error, BOUTTON SI SITUATION CACA
	static Button nullButton;


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
		std::cout << "Error, you want pop button who no exist, return fake button" << std::endl;
	}
	return nullButton;
}
Button& MenuHolder::PopButton(int _value)
{
	//Static object to return if error, BOUTTON SI SITUATION CACA
	static Button nullButton;



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
	std::cout << "Error, you want pop button who no exist, return fake button" << std::endl;
	return nullButton;
}

int MenuHolder::GetButtonCount(void)
{
	return (int)m_buttons.size();
}

std::string MenuSystem::GetButtonName(int _buttonIndex)
{
	return std::string();
}

std::string MenuSystem::GetCurrentMenuName()
{
	return std::string();
}

int MenuSystem::GetMenuCount()
{
	return 0;
}

void MenuSystem::Draw(MenuSystem* _menuSystem, sf::RenderWindow& _renderWindow, sf::RenderStates& _states)
{
}

void MenuSystem::DrawName(MenuSystem* _menuSystem, std::string _menuName, sf::RenderWindow& _renderWindow, sf::RenderStates& _states)
{
}

void MenuSystem::DrawAll(MenuSystem* _menuSystem, sf::RenderWindow& _renderWindow, sf::RenderStates& _states)
{
}

void MenuSystem::Destroy(MenuSystem* _menuSystem)
{
}

void MenuSystem::Print(MenuSystem* _menuSystem)
{
}

void MenuSystem::SetMenu(std::string _menuName)
{
}

void MenuSystem::SetMenuIndex(int _index)
{
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
		buttonREF.UpdateFrame();
	}
	m_selection = _selection;
}
void MenuHolder::AddSelection(int _value)
{
}
void MenuSystem::SetSelectionAll(int _selection)
{
}
void MenuHolder::ResetSelection(void)
{
}
void MenuSystem::ResetSelectionAll(void)
{
}
bool MenuSystem::IsSelected(std::string _menuName)
{
	return false;
}
bool MenuSystem::MenuExists(std::string _menuName)
{
	return false;
}
void MenuHolder::ConfirmSelection(void)
{
}
#pragma endregion

#pragma region MENU_SYSTEM
MenuSystem::MenuSystem()
{
}

MenuSystem::~MenuSystem()
{
}

void MenuSystem::PollEventAll(sf::Event& _event)
{
}

void MenuSystem::UpdateAll(float _deltaTime)
{
}

void MenuSystem::MenuAdd(std::string _menuName, bool _selectionLoop)
{
}

void MenuSystem::MenuRemove(std::string _menuName)
{
}

void MenuSystem::MenuRemoveIndex(int _menuIndex)
{
}

void MenuSystem::MenuAddButton(std::string _menuName, std::string _buttonName, Button& _buttonREF)
{
}

void MenuSystem::MenuDeleteButton(std::string _menuName, std::string _buttonName)
{



}

void MenuSystem::MenuDeleteButtonIndex(std::string _menuName, int _index)
{
}



MenuHolder& MenuSystem::GetMenuHolder(std::string _menuName)
{
	// TODO: insérer une instruction return ici
	//temp
	return m_menus.at(_menuName);
}

MenuHolder& MenuSystem::GetMenuHolderIndex(int _index)
{

	int i = 0;
	for (auto& [name, menuHolder] : m_menus)
	{
		if (i == _index)
		{
			return menuHolder;
		}
		i++;
	}
	std::cout << "Error, you get menu who no exist, return last menu can finded : " << m_menus.size() << std::endl;
	return m_menus.end()->second;
}

MenuHolder& MenuSystem::GetCurrentMenu(void)
{
	//Check if menuSysteme have menuHolder in his map
	if (!m_currentMenu.empty())
	{
		//Check if currentMenu exist in map
		if (m_menus.find(m_currentMenu) != m_menus.end())
		{
			return m_menus.at(m_currentMenu);
		}

		std::cout << "Current menu is not correctly set, but map not empty.\nReturned last MenuSystem on MenuHolder";
		return m_menus.at(m_currentMenu);

	}
	std::cout << "Error, you dont have MenuSystem in map of MenuHolder \n";
}
#pragma endregion