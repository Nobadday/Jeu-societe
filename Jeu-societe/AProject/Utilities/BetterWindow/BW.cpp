#include "BW.hpp"

namespace sfMod
{

RenderWindow::RenderWindow(void) : sf::RenderWindow(),
m_isFullscreen	(false),
m_fullscreenMode(BORDERLESS),
m_baseStyle		(0),
m_baseVideoMode	(0, 0),
m_aspectRatio	(16, 9),
m_view			(NULL)
{

}


void RenderWindow::SetFullscreen(bool _condition)
{
	if (this->m_isFullscreen != _condition)
	{
		if (_condition)
		{
			const std::vector<sf::VideoMode>& vModes = sf::VideoMode::getFullscreenModes();

			int valid = -1;
			for (int i = 0; i < vModes.size(); i++)
			{
				const sf::VideoMode& mode = vModes[i];
				if ((valid != -1) && mode.isValid())
				{
					valid = i;
				}
			}
			if (valid == -1)
			{
				valid = 0;
			}
			//if (this->m) Fullscreen mode :)

			const sf::VideoMode& modeTMP = sf::VideoMode::getDesktopMode();
			
			switch (this->m_fullscreenMode)
			{
				case FullscreenMode::BORDERLESS:
					this->create(modeTMP, "Uh oh!", sf::Style::None);
					break;
				case FullscreenMode::FULLSCREEN:
					this->create(vModes[valid], "Uh oh!", sf::Style::Fullscreen);
					break;
			}
			
			
		}
		else
		{
			this->create(sf::VideoMode(200,300), "fuck you", sf::Style::Default);
		}

		this->m_isFullscreen = _condition;
	}
}

void RenderWindow::ToggleFullscreen(void)
{
	this->SetFullscreen(!this->m_isFullscreen);
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
	_texture.update(*this);
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


void RenderWindow::ResetView(void)
{
	this->setView(this->getDefaultView());
}


void RenderWindow::onCreate(void)
{
	this->sf::RenderWindow::onCreate();
	this->m_renderSize = this->getSize();
	this->onResize();
}
void RenderWindow::onResize(void)
{
	// Get the new view from the RenderTexture
	sf::View newView = this->getView();
	
	sf::Vector2u windowSize = this->getSize();

	sf::FloatRect viewPort(0.0f, 0.0f, 1.0f, 1.0f);


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

	newView.setViewport(viewPort);

	this->setView(newView);
}

void RenderWindow::ApplyIcon(void)
{
	sf::Vector2u size = this->m_icon.getSize();
	this->sf::RenderWindow::setIcon(size.x, size.y, this->m_icon.getPixelsPtr());
}

}