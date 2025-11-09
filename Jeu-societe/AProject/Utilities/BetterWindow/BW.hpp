#pragma once
#ifndef _INC_RENDERWINDOW_PLUS_HPP
#define _INC_RENDERWINDOW_PLUS_HPP

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>


namespace sfMod
{

class RenderWindow : public sf::RenderWindow
{
	private:
		bool m_isFullscreen;
		sf::Uint32 m_baseStyle;
		sf::VideoMode m_baseVideoMode;
		sf::Vector2f m_aspectRatio;
		sf::Image m_icon;
		sf::View* m_view;

		sf::Vector2u m_renderSize;

		sf::VertexArray m_borders;
		sf::Color m_borderColor;

	public:
		enum FullscreenMode
		{
			BORDERLESS,
			FULLSCREEN
		};


	public:
		RenderWindow(void);


		void SetFullscreenMode(FullscreenMode _mode);
		void SetFullscreen(bool _condition);
		void ToggleFullscreen(void);
		
		

		void setIcon(const sf::Image& _image);
		void setIcon(const std::string& _filePath);


		// Modifies the texture and copies the contents of the window onto it
		void capture(sf::Texture& _texture);
		void capture(sf::Image& _image);
		bool Screenshot(const sf::String& _fileName);

		void ResetView(void);


		void DrawBorders(void);
		//void display(bool _drawBorders = false);

	protected:
		virtual void onCreate(void);
		virtual void onResize(void);

	private:
		void ApplyIcon(void);
};


}


#endif