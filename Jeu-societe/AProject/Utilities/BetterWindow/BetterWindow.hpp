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
		enum WindowMode
		{
			// Windowed mode
			WINDOWED,
			// Fullscreen mode
			FULLSCREEN,
			// Borderless fullscreen
			BORDERLESS
		};

		enum ScaleMode
		{
			// Default operation done by the SFML
			// Stretch all screen contents to the size of the window
			STRETCH,
			// Fit the displayed area inside the window
			LETTERBOX,
			// The displayed area will fill the entire window without being stretched
			PAN
		};

		enum AntiAliasing
		{
			X0 = 0u,
			X1 = 1u,
			X2 = 2u,
			X4 = 4u,
			X8 = 8u,
			X12 = 12u,
			X16 = 16u
		};

	private:

		// "Resolution" & BPP + WinMode

		sf::VideoMode m_baseVideoMode;
		WindowMode m_windowMode;
		sf::ContextSettings m_settings;
		
		// Remebered window data
		
		bool m_keyRepeat;
		unsigned int m_framerate;
		bool m_vsync;
		// Prefers borderless fullscreen mode if true, for toggles
		bool m_preferedBFMode;

		// Window Visual Properties

		sf::Image m_icon;
		sf::String m_title;
		
		// Window Mode only members

		sf::Uint32 m_windowModeStyle;
		sf::Vector2i m_windowModePosition;
		sf::Vector2u m_windowModeSize;

		// View & display

		sf::View m_defaultView;
		sf::View m_userView;

		ScaleMode m_scaleMode;
		sf::FloatRect m_displayViewport;


	public:
		RenderWindow(void);

		// Mode = resolution/BPP, style
		// style is the default style only in windowed mode
		virtual void create(sf::VideoMode _mode, const sf::String& _title, sf::Uint32 _style = sf::Style::Default);
		virtual void create(sf::VideoMode _mode, const sf::String& _title, sf::Uint32 _style, const sf::ContextSettings& _settings);

		bool pollEvent(sf::Event& _event);

		// Open the window, if it's already opened, closes it and reopens it
		virtual void ReOpen(void);

		// Opens the window only if it's closed
		virtual void Open(void);
		
		// Only Re-Opens the window if it's already opened
		void ReOpenIfOpen(void);

		// Sets the position relative to the monitor
		void SetPositionRelative(const sf::Vector2f& _coefficient, const sf::VideoMode& _screen = sf::VideoMode::getDesktopMode());
		// Sets the size relative to the size of the monitor
		// If _keepRation is True, the size will not be deformed
		void SetSizeRelative(const sf::Vector2f& _coefficient, bool _keepRatio = false, const sf::VideoMode& _screen = sf::VideoMode::getDesktopMode());

		void SetWindowMode(WindowMode _mode);
		
		// Sets the prefered fullscreen mode for the fullscreen toggles
		void SetFullscreenPrefered(bool _borderless);

		void SetFullscreen(bool _condition, bool _borderless);
		void SetFullscreen(bool _condition);

		void ToggleFullscreen(bool _borderless);
		void ToggleFullscreen(void);
		
		// Sets the Anti-Aliasing of the window
		// Warning : If the window is open it will be re-opened so set it BEFORE creating/opening the window
		void SetAntiAliasing(AntiAliasing _aliasing);

		void setIcon(const sf::Image& _image);
		void setIcon(const std::string& _filePath);

		void setTitle(const sf::String& _title);

		void setKeyRepeatEnabled(bool _enabled);
		void setFramerateLimit(unsigned int _limit);
		void setVerticalSyncEnabled(bool _enabled);

		// Set the current view, will be applied
		void setView(const sf::View& _view);

		void ResetView(void);
		void ResetViewVilain(void);

		// Resize the window to remove the borders
		void RemoveBorders(void);

		// Sets the scale mode of the window
		// Determines how elements are scaled up and rendered on screen
		void SetScaleMode(ScaleMode _mode);


		// Modifies the texture and copies the contents of the window onto it
		void capture(sf::Texture& _texture);
		// Modifies the images and copies the contents of the window onto it, more expensive than using a texture
		void capture(sf::Image& _image);
		bool Screenshot(const sf::String& _fileName);
		
		bool IsFullscreen(void);

		WindowMode GetWindowMode(void);

		const sf::View& getDefaultView(void) const;

		// Get the upper left corner of the rendered area with the scale mode applied
		sf::Vector2i GetRenderedOffset(void);
		// Get the size of the rendered area with the scale mode applied
		sf::Vector2u GetRenderedSize(void);
		// Get a rect of the rendered area with the scale mode applied
		sf::IntRect GetRenderedRect(void);

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
// BetterWindow C++ for SFML 2.6.2 || v0.9.6 (beta)
// Made by Yannou :)


// GitHub : https://github.com/Yannou-7
// LinkedIn : https://www.linkedin.com/in/yann-melisse-jf/