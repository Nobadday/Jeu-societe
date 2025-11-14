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
		this->DestroyButton(name);
	}
	m_buttons.clear();
}

void MenuHolder::PollEvent(sf::Event& _event)
{
	if (_event.type == sf::Event::MouseButtonPressed)
	{
		this->ResetSelection();
	}

	//Banger
	for (auto& [name, button] : m_buttons)
	{
		button->PollEvent(_event);
	}
}
void MenuHolder::Update(float _dt)
{
	int i = 0;
	for (auto& [name, button] : m_buttons)
	{
		Button& btnREF = *button;
		if (i == m_selection)
		{
			btnREF.UpdateFrame();
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
		_renderWindow.draw(*button);
	}
}

void MenuHolder::AddButton(std::string _name, Button* _button)
{
	m_buttons.insert({ _name, _button });
}
Button& MenuHolder::CreateButton(std::string _name)
{
	Button* btnNew = new Button;
	m_buttons.insert({ _name, btnNew });
	return *btnNew;
}
Button& MenuHolder::GetButton(std::string _name)
{
	//Static object to return if error, BOUTTON SI SITUATION CACA
	static Button nullButton;

	//Ref : https://www.geeksforgeeks.org/cpp/map-find-function-in-c-stl/
	auto key = m_buttons.find(_name);

	if (key != m_buttons.end())
	{
		return *m_buttons.at(key->first);
	}
	else
	{
		std::cout << "Error, you get button who no exist, return fake button" << std::endl;
	}
	return nullButton;
}

Button& MenuHolder::GetButton(int _value)
{
	//Static object to return if error, BOUTTON SI SITUATION CACA
	static Button nullButton; 

	int i = 0;
	for (auto& [name, button] : m_buttons)
	{
		if (i == _value)
		{
			return *button;
		}
		i++;
	}
	std::cout << "Error, you get button who no exist, return fake button" << std::endl;
	return nullButton;
}

void MenuHolder::DestroyButton(std::string _name)
{
	auto key = m_buttons.find(_name);

	if (key != m_buttons.end())
	{
		Button* buttonRef = m_buttons.at(key->first);
		delete buttonRef;

		m_buttons.erase(key->first);
	}
	else
	{
		std::cout << "Error, you want destroy button who no exist, return fake button" << std::endl;
	}
}
void MenuHolder::DestroyButton(int _value)
{
	int i = 0;
	for (auto& [name, button] : m_buttons)
	{
		if (i == _value)
		{
			//Same question here, ref or copy to return
			Button* buttonRef = m_buttons.at(name);
			delete buttonRef;
			m_buttons.erase(name);
			return;
		}
		i++;
	}
	std::cout << "Error, you want destroy who no exist, return fake button" << std::endl;
}

int MenuHolder::GetButtonCount(void)
{
	return (int)m_buttons.size();
}


void MenuHolder::SetSelection(int _selection)
{
	if (_selection < 0)
	{
		if (this->m_selectionLooping)
		{
			_selection = (int)this->m_buttons.size();
		}
		else
		{
			_selection = 0;
		}
	}
	else if (_selection > (int)this->m_buttons.size())
	{
		if (this->m_selectionLooping)
		{
			_selection = 0;
		}
		else
		{
			_selection = (int)this->m_buttons.size();
		}
	}
	if (this->m_selection != -1)
	{
		Button& buttonREF = GetButton(this->m_selection);
		buttonREF.UpdateFrame();
	}
	this->m_selection = _selection;
}
void MenuHolder::AddSelection(int _value)
{
	this->SetSelection(this->m_selection + _value);
}

void MenuHolder::ConfirmSelection(void)
{
	if ((this->m_selection >= 0) && (this->m_selection < this->m_buttons.size()))
	{
		Button& butttonREF = GetButton(this->m_selection);
		//butttonREF.HasBeenClicked = true;
		this->m_selection = -1;
	}
}
#pragma endregion

#pragma region MENU_SYSTEM
MenuSystem::MenuSystem()
	: m_currentMenu("")
{
}

MenuSystem::~MenuSystem()
{
	if (this->m_currentMenu != "")
	{
		this->m_currentMenu = "";
	}

	for (int i = (int)this->m_menus.size() - 1; i >= 0; i--)
	{
		MenuHolder* menuHolderPtr = this->m_menus.at(this->GetButtonName(i));
		(*menuHolderPtr).~MenuHolder();
		delete menuHolderPtr;
	}
	this->m_menus.clear();
}

void MenuSystem::PollEvent(sf::Event& _event)
{
	MenuHolder* menu = this->m_menus.at(this->m_currentMenu);
	if (menu != NULL)
	{
		menu->PollEvent(_event);
	}
}
void MenuSystem::PollEventAll(sf::Event& _event)
{
	for (auto& [name, Menu] : this->m_menus)
	{
		Menu->PollEvent(_event);
	}
}
void MenuSystem::Update(float _deltaTime)
{
	MenuHolder* menu = this->m_menus.at(this->m_currentMenu);
	if (menu != NULL)
	{
		menu->Update(_deltaTime);
	}
}

void MenuSystem::UpdateAll(float _deltaTime)
{
	for (auto& [name, Menu] : this->m_menus)
	{
		Menu->Update(_deltaTime);
	}
}

void MenuSystem::MenuAdd(std::string _menuName, bool _selectionLoop)
{
	if (!this->MenuHolderExists(_menuName) || this->m_menus.empty())
	{
		MenuHolder* newMenuHolder = new MenuHolder(_selectionLoop);
		m_menus.insert({ _menuName, newMenuHolder });

		if (this->m_currentMenu == "")
		{
			this->SetMenuHolder( _menuName);
		}
	}
	else
	{
		std::cout << "[WARNING] Menu System : Tried to add a menu name '" << _menuName << "' that already exists." << std::endl;
	}
}




void MenuSystem::MenuRemove(std::string _menuName)
{
	if (MenuHolderExists(_menuName))
	{
		if (IsSelected(_menuName))
		{
			SetMenuHolder(_menuName);
		}
		MenuHolder& menuHolderRef = GetMenuHolder(_menuName);
		menuHolderRef.~MenuHolder();
		this->m_menus.erase(_menuName);		
	}
	else
	{
		std::cout << "[WARNING] Menu System : Tried to remove a menu name '" << _menuName << "' that doesn't exist." << std::endl;
	}
}

void MenuSystem::MenuRemove(int _menuIndex)
{
	GetMenuHolder(_menuIndex).~MenuHolder();
	//this->m_menus.erase(_menuIndex);

}

void MenuSystem::MenuAddButton(std::string _menuName, std::string _buttonName, Button* _buttonPtr)
{
	if (this->MenuHolderExists(_menuName))
	{
		this->GetMenuHolder(_menuName).AddButton(_buttonName, _buttonPtr);
	}
	else
	{
		std::cout << "[WARNING] Menu System : Tried to add a button to a menu name '" << _menuName << "' that doesn't exist." << std::endl;
		std::cout << "Anything added" << std::endl;
	}
}

void MenuSystem::MenuDeleteButton(std::string _menuName, std::string _buttonName)
{



}

void MenuSystem::MenuDeleteButtonIndex(std::string _menuName, int _index)
{
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

void MenuSystem::Draw(sf::RenderWindow& _renderWindow, sf::RenderStates _states)
{
	MenuHolder* menu = this->m_menus.at(this->m_currentMenu);
	if (menu != NULL)
	{
		menu->Draw(_renderWindow, _states);
	}
}

void MenuSystem::DrawName(MenuSystem* _menuSystem, std::string _menuName, sf::RenderWindow& _renderWindow, sf::RenderStates& _states)
{
}

void MenuSystem::DrawAll(sf::RenderWindow& _renderWindow, sf::RenderStates _states)
{
	for (auto& [name, Menu] : this->m_menus)
	{
		Menu->Draw(_renderWindow, _states);
	}
}


void MenuSystem::Print(MenuSystem* _menuSystem)
{
}

bool MenuSystem::MenuHolderExists(std::string _menuName)
{
	auto key = m_menus.find(_menuName);

	if (key != m_menus.end())
	{
		return true;
	}
	else
	{
		std::cout << "Error, you get button who no exist, return fake button" << std::endl;
	}

	return false;
}

void MenuSystem::SetMenuHolder(std::string _menuName)
{
	if (this->MenuHolderExists(_menuName))
	{
		if (this->m_currentMenu != "")
		{

		}
		if (_menuName != "")
		{
			this->m_currentMenu = _menuName;
		}
		else
		{
			this->m_currentMenu = "";
		}
	}
}

void MenuSystem::SetMenuHolder(int _index)
{
}

MenuHolder& MenuSystem::GetMenuHolder(std::string _menuName)
{
	// TODO: insérer une instruction return ici
	//temp
	return *m_menus.at(_menuName);
}

MenuHolder& MenuSystem::GetMenuHolder(int _index)
{
	int i = 0;
	for (auto& [name, menuHolder] : m_menus)
	{
		if (i == _index)
		{
			return *menuHolder;
		}
		i++;
	}
	std::cout << "Error, you get menu who no exist, return last menu can finded : " << m_menus.size() << std::endl;
	return *m_menus.end()->second;
}

MenuHolder& MenuSystem::GetCurrentMenu(void)
{
	//Fake munuholder to return if error
	static MenuHolder nullMenuHolder;



	//Check if menuSysteme have menuHolder in his map
	if (!m_currentMenu.empty())
	{
		//Check if currentMenu exist in map
		if (m_menus.find(m_currentMenu) != m_menus.end())
		{
			return *m_menus.at(m_currentMenu);
		}

		std::cout << "Current menu is not correctly set, but map not empty.\nReturned last MenuSystem on MenuHolder";
		return *m_menus.at(m_currentMenu);

	}
	std::cout << "Error, you dont have MenuSystem in map of MenuHolder\nReturned fake MenuHolder (to prevent warning)";
	return nullMenuHolder;
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
	if (this->m_currentMenu == _menuName)
	{
		return true;
	}
	return false;
}
#pragma endregion