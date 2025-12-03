#pragma once
#ifndef _INC_MENUSYSTEM_HPP
#define _INC_MENUSYSTEM_HPP


#include <SFML/Graphics.hpp>
#include <map>
#include <vector>
#include "./Button.hpp"
#include "../Utilities/MathPlus.hpp"


// A container of multiple buttons
class MenuHolder : public std::map<std::string, Button>, public sf::Drawable
{
	private:
		int m_buttonSelected;
		bool m_selectionLoop;

	public:
		MenuHolder(void);
		void PollEvent(const sf::Event& _event);
		void Update(float _deltaTime);


		void SetSelection(int _selection, bool _looping = false);
		void AddSelection(int _value, bool _looping = false);

	private:
		virtual void draw(sf::RenderTarget& _target, sf::RenderStates _states) const;
};


class MenuSystem : public sf::Drawable, public std::map<std::string, MenuHolder>
{
	private:
		std::map<std::string, MenuHolder> m_menus;
		std::string m_currentMenu;


	public:
		MenuSystem(void);

		MenuHolder& CreateMenu(const std::string& _name, bool _selectionWrap = false);

		void PollEvent(const sf::Event& _event);
		void Update(float _deltaTime);

		void SetMenu(const std::string& _menuName);
		MenuHolder& GetCurrentMenu(void);
		bool HasMenuSelected(void);

	private:
		virtual void draw(sf::RenderTarget& _target, sf::RenderStates _states) const;
};

#endif