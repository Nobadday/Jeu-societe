#include "BW.hpp"

namespace sfMod
{

RenderWindow::RenderWindow(void) : sf::RenderWindow(),
m_isFullscreen	(false),
m_baseStyle		(0),
m_baseVideoMode	(0, 0),
m_aspectRatio	(16, 9),
m_view			(NULL)
{

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
	
}
void RenderWindow::onResize(void)
{
	// Get the new view from the RenderTexture
	sf::View newView = this->getView();
	
	sf::Vector2u windowSize = this->getSize();
	printf("%u, %u\n", windowSize.x, windowSize.y);

	sf::FloatRect viewPort(0.0f, 0.0f, 1.0f, 1.0f);


	sf::Vector2f newSize = (sf::Vector2f)this->m_renderSize;
	float widthRatio = (float)newSize.x / (float)windowSize.x;
	float heightRatio = (float)newSize.y / (float)windowSize.y;
	if (widthRatio > heightRatio)
	{
		newSize /= widthRatio;
	}
	else
	{
		newSize /= heightRatio;
	}
	printf("New Size = (%f, %f)\n", newSize.x, newSize.y);
	
	viewPort.width = (float)newSize.x / (float)windowSize.x;
	viewPort.height = (float)newSize.y / (float)windowSize.y;

	//if (windowSize.x < windowSize.y)
	//{
	//	// Width priority
	//	ratio = (float)this->m_renderSize.x / (float)windowSize.x;
	//	printf("W < Y\n");
	//	viewPort.height = (((float)this->m_renderSize.y) * ratio) / (float)windowSize.y;
	//}
	//else
	//{
	//	// Height priority
	//	ratio = (float)this->m_renderSize.y / (float)windowSize.y;
	//	printf("Height < Width\n");
	//	viewPort.width = (((float)this->m_renderSize.x) * ratio) / (float)windowSize.x;
	//}
	printf("VP : (%f, %f)\n", viewPort.width, viewPort.height);

	newView.setViewport(viewPort);

	newView.setViewport(sf::FloatRect(0.25f,0.25f,0.5f,0.5f));
	this->setView(newView);
}

void RenderWindow::ApplyIcon(void)
{
	sf::Vector2u size = this->m_icon.getSize();
	this->sf::RenderWindow::setIcon(size.x, size.y, this->m_icon.getPixelsPtr());
}

}