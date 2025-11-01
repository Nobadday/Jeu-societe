#ifndef _INC_MENU_SYSTEM_HPP
#define _INC_MENU_SYSTEM_HPP

#ifndef _INC_BUTTON
#include "./Button.hpp"
#endif


#include <SFML/Graphics.hpp>
#include <iostream>


class MenuSystem
{
public:
	MenuSystem();
	~MenuSystem();


	void PollEvent(sf::Event& _event);

	void PollEventAll(sf::Event& _event);

	void Update(float _deltaTime);

	void UpdateAll(float _deltaTime);

	// Adds a option category to the menu
	void MenuAdd(std::string _menuName, bool _selectionLoop);
	void MenuRemove(std::string _menuName);
	void MenuRemoveIndex(int _menuIndex);


	// Add a button to the menu category
	void MenuAddButton(std::string _menuName, std::string _buttonName, Button& _buttonREF);

	// Removes and gets a button from the menu
	Button* MenuPopButton(std::string _menuName, std::string _buttonName);
	Button* MenuPopButtonIndex(std::string _menuName, int _index);

	MenuHolder* GetMenuHolder(std::string _menuName);
	MenuHolder* GetMenuHolderIndex(int _index);
	MenuHolder* GetCurrentMenu(void);


	void SetMenu(std::string _menuName);
	void SetMenuIndex(int _index);

	void SetSelection(int _selection);
	void AddSelection(int _value);
	void ConfirmSelection(void);

	void SetSelectionAll(int _selection);
	void ResetSelection(void);
	void ResetSelectionAll(void);

	bool IsSelected(std::string _menuName);
	bool MenuExists(std::string _menuName);
	Button& GetButton(std::string _buttonName);
	Button& GetButtonIndex(int _buttonIndex);
	int GetButtonCount();
	std::string GetButtonName(int _buttonIndex);

	std::string GetCurrentMenuName();
	int GetMenuCount();

	void Draw(MenuSystem* _menuSystem, sf::RenderWindow& _renderWindow, sf::RenderStates& _states);

	void DrawName(MenuSystem* _menuSystem, std::string _menuName, sf::RenderWindow& _renderWindow, sf::RenderStates& _states);

	// Draw EVERY menus contained inside the menusystem
	void DrawAll(MenuSystem* _menuSystem, sf::RenderWindow& _renderWindow, sf::RenderStates& _states);

	void Destroy(MenuSystem* _menuSystem);

	void Print(MenuSystem* _menuSystem);



private:
	std::string currentMenu;
	//Map or vector of MenuHolder
};




class MenuHolder
{
public:
	MenuHolder(bool _looping = true);
	~MenuHolder(void);

	void PollEvent(sf::Event& _event);
	void Update(float _dt);
	void Draw(sf::RenderWindow& _renderWindow, sf::RenderStates _states);

	void AddButton(std::string _name, Button& _button);
	Button& GetButton(std::string _name);
	Button& GetButton(int _value);
	int GetButtonCount(void);

	Button& PopButton(std::string _name);
	Button& PopButton(int _value);

	void AddSelection(int _value);
	void SetSelection(int _selection);
	void ResetSelection(void);

	void ConfirmSelection(void);


	void Print(void);





private:
	int m_selection;
	bool m_selectionLooping;
	//List dynamic here
	std::map<std::string, Button> m_buttons;
};



#endif // _INC_MENU_SYSTEM_HPP