#include "Podium.hpp"

void Podium::Load()
{
	m_data = new SceneData;
	m_data->gameData = (GameData*)this->m_keptData;

	m_data->gameData->m_assetManager->LoadManifest("Manifests/Podium.json", "Podium");

	m_data->Podium.setTexture(*m_data->gameData->m_assetManager->GetAsset<sf::Texture>("podium", AssetManager::AssetType::TEXTURE));

	m_data->Podium.setOrigin(sf::Vector2f(m_data->Podium.getLocalBounds().width / 2, m_data->Podium.getLocalBounds().height));
	m_data->Podium.setPosition(SCREEN_WIDTH / 2, SCREEN_HEIGHT);


	if (m_data->gameData->m_gonnaPlayIndex.size() == 0)
	{
		m_data->gameData->m_gonnaPlayIndex.push_back(0);
		m_data->gameData->m_gonnaPlayIndex.push_back(1);
	}

	m_data->playerSpriteArray.resize(m_data->gameData->m_gonnaPlayIndex.size());
	m_data->playerTextArray.resize(m_data->gameData->m_gonnaPlayIndex.size());


	for (short i = 0; i < m_data->gameData->m_gonnaPlayIndex.size(); i++)
	{
		m_data->playerTextArray[i].setFont(*m_data->gameData->m_assetManager->GetAsset<sf::Font>("podiumFont", AssetManager::AssetType::FONT));
		m_data->playerTextArray[i].setOrigin(sf::Vector2f(0.5f, 0.5f));
		m_data->playerTextArray[i].setFillColor(sf::Color::White);

		m_data->playerTextArray[i].setString("Player " + std::to_string(m_data->gameData->m_gonnaPlayIndex[i] + 1));
		m_data->playerTextArray[i].setPosition({ (float)150 * (i + 1), 100.f });

		m_data->background.setTexture(*m_data->gameData->m_assetManager->GetAsset<sf::Texture>("MinigameBackground", AssetManager::AssetType::TEXTURE));
		
		
	}
}

void Podium::Unload()
{
	m_data->gameData->m_assetManager->DeleteContainer("Podium");
	delete this->m_data;
	this->m_data = NULL;
}

void Podium::PollEvent(sf::Event& _event)
{
	switch (_event.type)
	{
	case sf::Event::JoystickButtonPressed:
		break;

	default:
		break;
	}
}

void Podium::Update(float _dt)
{

}

void Podium::Draw(sf::RenderWindow& _renderWindow)
{
	_renderWindow.draw(m_data->background);
	_renderWindow.draw(m_data->Podium);

	for (auto& it : m_data->playerTextArray)
	{
		_renderWindow.draw(it);
	}

	for (auto& it : m_data->playerSpriteArray)
	{
		_renderWindow.draw(it);
	}
}