#ifndef _INC_MENU_SYSTEM_HPP
#define _INC_MENU_SYSTEM_HPP

#include "./Button.hpp"
#include <map>

class MenuHolder;

class MenuSystem
{
	public:
		MenuSystem();
		~MenuSystem();


		void PollEvent(sf::Event& _event);

		void PollEventAll(sf::Event& _event);

		void Update(float _deltaTime);

		void UpdateAll(float _deltaTime);

		void MenuAdd(std::string _menuName, bool _selectionLoop);
		void MenuRemove(std::string _menuName);
		void MenuRemove(int _menuIndex);


		void MenuAddButton(std::string _menuName, std::string _buttonName, Button& _buttonRef);
		Button& MenuCreateButton(std::string _menuName, std::string _buttonName);

		//Delete from map and destroy it, it will be unusable
		void MenuDeleteButton(std::string _menuName, std::string _buttonName);
		//Delete from map and destroy it, it will be unusable
		void MenuDeleteButtonIndex(std::string _menuName, int _index);



		MenuHolder& GetMenuHolder(std::string _menuName);
		MenuHolder& GetMenuHolder(int _index);
		MenuHolder& GetCurrentMenu(void);


		void SetMenuHolder(std::string _menuName);
		void SetMenuHolder(int _index);

		void SetSelection(int _selection);
		void AddSelection(int _value);
		void ConfirmSelection(void);

		void SetSelectionAll(int _selection);
		void ResetSelection(void);
		void ResetSelectionAll(void);

		bool IsSelected(std::string _menuName);
		Button& GetButton(std::string _buttonName);
		Button& GetButton(int _value);
		int GetButtonCount();
		std::string GetButtonName(int _buttonIndex);

		std::string GetCurrentMenuName();
		int GetMenuCount();

		void Draw(sf::RenderWindow& _renderWindow, sf::RenderStates _states);

		void DrawName(MenuSystem* _menuSystem, std::string _menuName, sf::RenderWindow& _renderWindow, sf::RenderStates& _states);

		// Draw EVERY menus contained inside the menusystem
		void DrawAll(sf::RenderWindow& _renderWindow, sf::RenderStates _states);

		void Print(MenuSystem* _menuSystem);


	private:
		std::string m_currentMenu;
		std::map< std::string, MenuHolder*> m_menus;
		bool MenuHolderExists(std::string _menuName);
};


class MenuHolder
{
	public:
		MenuHolder(bool _looping = true);
		~MenuHolder(void);

		void PollEvent(sf::Event& _event);
		void Update(float _dt);
		void Draw(sf::RenderWindow& _renderWindow, sf::RenderStates _states);

		//Create button from imported button
		void AddButton(std::string _name, Button _button);
		//Create button and return reference
		Button& CreateButton(std::string _name);
		Button& GetButton(std::string _name);
		Button& GetButton(int _value);
		int GetButtonCount(void);

		void DestroyButton(std::string _name);
		void DestroyButton(int _value);

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