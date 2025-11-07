#include "BW.hpp"

namespace sfMod
{

RenderWindow::RenderWindow(void) : sf::RenderWindow(),
m_isFullscreen(false),
m_baseStyle(0),
m_baseVideoMode(0, 0),
m_aspectRatio(16, 9),
m_view (NULL)
{

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

}
void RenderWindow::onResize(void)
{
	
}

}