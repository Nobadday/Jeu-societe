#include "BW.hpp"

namespace sfMod
{

RenderWindow::RenderWindow(void) : sf::RenderWindow(),
m_baseVideoMode		(0, 0),
m_windowMode		(WINDOWED),

m_icon				(),
m_title				(""),

m_windowModeStyle	(sf::Style::Default),
m_windowModePosition(0, 0),
m_windowModeSize	(500, 400),

m_userView			(),
m_displayMode		(LETTERBOX),
m_displayViewport	(0,0,1,1)

{

}

void RenderWindow::createCooler(sf::VideoMode _mode, const sf::String& _title, sf::Uint32 _style)
{
	this->m_title = _title;
	this->m_baseVideoMode = _mode;
	if (_style == sf::Style::Fullscreen)
	{
		this->m_windowModeStyle = sf::Style::Default;
		this->m_windowMode = WindowMode::FULLSCREEN;
	}
	else
	{
		this->m_windowModeStyle = _style;
		this->m_windowMode = WindowMode::WINDOWED;
	}
}

void RenderWindow::ReCreateExistingWindow(void)
{
	if (this->isOpen())
	{
		this->ReOpen();
	}
}

void RenderWindow::ReOpen(void)
{
	switch (this->m_windowMode)
	{
		case WindowMode::FULLSCREEN:
			this->create(sf::VideoMode::getFullscreenModes()[0], this->m_title, sf::Style::Fullscreen);
			break;

		case WindowMode::BORDERLESS:
			this->create(sf::VideoMode::getDesktopMode(), this->m_title, sf::Style::None);
			break;

		case WindowMode::WINDOWED:
		default:
			this->create(sf::VideoMode(this->m_windowModeSize.x, this->m_windowModeSize.y, this->m_baseVideoMode.bitsPerPixel), this->m_title, this->m_windowModeStyle);
			break;
	}
	ApplyIcon();
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

void RenderWindow::SetFullscreenMode(WindowMode _mode)
{
}
void RenderWindow::SetFullscreen(bool _condition)
{
	if (this->IsFullscreen() != _condition)
	{
		this->m_windowMode = (WindowMode)(_condition);
		this->ReOpen();
	}
}
void RenderWindow::ToggleFullscreen(void)
{
	this->SetFullscreen(!this->IsFullscreen());
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


void RenderWindow::capture(sf::Texture& _texture)
{
	const sf::Vector2u size = this->getSize();
	_texture.create(size.x, size.y);
	sf::Vector2u renderSize = this->GetRenderedSize();
	_texture.update(*this, renderSize.x, renderSize.y);
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


void RenderWindow::setView(const sf::View& _view)
{
	this->m_userView = _view;
	
}

void RenderWindow::ResetView(void)
{
	this->setView(this->getDefaultView());
}


bool RenderWindow::IsFullscreen(void)
{
	return this->m_windowMode != WindowMode::WINDOWED;
}

sf::Vector2u RenderWindow::GetRenderedSize(void)
{
	sf::Vector2u size = this->getSize();
	
	size.x = (unsigned)((float)size.x * this->m_displayViewport.width);
	size.y = (unsigned)((float)size.y * this->m_displayViewport.height);

	return size;
}

void RenderWindow::onCreate(void)
{
	this->sf::RenderWindow::onCreate();
	this->m_renderSize = this->getSize();
	this->ApplyIcon();
	this->onResize();
}
void RenderWindow::onResize(void)
{
	printf("Resize called\n");
	
	sf::Vector2u windowSize = this->getSize();

	sf::FloatRect& viewPort = this->m_displayViewport;
	this->m_displayViewport.left = 0.0f;
	this->m_displayViewport.top = 0.0f;
	this->m_displayViewport.width = 1.0f;
	this->m_displayViewport.height = 1.0f;

	sf::Vector2f newSize = (sf::Vector2f)this->m_renderSize;
	
	float widthRatio = (float)newSize.x / (float)windowSize.x;
	float heightRatio = (float)newSize.y / (float)windowSize.y;
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

	ApplyView();
}

void RenderWindow::ApplyIcon(void)
{
	const sf::Uint8* pixels = this->m_icon.getPixelsPtr();
	if (pixels != NULL)
	{
		sf::Vector2u size = this->m_icon.getSize();
		this->sf::RenderWindow::setIcon(size.x, size.y, pixels);
	}
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

// BetterWindow SFML || v0.0