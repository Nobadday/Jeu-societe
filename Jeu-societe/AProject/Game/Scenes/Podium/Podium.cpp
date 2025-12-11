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


		std::string PodiumTextureName = "PODIUM_";
		PodiumTextureName += std::to_string(currentPlayer + 1) + "_" + std::to_string(playerPos);

		m_data->playerSpriteArray[i].SetTexture(*m_data->gameData->m_assetManager->GetAsset<TextureAtlas>("CharactersPoses", AssetManager::AssetType::TEXTURE_ATLAS), playerTextureName);
		m_data->podiumsSpriteArray[i].setTexture(*m_data->gameData->m_assetManager->GetAsset<TextureAtlas>("Podium", AssetManager::AssetType::TEXTURE_ATLAS));

		m_data->podiumsSpriteArray[i].SetTextureFrame(PodiumTextureName);

		m_data->playerSpriteArray[i].setOrigin(sf::Vector2f(0.5f, 1.f));
		m_data->podiumsSpriteArray[i].setOrigin(sf::Vector2f(0.5f, 1.f));

		m_data->playerSpriteArray[i].setPosition(sf::Vector2f(SCREEN_WIDTH / (m_data->gameData->m_gonnaPlayIndex.size() + 1) * (i + 1), SCREEN_HEIGHT));
		m_data->podiumsSpriteArray[i].setPosition(sf::Vector2f(SCREEN_WIDTH / (m_data->gameData->m_gonnaPlayIndex.size() + 1) * (i + 1), SCREEN_HEIGHT + 2056 / 4));


		m_data->playerTextArray[i].SetOutline(2, sf::Color::Black);
		m_data->playerTextArray[i].setString("Player " + std::to_string(i + 1));
		m_data->playerTextArray[i].setPosition(m_data->playerSpriteArray[currentPlayer].getPosition().x, m_data->playerSpriteArray[currentPlayer].getPosition().y - m_data->playerSpriteArray[currentPlayer].getGlobalBounds().height);

		m_data->animatorArray[i].Modify(3.f, 60.f, false);
		m_data->animatorArray[i].SetGoTo(m_data->podiumsSpriteArray[i], sf::Vector2f(m_data->podiumsSpriteArray[i].getPosition().x, SCREEN_HEIGHT + 1));
		m_data->animatorArray[i].SetAnimationEasing(anim::Animator::GOTO, anim::Easing::INOUTELASTIC);

		currentPlayer++;
	}

	m_data->congrat.setTexture(*m_data->gameData->m_assetManager->GetAsset<sf::Texture>("congratulation", AssetManager::AssetType::TEXTURE));
	m_data->congrat.setOrigin(m_data->congrat.getLocalBounds().getSize().x / 2, m_data->congrat.getLocalBounds().getSize().y / 2);
	m_data->congrat.setPosition(sf::Vector2f(SCREEN_WIDTH / 2, m_data->congrat.getLocalBounds().height / 2));


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
	case sf::Event::KeyPressed:
		this->m_data->animatorArray[0].Restart();
		break;

	default:
		break;
	}
}

void Podium::Update(float _dt)
{
	//Anmiation
	for (short i = 0; i < m_data->podiumsSpriteArray.size(); i++)
	{
		m_data->animatorArray[i].Update(_dt);
		m_data->animatorArray[i].AnimateObject(m_data->podiumsSpriteArray[i]);

		int playerPos = 0;
		for (short j = 0; j < m_data->gameData->m_gonnaPlayIndex.size(); j++)
		{
			if (m_data->gameData->m_gonnaPlayIndex[j] == i)
			{
				playerPos = j + 1;
			}
		}

		const sf::Vector2f& podiumPos = m_data->podiumsSpriteArray[i].getPosition();
		sf::Vector2f playerPosition = m_data->playerSpriteArray[i].getPosition();

		switch (playerPos)
		{
			case 1:
				playerPosition.y -= 450;
				break;

			case 2:
				playerPosition.y -= 300;
				break;

			case 3:
				playerPosition.y -= 150;
				break;

			default:
				break;
		}


		if (playerPosition.y > SCREEN_HEIGHT)
		{
			playerPosition.y = SCREEN_HEIGHT;
		}

		m_data->playerSpriteArray[i].setPosition(playerPosition);
		m_data->playerTextArray[i].setPosition(playerPosition - sf::Vector2f(0, m_data->playerSpriteArray[i].getLocalBounds().getSize().y));

	}
}

void Podium::Draw(sf::RenderWindow& _renderWindow)
{
	_renderWindow.draw(m_data->background);
	_renderWindow.draw(m_data->congrat);

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