#pragma once
#ifndef _INC_MENUSYSTEM_HPP
#define _INC_MENUSYSTEM_HPP


#include <SFML/Graphics.hpp>
#include <map>
#include <vector>
#include "./Button.hpp"



class MenuSystem : public sf::Drawable
{
	public:
		class MenuHolder : public std::vector<Button>, public sf::Drawable
		{
			private:
				int m_buttonSelected;
				bool m_selectionLoop;

			public:
				MenuHolder(void);

			private:
				virtual void draw(sf::RenderTarget& _target, sf::RenderStates _states) const;

		};


	private:
		std::map<std::string, MenuHolder> m_menus;
		std::string m_currentMenu;


	public:
		MenuSystem(void);

		MenuHolder& CreateMenu(const std::string& _name, bool _selectionWrap = false);


	private:
		virtual void draw(sf::RenderTarget& _target, sf::RenderStates _states) const;
};

#endif