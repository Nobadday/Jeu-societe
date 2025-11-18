#include "BetterWindow.hpp"

namespace sfMod
{

RenderWindow::RenderWindow(void) : sf::RenderWindow(),
m_baseVideoMode		(0, 0),
m_windowMode		(WINDOWED),

m_keyRepeat			(true),
m_framerate			(0u),
m_vsync				(false),

m_icon				(),
m_title				(""),

m_windowModeStyle	(sf::Style::Default),
m_windowModePosition(0, 0),
m_windowModeSize	(1, 1),

m_defaultView		(),
m_userView			(),
m_displayMode		(LETTERBOX),
m_displayViewport	(0, 0, 1, 1)
{

}

void RenderWindow::create(sf::VideoMode _mode, const sf::String& _title, sf::Uint32 _style)
{
	this->m_title = _title;
	this->m_baseVideoMode = _mode;
	this->m_windowModeSize.x = _mode.width;
	this->m_windowModeSize.y = _mode.height;
	this->m_defaultView.reset(sf::FloatRect(	0.0f						,	  0.0f,
											(float)this->m_windowModeSize.x	, (float)this->m_windowModeSize.y));
	
	this->m_userView = this->m_defaultView;
	
	// Position
	sf::VideoMode vid = sf::VideoMode::getDesktopMode();
	this->m_windowModePosition.x = (int)(vid.width / 2) - (int)(this->m_windowModeSize.x / 2);
	this->m_windowModePosition.y = (int)(vid.height / 2) - (int)(this->m_windowModeSize.y / 2);
	
	if (_style == sf::Style::Fullscreen)
	{
		// User wants it to be directly fullscreen
		this->m_windowModeStyle = sf::Style::Default;
		this->m_windowMode = WindowMode::FULLSCREEN;
	}
	else if ((_style == sf::Style::None) && (_mode == vid))
	{
		// User wants it to be directly borderless fullscreen
		this->m_windowModeStyle = sf::Style::Default;
		this->m_windowMode = WindowMode::BORDERLESS;
	}
	else
	{
		// User wants it to be windowed
		this->m_windowModeStyle = _style;
		this->m_windowMode = WindowMode::WINDOWED;
	}
	this->ReOpen();
}



void RenderWindow::ReOpen(void)
{
	switch (this->m_windowMode)
	{
		case WindowMode::FULLSCREEN:
			this->sf::RenderWindow::create(GetBestFullscreenMode(), this->m_title, sf::Style::Fullscreen);
			break;

		case WindowMode::BORDERLESS:
			this->sf::RenderWindow::create(sf::VideoMode::getDesktopMode(), this->m_title, sf::Style::None);
			break;

		case WindowMode::WINDOWED:
		default:
			this->sf::RenderWindow::create(sf::VideoMode(this->m_windowModeSize.x, this->m_windowModeSize.y, this->m_baseVideoMode.bitsPerPixel), this->m_title, this->m_windowModeStyle);
			this->sf::RenderWindow::setPosition(this->m_windowModePosition);
			break;
	}
	this->sf::RenderWindow::setKeyRepeatEnabled(this->m_keyRepeat);
	this->sf::RenderWindow::setFramerateLimit(this->m_framerate);
	this->sf::RenderWindow::setVerticalSyncEnabled(this->m_vsync);
	this->ApplyIcon();
	this->UpdateViewport();
}

void RenderWindow::Open(void)
{
	if (!this->isOpen())
	{
		this->ReOpen();
	}
}


void RenderWindow::SetWindowMode(WindowMode _mode)
{
	if (this->m_windowMode != _mode)
	{
		if (this->isOpen() && (this->m_windowMode == WindowMode::WINDOWED))
		{
			// Remember the old window...
			this->m_windowModePosition = this->getPosition();
			this->m_windowModeSize = this->getSize();
		}
		this->m_windowMode = _mode;
		this->ReOpen();
	}
}


void RenderWindow::SetFullscreen(bool _condition, bool _borderless)
{
	if (this->IsFullscreen() != _condition)
	{
		if (_condition)
		{
			this->SetWindowMode((WindowMode)(WindowMode::FULLSCREEN + _borderless));
		}
		else
		{
			this->SetWindowMode(WindowMode::WINDOWED);
		}
	}
}
void RenderWindow::ToggleFullscreen(bool _borderless)
{
	this->SetFullscreen(!this->IsFullscreen(), _borderless);
}

void RenderWindow::setIcon(const sf::Image& _image)
{
	this->m_icon = _image;
	this->ApplyIcon();
}
void RenderWindow::setIcon(const std::string& _filePath)
{
	this->m_icon.loadFromFile(_filePath);
	this->ApplyIcon();
}

void RenderWindow::setTitle(const sf::String& _title)
{
	this->m_title = _title;
	this->sf::RenderWindow::setTitle(this->m_title);
}
void RenderWindow::setKeyRepeatEnabled(bool _enabled)
{
	this->m_keyRepeat = _enabled;
	this->sf::RenderWindow::setKeyRepeatEnabled(_enabled);
}
void RenderWindow::setFramerateLimit(unsigned int _limit)
{
	this->m_framerate = _limit;
	this->sf::RenderWindow::setFramerateLimit(_limit);
}
void RenderWindow::setVerticalSyncEnabled(bool _enabled)
{
	this->m_vsync = _enabled;
	this->sf::RenderWindow::setVerticalSyncEnabled(_enabled);
}

void RenderWindow::setView(const sf::View& _view)
{
	this->m_userView = _view;
	this->ApplyView();
}

void RenderWindow::ResetView(void)
{
	this->setView(this->m_defaultView);
}

void RenderWindow::ResetViewVilain(void)
{
	sf::View evil = this->m_defaultView;
	evil.setViewport(sf::FloatRect(	-this->m_displayViewport.left,
									-this->m_displayViewport.top,
									this->m_displayViewport.width + this->m_displayViewport.left, 
									this->m_displayViewport.height + this->m_displayViewport.top));
	this->setView(evil);
}

void RenderWindow::SetDisplayMode(DisplayMode _mode)
{
	this->m_displayMode = _mode;
	this->UpdateViewport();
}


void RenderWindow::capture(sf::Texture& _texture)
{
	const sf::Vector2u size = this->getSize();
	_texture.create(size.x, size.y);
	//sf::Vector2u offset = this->GetRenderedPosition();
	//_texture.update(*this, offset.x, offset.y);
	_texture.update(*this, 0u, 0u);
}
void RenderWindow::capture(sf::Image& _image)
{
	sf::Texture texture;
	this->capture(texture);
	_image = texture.copyToImage();
}
bool RenderWindow::Screenshot(const sf::String& _fileName)
{
	sf::Image screenshot;
	this->capture(screenshot);
	return screenshot.saveToFile(_fileName);
}



bool RenderWindow::IsFullscreen(void)
{
	return this->m_windowMode != WindowMode::WINDOWED;
}

RenderWindow::WindowMode RenderWindow::GetWindowMode(void)
{
	return this->m_windowMode;
}

sf::Vector2u RenderWindow::GetRenderedSize(void)
{
	sf::Vector2u size = this->getSize();
	
	size.x = (unsigned)((float)size.x * this->m_displayViewport.width);
	size.y = (unsigned)((float)size.y * this->m_displayViewport.height);

	return size;
}
sf::Vector2i RenderWindow::GetRenderedPosition(void)
{
	sf::Vector2u size = this->getSize();
	return sf::Vector2i((int)((float)size.x * this->m_displayViewport.left)/2,
						(int)((float)size.y * this->m_displayViewport.top )/2);
}
sf::IntRect RenderWindow::GetRenderedRect(void)
{
	sf::Vector2u size = this->getSize();

	return sf::IntRect(	(int)((float)size.x * this->m_displayViewport.left) / 2,
						(int)((float)size.y * this->m_displayViewport.top ) / 2,
						(int)((float)size.x * this->m_displayViewport.width),
						(int)((float)size.y * this->m_displayViewport.height));
}

const sf::View& RenderWindow::getDefaultView(void) const
{
	return this->m_defaultView;
}

void RenderWindow::CorrectMousePos(sf::Vector2i& _position)
{
	sf::Vector2i offset = this->GetRenderedPosition();
	float ratio = this->m_displayViewport.width * this->m_displayViewport.height;
	//_position.x *= ratio;
	//_position.y *= ratio;
	//_position.x *= this->m_displayViewport.width;
	//_position.y *= this->m_displayViewport.height;
	sf::Vector2u size = this->getSize();
	//_position.x -= size.x /this->m_displayViewport.width;
	//_position.y -= size.y /this->m_displayViewport.height;
	printf("%f, %f, %f, %f\n", this->m_displayViewport.left, this->m_displayViewport.top, this->m_displayViewport.width, this->m_displayViewport.height);

}

const sf::VideoMode& RenderWindow::GetBestFullscreenMode(const sf::VideoMode& _screenMode)
{
	const std::vector<sf::VideoMode>& fModes = sf::VideoMode::getFullscreenModes();
	for (int i = 0; i < fModes.size(); i++)
	{
		const sf::VideoMode& mode = fModes[i];
		if (mode.isValid() && (mode == _screenMode))
		{
			return mode;
		}
	}
	return fModes[0];
}


void RenderWindow::onResize(void)
{
	if (this->m_windowMode == WindowMode::WINDOWED)
	{
		// Remember the last window mode size
		this->m_windowModeSize = this->getSize();
	}
	UpdateViewport();
}

void RenderWindow::ApplyIcon(void)
{
	// Could check pixelPTR but it prints a warning :(
	//const sf::Uint8* pixels = this->m_icon.getPixelsPtr();
	//if (pixels != NULL)
	//{
	//	sf::Vector2u size = this->m_icon.getSize();
	//	this->sf::RenderWindow::setIcon(size.x, size.y, pixels);
	//}
	sf::Vector2u size = this->m_icon.getSize();
	if ((size.x > 0) && (size.y > 0))
	{
		this->sf::RenderWindow::setIcon(size.x, size.y, this->m_icon.getPixelsPtr());
	}
}

void RenderWindow::UpdateViewport(void)
{
	sf::Vector2u windowSize = this->getSize();

	sf::FloatRect& viewPort = this->m_displayViewport;
	viewPort.left = 0.0f;
	viewPort.top = 0.0f;
	viewPort.width = 1.0f;
	viewPort.height = 1.0f;
	sf::Vector2f newSize((float)this->m_baseVideoMode.width, (float)this->m_baseVideoMode.height);

	float widthRatio = (float)newSize.x / (float)windowSize.x;
	float heightRatio = (float)newSize.y / (float)windowSize.y;


	switch (this->m_displayMode)
	{
		default:
		case DisplayMode::STRETCH:
			break;

		case DisplayMode::LETTERBOX:
			if (widthRatio > heightRatio)
			{
				newSize /= widthRatio;
				viewPort.height = (float)newSize.y / (float)windowSize.y;
				viewPort.top = 0.5f - (viewPort.height / 2.0f);
			}
			else
			{
				newSize /= heightRatio;
				viewPort.width = (float)newSize.x / (float)windowSize.x;
				viewPort.left = 0.5f - (viewPort.width / 2.0f);
			}
			break;

		case DisplayMode::PAN:
			if (widthRatio < heightRatio)
			{
				newSize /= widthRatio;
				viewPort.height = (float)newSize.y / (float)windowSize.y;
				viewPort.top = 0.5f - (viewPort.height / 2.0f);
			}
			else
			{
				newSize /= heightRatio;
				viewPort.width = (float)newSize.x / (float)windowSize.x;
				viewPort.left = 0.5f - (viewPort.width / 2.0f);
			}
			break;
	}

	ApplyView();
}

void RenderWindow::ApplyView(void)
{
	sf::View newView = this->m_userView;
	sf::FloatRect userViewport = newView.getViewport();
	userViewport.left += this->m_displayViewport.left;
	userViewport.top += this->m_displayViewport.top;
	userViewport.width *= this->m_displayViewport.width;
	userViewport.height *= this->m_displayViewport.height;
	newView.setViewport(userViewport);
	
	this->sf::RenderWindow::setView(newView);
}

}

// BetterWindow C++ for SFML 2.6.2 || v0.9