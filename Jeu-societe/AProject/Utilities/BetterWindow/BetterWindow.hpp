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

		// "Resolution" & BPP + WinMode
		sf::VideoMode m_baseVideoMode;
		WindowMode m_windowMode;
		
		// Window Visual Properties

		sf::Image m_icon;
		sf::String m_title;
		
		// Window Mode only members

		sf::Uint32 m_windowModeStyle;
		sf::Vector2i m_windowModePosition;
		sf::Vector2u m_windowModeSize;

		// View & display mode

		sf::View m_defaultView;
		sf::View m_userView;

		DisplayMode m_displayMode;
		sf::FloatRect m_displayViewport;


	public:
		RenderWindow(void);


		// Mode = resolution/BPP, style
		// style is the default style only in windowed mode
		virtual void create(sf::VideoMode _mode, const sf::String& _title, sf::Uint32 _style = sf::Style::Default);


		// Open the window, if it's already opened, closes it and reopens it
		virtual void ReOpen(void);

		// Opens the window only if it's closed
		virtual void Open(void);


		void SetWindowMode(WindowMode _mode);
		
		void SetFullscreen(bool _condition, bool _borderless = false);
		void ToggleFullscreen(bool _borderless = false);
		
		
		void setIcon(const sf::Image& _image);
		void setIcon(const std::string& _filePath);

		void setTitle(const sf::String& _title);

		// Set the current view, will be applied
		void setView(const sf::View& _view);

		void ResetView(void);
		void ResetViewVilain(void);

		// Sets the display mode of the window
		// Determines how elements are rendered on screen
		void SetDisplayMode(DisplayMode _mode);


		// Modifies the texture and copies the contents of the window onto it
		void capture(sf::Texture& _texture);
		// Modifies the images and copies the contents of the window onto it, more expensive than using a texture
		void capture(sf::Image& _image);
		bool Screenshot(const sf::String& _fileName);

		
		bool IsFullscreen(void);

		WindowMode GetWindowMode(void);

		// Get the size of the rendered area
		sf::Vector2u GetRenderedSize(void);
		// Get the upper left corner of the rendered area
		sf::Vector2u GetRenderedPosition(void);


		const sf::View& getDefaultView(void) const;


		// Will attempt to get a valid videoMode matching the desktop mode to avoid weird beheaviors
		// Else, returns the first available fullscreenMode
		static const sf::VideoMode& GetBestFullscreenMode(const sf::VideoMode& _screenMode = sf::VideoMode::getDesktopMode());

	protected:
		virtual void onResize(void);

	private:
		void ApplyIcon(void);
		// Also updates the view
		void UpdateViewport(void);
		void ApplyView(void);
};


}


#endif
// BetterWindow C++ for SFML 2.6.2 || v0.9
// Made by Yannou :)


// GitHub : https://github.com/Yannou-7
// LinkedIn : https://www.linkedin.com/in/yann-melisse-jf/