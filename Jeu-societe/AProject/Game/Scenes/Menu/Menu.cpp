#include "Menu.hpp"

void Menu::Load(void)
{
	m_data = new SceneData;
	m_data->gameData = (GameData*)this->m_keptData;
	m_data->state = MAIN_MENU;
	m_data->gameData->m_assetManager->LoadManifest("Manifests/Menu.json", "Menu");
	
	LoadUI();
}
void Menu::LoadUI(void)
{
	m_data->ui.buttonMap["playBtn"].setTexture(*m_data->gameData->m_assetManager->GetAsset<TextureAnimated>("playBtn"));
	m_data->ui.buttonMap["settingsBtn"].setTexture(*m_data->gameData->m_assetManager->GetAsset<TextureAnimated>("settingsBtn"));
	m_data->ui.buttonMap["leaveBtn"].setTexture(*m_data->gameData->m_assetManager->GetAsset<TextureAnimated>("leaveBtn"));
	m_data->ui.background.setTexture(*m_data->gameData->m_assetManager->GetAsset<sf::Texture>("background"));


	sf::FloatRect buttonRect = m_data->ui.buttonMap["playBtn"].getLocalBounds();
	m_data->ui.buttonMap["playBtn"].setPosition({ SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 });
	m_data->ui.buttonMap["settingsBtn"].setPosition({ SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + buttonRect.height});
	m_data->ui.buttonMap["leaveBtn"].setPosition({ SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 2 * buttonRect.height });
	m_data->ui.buttonMap["playBtn"].setOrigin({ 0.5f,0.5f });
	m_data->ui.buttonMap["settingsBtn"].setOrigin({ 0.5f,0.5f });
	m_data->ui.buttonMap["leaveBtn"].setOrigin({ 0.5f,0.5f });
}
void Menu::Unload(void)
{

}

void Menu::PollEvent(sf::Event& _event)
{
	ButtonsPollEvent(_event);
}
void Menu::ButtonsPollEvent(sf::Event& _event)
{
	m_data->ui.buttonMap["playBtn"].PollEvent(_event);
	m_data->ui.buttonMap["settingsBtn"].PollEvent(_event);
	m_data->ui.buttonMap["leaveBtn"].PollEvent(_event);
}

void Menu::Update(float _deltaTime)
{
	ButtonsUpdate(_deltaTime);
}
void Menu::ButtonsUpdate(float _dt)
{
	m_data->ui.buttonMap["playBtn"].Update(_dt);
	m_data->ui.buttonMap["settingsBtn"].Update(_dt);
	m_data->ui.buttonMap["leaveBtn"].Update(_dt);

	switch (m_data->state)
	{
	case MAIN_MENU:
		break;
	case PLAYER_SELECTION:
		break;
	}



	if (m_data->ui.buttonMap["playBtn"].HasBeenClicked())
	{
		m_data->state = PLAYER_SELECTION;
	}
	else if (m_data->ui.buttonMap["settingsBtn"].HasBeenClicked())
	{

	}
}

void Menu::Draw(sf::RenderWindow& _renderWindow)
{
	_renderWindow.draw(m_data->ui.background);
	ButtonsDraw(_renderWindow);
}
void Menu::ButtonsDraw(sf::RenderWindow& _renderWindow)
{
	_renderWindow.draw(m_data->ui.buttonMap["playBtn"]);
	_renderWindow.draw(m_data->ui.buttonMap["settingsBtn"]);
	_renderWindow.draw(m_data->ui.buttonMap["leaveBtn"]);

	sf::Vector2i mousePos = sf::Mouse::getPosition();
	if (m_data->ui.buttonMap["leaveBtn"].IsClicked(mousePos.x, mousePos.y))
	{
		_renderWindow.close();
	}
}
