#include "Podium.hpp"

void Podium::Load()
{
	m_data = new SceneData;
	m_data->gameData = (GameData*)this->m_keptData;

	m_data->gameData->m_assetManager->LoadManifest("Manifests/Podium.json", "Podium");

	if (m_data->gameData->m_gonnaPlayIndex.size() == 0)
	{
		m_data->gameData->m_gonnaPlayIndex.push_back(2);
		m_data->gameData->m_gonnaPlayIndex.push_back(0);
		m_data->gameData->m_gonnaPlayIndex.push_back(1);
		m_data->gameData->m_gonnaPlayIndex.push_back(3);
	}

	m_data->playerSpriteArray.resize(m_data->gameData->m_gonnaPlayIndex.size());
	m_data->playerTextArray.resize(m_data->gameData->m_gonnaPlayIndex.size());
	m_data->podiumsSpriteArray.resize(m_data->gameData->m_gonnaPlayIndex.size());
	
	int currentPlayer = 0;
	for (short i = 0; i < m_data->gameData->m_gonnaPlayIndex.size(); i++)
	{
		m_data->background.setTexture(*m_data->gameData->m_assetManager->GetAsset<sf::Texture>("MinigameBackground", AssetManager::AssetType::TEXTURE));

		//Recup position joueur actuel (joueur 1 - 2 - 3 - 4)
		int playerPos = 0;

		for (short j = 0; j < m_data->gameData->m_gonnaPlayIndex.size(); j++)
		{
			if (m_data->gameData->m_gonnaPlayIndex[j] == currentPlayer)
			{
				playerPos = j + 1;
			}
		}

		//Charger ressources
		m_data->playerTextArray[i].setFont(*m_data->gameData->m_assetManager->GetAsset<sf::Font>("podiumFont", AssetManager::AssetType::FONT));
		m_data->playerTextArray[i].setOrigin(sf::Vector2f(0.5f, 0.5f));
		m_data->playerTextArray[i].setFillColor(sf::Color::White);

		std::string playerTextureName;
		playerTextureName = m_data->gameData->m_playerDataList[m_data->gameData->m_gonnaPlayIndex[playerPos - 1]].GetTextureName(m_data->gameData->m_playerDataList[m_data->gameData->m_gonnaPlayIndex[playerPos - 1]].m_playerSkin);
		playerTextureName += "_" + std::to_string(playerPos);


		std::string PodiumTextureName = "Podium_";
		PodiumTextureName += std::to_string(currentPlayer + 1) + "_" + std::to_string(playerPos);

		m_data->playerSpriteArray[i].SetTexture(*m_data->gameData->m_assetManager->GetAsset<TextureAtlas>("CharactersPoses", AssetManager::AssetType::TEXTURE_ATLAS), playerTextureName);
		m_data->podiumsSpriteArray[i].setTexture(*m_data->gameData->m_assetManager->GetAsset<TextureAtlas>("Podiums", AssetManager::AssetType::TEXTURE_ATLAS));

		m_data->podiumsSpriteArray[i].SetTextureFrame(PodiumTextureName);

		m_data->playerSpriteArray[i].setOrigin(sf::Vector2f(0.5f, 1.f));
		m_data->podiumsSpriteArray[i].setOrigin(sf::Vector2f(0.5f, 1.f));

		m_data->playerSpriteArray[i].setPosition(sf::Vector2f(SCREEN_WIDTH / ((float)m_data->gameData->m_gonnaPlayIndex.size() + 1.f) * (i + 1.f), SCREEN_HEIGHT - 450.f + 150.f * (playerPos - 1.f)));
		m_data->podiumsSpriteArray[i].setPosition(sf::Vector2f(SCREEN_WIDTH / ((float)m_data->gameData->m_gonnaPlayIndex.size() + 1.f) * (i + 1.f), SCREEN_HEIGHT + 1.f));


		m_data->playerTextArray[i].SetOutline(2, sf::Color::Black);
		m_data->playerTextArray[i].setString("Player " + std::to_string(i + 1));
		m_data->playerTextArray[i].setPosition(m_data->playerSpriteArray[currentPlayer].getPosition().x, m_data->playerSpriteArray[currentPlayer].getPosition().y - m_data->playerSpriteArray[currentPlayer].getGlobalBounds().height);

		currentPlayer++;
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

	for (auto& it : m_data->podiumsSpriteArray)
	{
		_renderWindow.draw(it);
	}

	for (auto& it : m_data->playerTextArray)
	{
		_renderWindow.draw(it);
	}

	for (auto& it : m_data->playerSpriteArray)
	{
		_renderWindow.draw(it);
	}
}