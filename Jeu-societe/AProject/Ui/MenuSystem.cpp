#include "MenuSystem.hpp"


MenuSystem::MenuHolder::MenuHolder(void) :
m_buttonSelected (-1),
m_selectionLoop  (false)
{
	
}

MenuSystem::MenuSystem(void) :
m_menus ()
{

}
