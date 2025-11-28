#include "Podium.hpp"

void Podium::Load()
{
	m_data = new SceneData;
	m_data->gameData = (GameData*)this->m_keptData;

	m_data->gameData->m_assetManager->LoadManifest("Manifests/Podium.json", "Podium");

	if (m_data->gameData->m_gonnaPlayIndex.size() == 0)
	{
		m_data->gameData->m_gonnaPlayIndex.push_back(0);
		m_data->gameData->m_gonnaPlayIndex.push_back(1);
		m_data->gameData->m_gonnaPlayIndex.push_back(2);
		m_data->gameData->m_gonnaPlayIndex.push_back(3);
	}

	m_data->playerSpriteArray.resize(m_data->gameData->m_gonnaPlayIndex.size());
	m_data->playerTextArray.resize(m_data->gameData->m_gonnaPlayIndex.size());

	for (short i = 0; i < m_data->gameData->m_gonnaPlayIndex.size(); i++)
	{
		m_data->playerTextArray[i].setFont(*m_data->gameData->m_assetManager->GetAsset<sf::Font>("podiumFont", AssetManager::AssetType::FONT));
		m_data->playerTextArray[i].setOrigin(sf::Vector2f(0.5f, 0.5f));
		m_data->playerTextArray[i].setFillColor(sf::Color::White);

		m_data->playerSpriteArray[i].setTexture(*m_data->gameData->m_assetManager->GetAsset<TextureAtlas>("CharactersPoses", AssetManager::AssetType::TEXTURE_ATLAS));
		
		std::string textureName = m_data->gameData->m_playerDataList[m_data->gameData->m_gonnaPlayIndex[i]].GetTextureName(m_data->gameData->m_playerDataList[m_data->gameData->m_gonnaPlayIndex[i]].m_playerSkin) + "_" + std::to_string(i + 1);
		std::cout << textureName << std::endl;
		m_data->playerSpriteArray[i].SetTextureFrame(textureName);


		m_data->playerTextArray[i].setString("Player " + std::to_string(m_data->gameData->m_gonnaPlayIndex[i] ));
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

	for (auto& it : m_data->playerTextArray)
	{
		_renderWindow.draw(it);
	}

	for (auto& it : m_data->playerSpriteArray)
	{
		_renderWindow.draw(it);
	}
}