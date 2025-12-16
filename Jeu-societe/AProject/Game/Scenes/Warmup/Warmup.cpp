#include "Warmup.hpp"


void Warmup::Load(void)
{
	m_data = new SceneData;
	m_data->gameData = (GameData*)this->m_keptData;
	m_data->gameData->m_assetManager->LoadManifest("Manifests/Warmup.json", "Warmup");
	m_data->audio = (AudioEngine*)m_data->gameData->m_audioEngine;

	m_data->playersReadyVec.resize(m_data->gameData->m_gonnaPlayIndex.size());
	for (size_t i = 0; i < m_data->playersReadyVec.size(); i++)
	{
		m_data->playersReadyVec[i] = false;
	}

	m_data->text.setFont(*m_data->gameData->m_assetManager->GetAsset<sf::Font>("BadFont"));
	m_data->buttonText.setFont(*m_data->gameData->m_assetManager->GetAsset<sf::Font>("BadFont"));
	m_data->text.setCharacterSize(50u);
	m_data->buttonText.setCharacterSize(50u);
	m_data->text.setPosition({ SCREEN_WIDTH / 2.f , SCREEN_HEIGHT * 0.8f });
	m_data->buttonText.SetCharactersPerLine(17);
	m_data->text.setOrigin({ 0.6f,0.8f });
	//m_data->buttonText.setOrigin({ 0.6f,0.8f });
	m_data->text.setString("Press button to be ready");


	#pragma region Icons
	m_data->background.setTexture(*m_data->gameData->m_assetManager->GetAsset<sf::Texture>("Background", AssetManager::AssetType::TEXTURE));

	//Icons
	m_data->iconsChara.setTexture(*m_data->gameData->m_assetManager->GetAsset<TextureAnimated>("Icone", AssetManager::AssetType::TEXTURE_ANIMATED));
	m_data->iconsChara.SetAnimation("Perso1-1");
	m_data->iconsChara.setOrigin({ 0.5f,0.f });

	//Icons chara
	m_data->charaAvaible.push_back("Perso1-1");
	m_data->charaAvaible.push_back("Perso2-1");
	m_data->charaAvaible.push_back("Perso3-1");
	m_data->charaAvaible.push_back("Perso4-1");

	m_data->charaAvaible.push_back("Perso1-2");
	m_data->charaAvaible.push_back("Perso2-2");
	m_data->charaAvaible.push_back("Perso3-2");
	m_data->charaAvaible.push_back("Perso4-2");
	#pragma endregion
	m_data->scaleVid = { 0.75f,0.75f };

	m_data->buttonSpr.setTexture((*m_data->gameData->m_assetManager->GetAsset<TextureAtlas>("Input", AssetManager::AssetType::TEXTURE_ATLAS)));
	if (m_data->gameData->m_nextScene == "rockPaperSizor")
	{
		m_data->videoPlayer.loadFromFile("Assets/Video/RPS.mov");


		m_data->buttonName.push_back("X");
		m_data->buttonName.push_back("A");
		m_data->buttonName.push_back("B");


		m_data->stringOfButton.push_back("Rock Paper Scissor");
		m_data->stringOfButton.push_back("Rock");
		m_data->stringOfButton.push_back("Paper");
		m_data->stringOfButton.push_back("Scissors");
	}
	else if (m_data->gameData->m_nextScene == "ArmWrestling")
	{
		m_data->videoPlayer.loadFromFile("Assets/Video/ArmW.mov");

		m_data->buttonName.push_back("A");

		m_data->stringOfButton.push_back("Arm Wrestling");
		m_data->stringOfButton.push_back("Press to force");
	}
	else if (m_data->gameData->m_nextScene == "FlagGame")
	{
		m_data->videoPlayer.loadFromFile("Assets/Video/FlagGame.mp4");
	
		m_data->stringOfButton.push_back("Flag Game\n\nPress the button displayed as quickly as possible");
	}
	else if (m_data->gameData->m_nextScene == "RandCard")
	{
		m_data->videoPlayer.loadFromFile("Assets/Video/RandCard.mov");
		
		m_data->stringOfButton.push_back("Random Card\n\nSelect one card, but dont pick the bomb !");
	}
	else if (m_data->gameData->m_nextScene == "RuRoul")
	{
		m_data->videoPlayer.loadFromFile("Assets/Video/RuRoul.mov");

		m_data->stringOfButton.push_back("Russian Roulette\n\nPress button to shoot. Be lucky to survive !");
	}
	else
	{
		std::cout << "Va bien te faire enculer, appelle benoit pour faire la modif" << std::endl;
		std::cout << "Tu veut load le warmup d'un jeu ou la video est pas implemente" << std::endl;
	}

	m_data->transition.PlayTransition();
}

void Warmup::Unload(void)
{

}
void Warmup::PollEvent(sf::Event& _event)
{
	switch (_event.type)
	{
		case sf::Event::KeyPressed:

			if (_event.key.code == sf::Keyboard::Space)
			{
				m_data->playersReady = true;
			}
			break;

		case sf::Event::JoystickButtonPressed:

			int nbOfReadyPlayers = 0;
			for (size_t i = 0; i < m_data->gameData->m_gonnaPlayIndex.size(); i++)
			{
				if (_event.joystickButton.joystickId == m_data->gameData->m_playerDataList[m_data->gameData->m_gonnaPlayIndex[i]].GetJoystickId())
				{
					m_data->playersReadyVec[i] = true;
				}
				if (m_data->playersReadyVec[i])
				{
					nbOfReadyPlayers++;
				}
			}
			if (nbOfReadyPlayers == m_data->playersReadyVec.size())
			{
				m_data->playersReady = true;
			}
			break;
	}
}
void Warmup::Update(float _deltaTime)
{
	m_data->videoPlayer.update(_deltaTime);

	switch (m_data->state)
	{
		case TRANS_1:
		
			m_data->transition.Update(_deltaTime);

			if (m_data->transition.IsFinished())
			{
				m_data->state = VIDEO;
				m_data->videoPlayer.play();
			}
			break;
		case VIDEO:

			m_data->transition.Update(_deltaTime);

			if (m_data->playersReady)
			{
				m_data->state = TRANS_2;

				m_data->transition.SetTransition(TransitionClass::FADED_OUT);
				m_data->transition.PlayTransition();
			}
			else if (m_data->transition.IsFinished())
			{
				m_data->videoPlayer.play();
			}
			break;

		case TRANS_2:
			
			m_data->transition.Update(_deltaTime);

			if (m_data->transition.IsFinished())
			{
				ChangeScene(m_data->gameData->m_nextScene);
			}			
			break;
	}
}
void Warmup::Draw(sf::RenderWindow& _renderWindow)
{
	_renderWindow.draw(m_data->background);
	PrintIcons(_renderWindow);



	switch (m_data->state)
	{
		case TRANS_1:

			m_data->transition.Draw(_renderWindow);
			break;

		case VIDEO:
		{
			_renderWindow.draw(m_data->text);
			PrintButtons(_renderWindow);

			//Video
			sf::Sprite vid = m_data->videoPlayer.getSprite();
			vid.setScale(m_data->scaleVid.x, m_data->scaleVid.y);
			_renderWindow.draw(vid);
		}
		break;

		case TRANS_2:

			m_data->transition.Draw(_renderWindow);
			break;
	}
}

void Warmup::PrintIcons(sf::RenderWindow& _renderWindow)
{
	//For placement 
	float border = 200.f;
	float iconSpacing = (float)(SCREEN_WIDTH - 2.f * border) / ((float)m_data->playersReadyVec.size() - 1);
	sf::FloatRect iconBounds = m_data->iconsChara.getLocalBounds();

	for (int i = 0; i < (int)m_data->playersReadyVec.size(); i++)
	{
		sf::Vector2f pos = { (float)(border + i * iconSpacing), SCREEN_HEIGHT - iconBounds.height};

		PlayerData::PlayerSkin skin = m_data->gameData->m_playerDataList[i].GetPlayerSkin();
		m_data->iconsChara.SetAnimation(skin);

		m_data->iconsChara.setPosition(pos);
		if (m_data->playersReadyVec[i])
		{
			m_data->iconsChara.setColor({ 155,155,155,255 });
		}
		_renderWindow.draw(m_data->iconsChara);
		m_data->iconsChara.setScale({ 1.0f,1.0f });
		m_data->iconsChara.setColor({ 255,255,255,150 });
	}
}

void Warmup::PrintButtons(sf::RenderWindow& _renderWindow)
{
	//Title of the minigame
	m_data->buttonText.setPosition({ SCREEN_WIDTH * 0.76f, SCREEN_HEIGHT * 0.1f });
	m_data->buttonText.setString(m_data->stringOfButton[0]);
	_renderWindow.draw(m_data->buttonText);

	std::cout << "size of string of button : " << m_data->stringOfButton.size() << std::endl;
	if (m_data->stringOfButton.size() > 1)
	{
		for (size_t i = 1; i < m_data->stringOfButton.size(); i++)
		{
			std::cout << "Drawing button " << i << " name : " << m_data->buttonName[i - 1] << " with string : " << m_data->stringOfButton[i] << std::endl;


			//m_data->buttonText.setString(m_data->buttonName[i - 1] + "\n\n" + m_data->stringOfButton[i]);
			m_data->buttonSpr.SetTextureFrame(m_data->buttonName[i - 1]);
			m_data->buttonSpr.setPosition({ SCREEN_WIDTH * 0.76f, SCREEN_HEIGHT / 3.f + i * 100.f });
			_renderWindow.draw(m_data->buttonSpr);

			//m_data->buttonSpr.setPosition({ SCREEN_WIDTH * 0.8f, SCREEN_HEIGHT / 3.f + i * 100.f });
			m_data->buttonText.setString(m_data->stringOfButton[i]);
			m_data->buttonText.setPosition({ SCREEN_WIDTH * 0.85f, SCREEN_HEIGHT / 3.f + i * 100.f });
			_renderWindow.draw(m_data->buttonText);
		}
	}




}
