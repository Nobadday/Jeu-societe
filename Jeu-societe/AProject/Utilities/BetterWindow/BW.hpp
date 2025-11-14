#pragma once
#ifndef _INC_RENDERWINDOW_PLUS_HPP
#define _INC_RENDERWINDOW_PLUS_HPP

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>


namespace sfMod
{

class RenderWindow : public sf::RenderWindow
{
	public:
		enum DisplayMode
		{
			// Default operation done by the SFML
			// Stretch all screen contents to the size of the window
			STRETCH,
			// Fit the displayed area inside the window
			LETTERBOX,
			// The displayed area will fill the entire window without being stretched
			PAN
		};

		enum WindowMode
		{
			// Windowed mode
			WINDOWED,
			// Fullscreen mode
			FULLSCREEN,
			// Borderless fullscreen
			BORDERLESS
		};

	private:
		WindowMode m_windowMode;

		// Resolution & BPP
		sf::VideoMode m_baseVideoMode;
		

		// Window Visual Properties

		sf::Image m_icon;
		sf::String m_title;
		
		// Window Mode only

		sf::Uint32 m_windowModeStyle;
		sf::Vector2i m_windowModePosition;
		sf::Vector2u m_windowModeSize;

		// View & display mode

		sf::View m_userView;

		DisplayMode m_displayMode;
		sf::FloatRect m_displayViewport;

		// Graphics

		sf::VertexArray m_borders;
		sf::Color m_borderColor;
		bool m_bordersNeedUpdate;

	public:
		RenderWindow(void);

		
		//virtual void create(sf::VideoMode mode, const sf::String& title, sf::Uint32 style = sf::Style::Default);
		

		// Mode = resolution/BPP, style
		// style is the default style only in windowed mode
		virtual void create(sf::VideoMode _mode, const sf::String& _title, sf::Uint32 _style = sf::Style::Default);
		//virtual void create2(sf::VideoMode _mode, const sf::String& _title, sf::Uint32 _style = sf::Style::Default);

		void ReCreateExistingWindow(void);

		// Open the window, if it's already opened, closes it and reopens it
		virtual void ReOpen(void);

		// Opens the window if it's closed
		virtual void Open(void);


		void SetWindowMode(WindowMode _mode);

		void SetFullscreenMode(WindowMode _mode);
		void SetFullscreen(bool _condition);
		void ToggleFullscreen(void);
		
		
		void setIcon(const sf::Image& _image);
		void setIcon(const std::string& _filePath);

		void setTitle(const sf::String& _title);

		// Modifies the texture and copies the contents of the window onto it
		void capture(sf::Texture& _texture);
		// Modifies the images and copies the contents of the window onto it, more expensive than using a texture
		void capture(sf::Image& _image);
		bool Screenshot(const sf::String& _fileName);


		// Set the current view, will be applied
		void setView(const sf::View& _view);


		void ResetView(void);


		void DrawBorders(void);
		//void display(bool _drawBorders = false);

		
		bool IsFullscreen(void);


		// Get the size of the rendered area
		sf::Vector2u GetRenderedSize(void);

		sf::Vector2u GetRenderedPosition(void);

	protected:
		virtual void onCreate(void);
		virtual void onResize(void);

	private:
		void ApplyIcon(void);
		void UpdateViewport(void);
		void ApplyView(void);
};


}


#endif
// BetterWindow SFML || v0.0