#pragma once
#ifndef _INC_RENDERWINDOW_PLUS_HPP
#define _INC_RENDERWINDOW_PLUS_HPP

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

sf::RenderWindow what;


namespace sfMod
{

class RenderWindow : public sf::RenderWindow
{
	private:
		bool m_isFullscreen;
		sf::Uint32 m_baseStyle;
		sf::Vector2f m_baseSize;
		sf::Vector2f m_aspectRatio;


	public:
		RenderWindow(void);

		
};


}









#endif