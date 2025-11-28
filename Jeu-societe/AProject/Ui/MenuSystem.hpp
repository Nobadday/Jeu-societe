#pragma once
#ifndef _INC_MENUSYSTEM_HPP
#define _INC_MENUSYSTEM_HPP


#include "./Button.hpp"
#include <map>



class MenuSystem
{
	private:
		class MenuHolder : public std::vector<Button>

		{
			private:
				int m_buttonSelected;
				bool m_selectionLoop;

			public:
				MenuHolder(void);

		};


	private:
		std::map<std::string, MenuHolder> m_menus;


	public:
		MenuSystem(void);


};

#endif