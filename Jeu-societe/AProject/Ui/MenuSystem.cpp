#include "MenuSystem.hpp"



MenuHolder::MenuHolder(void) :
m_buttonSelected (-1),
m_selectionLoop  (false)
{
	
}

void MenuHolder::PollEvent(const sf::Event& _event)
{
	for (auto& btn : *this)
	{
		btn.second.CheckEvent(_event);
		
	}
}
void MenuHolder::Update(float _deltaTime)
{
	for (auto& btn : *this)
	{
		btn.second.Update(_deltaTime);
	}
}

void MenuHolder::SetSelection(int _selection, bool _looping)
{
	int size = (int)this->size();
	if (_looping)
	{
		_selection = mathp::ModuloPositiveI(_selection, size);
	}
	else
	{
		if (_selection < 0)
		{
			_selection = size - 1;
		}
		else if (_selection >= size)
		{
			_selection = size - 1;
		}
	}
	this->m_buttonSelected = _selection;

}
void MenuHolder::AddSelection(int _value, bool _looping)
{
	this->SetSelection(this->m_buttonSelected + _value, _looping);
}

void MenuHolder::draw(sf::RenderTarget& _target, sf::RenderStates _states) const
{
	for (const auto& btn : *this)
	{
		_target.draw(btn.second, _states);
	}
}



MenuSystem::MenuSystem(void) :
m_menus		  (),
m_currentMenu ()
{

}

MenuHolder& MenuSystem::CreateMenu(const std::string& _name, bool _selectionWrap)
{
	// TODO : lol, lazy, change that later to the map create function whatever it is
	MenuHolder& holder = this->operator[](_name);
	
	if (this->m_currentMenu.empty())
	{
		this->m_currentMenu = _name;
	}
	return holder;
}

void MenuSystem::PollEvent(const sf::Event& _event)
{
	if (this->HasMenuSelected())
	{
		this->at(this->m_currentMenu).PollEvent(_event);
	}
}
void MenuSystem::Update(float _deltaTime)
{
	if (this->HasMenuSelected())
	{
		this->at(this->m_currentMenu).Update(_deltaTime);
	}
}
void MenuSystem::SetMenu(const std::string& _menuName)
{
	this->m_currentMenu = _menuName;

}
MenuHolder& MenuSystem::GetCurrentMenu(void)
{
	return this->at(this->m_currentMenu);
}
bool MenuSystem::HasMenuSelected(void)
{
	return !this->m_currentMenu.empty();
}

void MenuSystem::draw(sf::RenderTarget& _target, sf::RenderStates _states) const
{
	if (!this->m_currentMenu.empty())
	{
		_target.draw(this->at(this->m_currentMenu), _states);
	}
}
