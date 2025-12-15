#include "./RussianRoulette.hpp"
#include "../../../Utilities/Random.hpp"


void RussianRoulette::Load(void)
{
	m_data = new SceneData;

	m_data->gameData = (GameData*)this->m_keptData;
	m_data->audio = (AudioEngine*)m_data->gameData->m_audioEngine;
	m_data->gameData->m_assetManager->LoadManifest("Manifests/RussianRoulette.json", "RussianRoulette");


	for (int i = 0; i < (int)m_data->gameData->m_gonnaPlayIndex.size(); ++i)
	{
		int playerId = m_data->gameData->m_gonnaPlayIndex.at(i);

		m_data->players.push_back(
			{
				(short)playerId,
				(short)m_data->gameData->m_playerDataList[playerId].GetPlayerSkin(),
				true
			});

		std::cout << "player skin = " << m_data->players[i].skin << std::endl;


	}

	m_data->currentPlayer = 0;
	m_data->timerEnd.SetTimeTarget(3.f);
	m_data->bullet = random::RandomInt(1, 6);

	m_data->text.setFont(*m_data->gameData->m_assetManager->GetAsset<sf::Font>("GameFont"));
	m_data->text.setCharacterSize(40u);
	m_data->text.setOrigin({ 0.5f,0.5f });
	m_data->text.setPosition({ SCREEN_WIDTH / 2.f, 0.8 * SCREEN_HEIGHT });

	m_data->iconsChara.setTexture(*m_data->gameData->m_assetManager->GetAsset<TextureAnimated>("Icone", AssetManager::AssetType::TEXTURE_ANIMATED));
	m_data->iconsChara.setOrigin({ 0.5f,0.5f });
	m_data->iconsChara.setPosition({ SCREEN_WIDTH / 1.2f, SCREEN_HEIGHT / 2.f });
	m_data->iconsChara.setScale({ 1.2f, 1.2f });




	PlayerData::PlayerSkin skin = (PlayerData::PlayerSkin)m_data->players[0].skin;
	m_data->iconsChara.SetAnimation(skin);

	//m_data->iconsChara.SetAnimation(Player::GetTextureName((Player::PlayerSkin)m_data->players[0].skin));


	m_data->background.setTexture(*m_data->gameData->m_assetManager->GetAsset<sf::Texture>("MinigameBackground", AssetManager::AssetType::TEXTURE));

	m_data->gunSprAnim.setTexture(*m_data->gameData->m_assetManager->GetAsset<TextureAnimated>("Gun_Rien"));
	//m_data->gameState = WAITING_FOR_INPUT;



	//Icons chara
	m_data->charaAvaible.push_back("Perso1-1");
	m_data->charaAvaible.push_back("Perso2-1");
	m_data->charaAvaible.push_back("Perso3-1");
	m_data->charaAvaible.push_back("Perso4-1");

	m_data->charaAvaible.push_back("Perso1-2");
	m_data->charaAvaible.push_back("Perso2-2");
	m_data->charaAvaible.push_back("Perso3-2");
	m_data->charaAvaible.push_back("Perso4-2");



	m_data->transition.PlayTransition();
}

void RussianRoulette::Unload(void)
{
	this->m_data->gameData->m_assetManager->DeleteContainer("RussianRoulette");
	delete this->m_data;
	this->m_data = NULL;
}

void RussianRoulette::PollEvent(sf::Event& _event)
{
	int joyId = m_data->players[m_data->currentPlayer].id;

	switch (m_data->gameState)
	{
	case WAITING_FOR_INPUT:

		switch (_event.type)
		{
			//case sf::Event::KeyPressed:
		case sf::Event::JoystickButtonPressed:

			if (joyId == _event.joystickButton.joystickId)
			{
				m_data->text.setString("");

				//Every round, we change the bullet in "barilet"
				m_data->bulletUser++;
				if (m_data->bulletUser == m_data->bullet)
				{
					m_data->players[m_data->currentPlayer].isAlive = false;

					//Launch sound
					m_data->audio->PlaySound("SoundDie");
					//Set and Launch death animation
					m_data->gunSprAnim.setTexture(*m_data->gameData->m_assetManager->GetAsset<TextureAnimated>("Gun_Mort", AssetManager::AssetType::TEXTURE_ANIMATED));
					m_data->gunSprAnim.SetAnimation("ON");
				}
				else
				{
					//DEBUG
					std::cout << "vivant" << std::endl;

					//Launch sound
					m_data->audio->PlaySound("SoundNothing");
					//Set and Launch normal animation
					m_data->gunSprAnim.setTexture(*m_data->gameData->m_assetManager->GetAsset<TextureAnimated>("Gun_Rien", AssetManager::AssetType::TEXTURE_ANIMATED));
					m_data->gunSprAnim.SetAnimation("ON");
				}
				m_data->gameState = SPINNING;
			}
			break;
		default:
			break;
		}
		break;
	case SPINNING:

		break;

	case END:
		break;
	}
}
void RussianRoulette::Update(float _deltaTime)
{
	switch (m_data->gameState)
	{
	case WAITING_FOR_INPUT:

		//Attendre input
		m_data->text.setString("PRESS ANY BUTTON TO SPIN THE CHAMBER !");
		break;
	case SPINNING:

		m_data->gunSprAnim.Update(_deltaTime);

		//Animation ended
		if (m_data->gunSprAnim.IsFinished())
		{
			if (!m_data->players[m_data->currentPlayer].isAlive)
			{
				m_data->gameState = END;

				//char buffer[100];
				//std::cout << buffer << std::endl;
				std::string text = "Player die";
				m_data->text.setString(text);

				m_data->deadPlayers.push_back(m_data->players[m_data->currentPlayer]);

				//Add all other player than current to deadPlayers
				for (int i = (int)m_data->gameData->m_gonnaPlayIndex.size() - 1; i >= 0; i--)
				{
					if (i != m_data->currentPlayer)
					{
						std::cout << "id : " << m_data->players[i].id << std::endl;
						m_data->deadPlayers.push_back(m_data->players[i]);
					}
				}


				//Save data
				int nbOfPlayers = (int)m_data->gameData->m_gonnaPlayIndex.size();
				std::cout << "nb player : " << nbOfPlayers << std::endl;
				std::cout << "nb dead : " << m_data->deadPlayers.size() << std::endl;
				std::cout << "win nb player : " << m_data->gameData->m_winIndex.size() << std::endl;

				for (int i = (int)m_data->deadPlayers.size() - 1; i >= 0; i--)
				{
					m_data->gameData->AddPlayerWin(m_data->deadPlayers[i].id);
				}

				//Load bullet for next game, its not useful
				m_data->bullet = random::RandomInt(1, 6);
			}
			else
			{
				m_data->gameState = WAITING_FOR_INPUT;

				m_data->gunSprAnim.Restart();

				//Next player
				if (m_data->currentPlayer + 1 < m_data->players.size())
				{
					m_data->currentPlayer++;
				}
				else
				{
					m_data->currentPlayer = 0;
				}
				std::cout << "Player : " << m_data->currentPlayer << ", his skin : " << PlayerData::GetTextureName((PlayerData::PlayerSkin)m_data->players[m_data->currentPlayer].skin) << std::endl;

				PlayerData::PlayerSkin skin = m_data->gameData->m_playerDataList[m_data->players[m_data->currentPlayer].id].GetPlayerSkin();
				m_data->iconsChara.SetAnimation(skin);




				//m_data->iconsChara.SetAnimation(Player::GetTextureName((Player::PlayerSkin)m_data->players[m_data->currentPlayer].skin));
				//m_data->iconsChara.SetAnimation(m_data->iconsChara[m_data->players[m_data->currentPlayer].skin]);

			}
		}
		break;

	case END:

		m_data->timerEnd.Update(_deltaTime);

		if (m_data->timerEnd.IsFinished())
		{
			//std::cout << "FINI, CHANGEMENT De SCENE ICI" << std::endl;
			m_data->gameState = TRANSITION;
			m_data->transition.SetTransition(TransitionClass::FADED_OUT);
			m_data->transition.PlayTransition();
		}
		break;
	case TRANSITION:

		m_data->transition.Update(_deltaTime);

		if (m_data->transition.IsFinished())
		{
			//Check if it's not the first transition
			if (m_data->deadPlayers.size() != 0)
			{
				SceneBase::ChangeScene("Board", false);
			}
			else
			{
				m_data->gameState = WAITING_FOR_INPUT;
			}
		}
		break;
	}
}
void RussianRoulette::Draw(sf::RenderWindow& _renderWindow)
{

	sfMod::RenderWindow& renderWindow = *m_data->gameData->m_renderWindow;

	renderWindow.draw(m_data->background);
	renderWindow.draw(m_data->gunSprAnim);
	renderWindow.draw(m_data->text);
	renderWindow.draw(m_data->iconsChara);
	m_data->transition.Draw(renderWindow);
}