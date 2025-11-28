#include "MenuSystem.hpp"


MenuSystem::MenuHolder::MenuHolder(void) :
m_buttonSelected (-1),
m_selectionLoop  (false)
{
	
}

void MenuSystem::MenuHolder::draw(sf::RenderTarget& _target, sf::RenderStates _states) const
{
	for (const auto& btn : *this)
	{
		_target.draw(btn, _states);
	}
}



MenuSystem::MenuSystem(void) :
m_menus ()
{

}

void MenuSystem::draw(sf::RenderTarget& _target, sf::RenderStates _states) const
{
	if (!this->m_menus.empty())
	{

	}
}
