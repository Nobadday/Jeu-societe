#include "Podium.hpp"

#define CONFETTI_NB 250

void Podium::Load()
{
	m_data = new SceneData;
	m_data->gameData = (GameData*)this->m_keptData;

	m_data->audio = (AudioEngine*)this->m_data->gameData->m_audioEngine;

	m_data->gameData->m_assetManager->LoadManifest("Manifests/Podium.json", "Podium");

	if (m_data->gameData->m_gonnaPlayIndex.size() == 0)
	{
		m_data->gameData->m_gonnaPlayIndex.push_back(2);
		m_data->gameData->m_gonnaPlayIndex.push_back(0);
		m_data->gameData->m_gonnaPlayIndex.push_back(3);
		m_data->gameData->m_gonnaPlayIndex.push_back(1);
	}

	m_data->playerSpriteArray.resize(m_data->gameData->m_gonnaPlayIndex.size());
	m_data->playerTextArray.resize(m_data->gameData->m_gonnaPlayIndex.size());
	m_data->podiumsSpriteArray.resize(m_data->gameData->m_gonnaPlayIndex.size());
	
	int currentPlayer = 0;
	for (short i = 0; i < m_data->gameData->m_gonnaPlayIndex.size(); i++)
	{
		m_data->background.setTexture(*m_data->gameData->m_assetManager->GetAsset<sf::Texture>("MinigameBackground", AssetManager::AssetType::TEXTURE));

		//Recup position joueur actuel (joueur 1 - 2 - 3 - 4)
		int playerPos = GetPlayerClassement(i);

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

		m_data->podiumsSpriteArray[i].setPosition(sf::Vector2f((float)SCREEN_WIDTH / (m_data->gameData->m_gonnaPlayIndex.size() + 1) * (i + 1), (float)SCREEN_HEIGHT + 2056.f / 4.f));

		sf::Vector2f position(m_data->podiumsSpriteArray[i].getPosition().x, 0);

		switch (playerPos)
		{
			case 1:
				position.y = -470;
				break;

			case 2:
				position.y = -300;
				break;

			case 3:
				position.y = -150;
				break;

			case 4:
				position.y = -1;
				break;

			default:
				break;
		}


		m_data->playerSpriteArray[i].setPosition(position);

		m_data->playerTextArray[i].SetOutline(2, sf::Color::Black);
		m_data->playerTextArray[i].setString("Player " + std::to_string(i + 1));
		m_data->playerTextArray[i].setPosition(m_data->playerSpriteArray[currentPlayer].getPosition().x, m_data->playerSpriteArray[currentPlayer].getPosition().y - m_data->playerSpriteArray[currentPlayer].getGlobalBounds().height);

		m_data->animatorArray[i].Modify(3.f, 60.f, false);
		m_data->animatorArray[i].SetGoTo(m_data->podiumsSpriteArray[i], sf::Vector2f(m_data->podiumsSpriteArray[i].getPosition().x, SCREEN_HEIGHT + 1));
		m_data->animatorArray[i].SetAnimationEasing(anim::Animator::GOTO, anim::Easing::INOUTSINE);

		currentPlayer++;
	}

	m_data->congrat.setTexture(*m_data->gameData->m_assetManager->GetAsset<sf::Texture>("congratulation", AssetManager::AssetType::TEXTURE));
	m_data->congrat.setOrigin(m_data->congrat.getLocalBounds().getSize().x / 2, 0);
	m_data->congrat.setPosition(sf::Vector2f(SCREEN_WIDTH / 2, -(m_data->congrat.getLocalBounds().height * 2)));

	m_data->confetti.setSize(sf::Vector2f(4, 4));
	m_data->confetti.setOrigin(sf::Vector2f(2, 2));

	m_data->state = PODIUM;
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
			if (m_data->state == DONE)
			{
				switch (_event.joystickButton.button)
				{
					case 0:
						ChangeScene("Lo", false);
						break;

					default:
						break;
				}
			}
			break;

		case sf::Event::KeyPressed:
			if (m_data->state == DONE)
			{
				if (sf::Keyboard::Space)
				{
					ChangeScene("Lo", false);
				}
			}

			break;

		default:
			break;
	}
}

void Podium::Update(float _dt)
{
	int playerPos = 0;
	m_data->animatorArray[0].Update(_dt);

	switch (m_data->state)
	{
		case PODIUM:
			for (short i = 0; i < m_data->podiumsSpriteArray.size(); i++)
			{
				m_data->animatorArray[i].SyncTime(m_data->animatorArray[0]);
				m_data->animatorArray[i].AnimateObject(m_data->podiumsSpriteArray[i]);

				if (m_data->animatorArray[i].IsFinished())
				{
					for (short i = 0; i < m_data->podiumsSpriteArray.size(); i++)
					{
						sf::Vector2f finalPos(m_data->playerSpriteArray[i].getPosition().x, 0.f);
						switch (GetPlayerClassement(i))
						{
						case 1:
							finalPos.y = SCREEN_HEIGHT - 470;
							break;

						case 2:
							finalPos.y = SCREEN_HEIGHT - 300;
							break;

						case 3:
							finalPos.y = SCREEN_HEIGHT - 150;
							break;

						case 4:
							finalPos.y = SCREEN_HEIGHT;
							break;

						default:
							break;
						}

						m_data->animatorArray[i].SetGoTo(m_data->playerSpriteArray[i], finalPos);
						m_data->animatorArray[i].Modify(3.f, 60.f);
						m_data->animatorArray[i].SetAnimationEasing(anim::Animator::GOTO, anim::Easing::OUTBOUNCE);
					}
					m_data->state = PLAYERS;
					return;
				}
			}
		break;

		case PLAYERS:
		{
			for (short i = 0; i < m_data->playerSpriteArray.size(); i++)
			{
				m_data->animatorArray[i].SyncTime(m_data->animatorArray[0]);
				m_data->animatorArray[i].AnimateObject(m_data->playerSpriteArray[i]);

				m_data->playerTextArray[i].setPosition(m_data->playerSpriteArray[i].getPosition() - sf::Vector2f(0, m_data->playerSpriteArray[i].getGlobalBounds().height * 0.92f));
			}

			if (m_data->animatorArray[0].IsFinished())
			{
				m_data->animatorArray[0].SetGoTo(m_data->congrat, sf::Vector2f(SCREEN_WIDTH / 2, -3 ));
				m_data->animatorArray[0].Modify(3.f, 60.f);

				int nbExplosions = m_data->playerSpriteArray.size() * 2 + 1 ;
				int patate = 1;

				for (int i = 0; i < nbExplosions; i++)
				{
					sf::Vector2f origin(SCREEN_WIDTH / (nbExplosions) * (i + 1), random::RandomFloat(SCREEN_HEIGHT * 0.1f, SCREEN_HEIGHT * 0.6f));

					
					for (int j = 0; j < CONFETTI_NB; j++)
					{
						Confetti newConfetti;

						newConfetti.position = origin;
						newConfetti.lifeTime = random::RandomFloat(1.f, 2.5f);
						newConfetti.currentLife = newConfetti.lifeTime;

						float tempAngle = rand() / float(RAND_MAX) * 2 * M_PI;
						float tempAmplitude = 1000 * acosf(rand() / float(RAND_MAX));

						newConfetti.velocity = sf::Vector2f(cosf(tempAngle) * tempAmplitude, sinf(tempAngle) * tempAmplitude);
						
						newConfetti.color = sf::Color(random::RandomInt(0, 255), random::RandomInt(0, 255), random::RandomInt(0, 255));
						m_data->confettiVector.push_back(newConfetti);
					}
					std::cout << std::endl;
				}
				m_data->state = CONGRATS;

			}
		}
		break;

		case CONGRATS:
		{
			m_data->animatorArray[0].Update(_dt);
			m_data->animatorArray[0].AnimateObject(m_data->congrat);

			if (m_data->animatorArray[0].IsFinished())
			{
				if (!m_data->patate)
				{
					for (int  i = 0; i < 5; i++)
					{
						m_data->audio->PlaySound("baloonPop");
					}
					m_data->audio->PlaySound("YaY");
					m_data->patate = true;
				}

				for (auto& it : m_data->confettiVector)
				{
					it.currentLife -= _dt;
					it.position.x += it.velocity.x * _dt;
					it.position.y += it.velocity.y * _dt;

					it.velocity.x *= 0.99f;
					it.velocity.y *= 0.99f;

					it.rotation = atan2f(it.velocity.y, it.velocity.x) * 180 / M_PI;
					it.scale = sf::Vector2f(sqrtf(it.velocity.x * it.velocity.x + it.velocity.y * it.velocity.y) * 0.01f + 1, 1);

					if (it.currentLife <= 0)
					{
						it = m_data->confettiVector.back();
						m_data->confettiVector.pop_back();
					}
				}
				if (m_data->confettiVector.size() == 0)
				{
					m_data->state = DONE;
				}
			}
		}
		break;

	default:
		break;
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

	if (m_data->state == CONGRATS)
	{
		for (auto& it : m_data->confettiVector)
		{
			Apply(it, m_data->confetti);
			_renderWindow.draw(m_data->confetti);
		}
	}
}

int Podium::GetPlayerClassement(int _i)
{
	for (short j = 0; j < m_data->gameData->m_gonnaPlayIndex.size(); j++)
	{
		if (m_data->gameData->m_gonnaPlayIndex[j] == _i)
		{
			return j + 1;
		}
	}
	return 0;
}

void Podium::Apply(Confetti& _confetti, sf::RectangleShape& _rectangle)
{
	_rectangle.setPosition(_confetti.position);
	_rectangle.setRotation(_confetti.rotation);
	_rectangle.setScale(_confetti.scale);

	_rectangle.setFillColor(_confetti.color);
}