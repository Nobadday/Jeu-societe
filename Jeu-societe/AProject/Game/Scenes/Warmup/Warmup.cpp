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

	//Shader
	if (!m_data->chromaKeyShader.loadFromMemory(
		R"(
        uniform sampler2D texture;
        uniform vec3 keyColor; // Couleur à rendre transparente (ex: vert)
        uniform float threshold; // Seuil de tolérance

        void main()
        {
            vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);
            float dist = distance(pixel.rgb, keyColor);
            
            // Si la couleur est proche de keyColor, rendre transparent
            if (dist < threshold)
            {
                pixel.a = 0.0;
            }
            
            gl_FragColor = pixel * gl_Color;
        }
        )", sf::Shader::Fragment))
	{
		std::cout << "Erreur : Impossible de charger le shader chroma key" << std::endl;
	}
	else
	{
		// Définir la couleur à rendre transparente (vert dans cet exemple)
		m_data->chromaKeyShader.setUniform("keyColor", sf::Glsl::Vec3(0.0f, 1.0f, 0.0f)); // RGB vert
		m_data->chromaKeyShader.setUniform("threshold", 0.7f); // Ajuster selon vos besoins
	}

	m_data->background.setTexture(*m_data->gameData->m_assetManager->GetAsset<sf::Texture>("MinigameBackground", AssetManager::AssetType::TEXTURE));

	//Icons
	m_data->iconsChara.setTexture(*m_data->gameData->m_assetManager->GetAsset<TextureAnimated>("Icone", AssetManager::AssetType::TEXTURE_ANIMATED));
	m_data->iconsChara.SetAnimation("Perso1-1");
	m_data->iconsChara.setOrigin({ 0.5f,0.f });

	m_data->text.setFont(*m_data->gameData->m_assetManager->GetAsset<sf::Font>("BadFont"));
	m_data->text.setCharacterSize(50u);
	m_data->text.setPosition({ SCREEN_WIDTH / 2 , SCREEN_HEIGHT * 0.8 });
	m_data->text.setOrigin({ 0.6f,0.8f });
	m_data->text.setString("Press button to be ready");

	m_data->videoPlayer.loadFromFile("Assets/Video/TRANSITION_1.mp4");
	m_data->videoPlayer.play();
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

	if (m_data->state == VIDEO && m_data->playersReady)
	{
		m_data->state = TRANS_2;

		std::string videoPath = "Assets/Video/TRANSITION_2.mp4";
		m_data->videoPlayer.loadFromFile(videoPath);
		m_data->scaleVid = { 1.f,1.f };
		m_data->videoPlayer.play();
	}

	if (m_data->videoPlayer.isFinish())
	{
		switch (m_data->state)
		{
			case TRANS_1:
			{
				m_data->state = VIDEO;

				//Debug
				//std::string videoPath = "Assets/Videos/" + m_data->gameData->m_nextScene + ".mp4";
				//std::string videoPath = "Assets/Video/De1.mp4";
				std::string videoPath = "Assets/Video/TRANSITION_1.mp4";
				m_data->videoPlayer.loadFromFile(videoPath);
				m_data->scaleVid = { 0.8f,0.8f };
				m_data->videoPlayer.play();
			}
				break;

			case VIDEO:
			{
				m_data->videoPlayer.play();
			}
			break;

			case TRANS_2:

				ChangeScene(m_data->gameData->m_nextScene);
				break;
		}
	}
}
void Warmup::Draw(sf::RenderWindow& _renderWindow)
{
	_renderWindow.draw(m_data->background);

	if (!m_data->playersReady)
	{
		_renderWindow.draw(m_data->text);
	}
	PrintIcons(_renderWindow);


	sf::Sprite vid = m_data->videoPlayer.getSprite();
	vid.setScale(m_data->scaleVid.x, m_data->scaleVid.y);
	_renderWindow.draw(vid, &m_data->chromaKeyShader);
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

		m_data->iconsChara.SetAnimation(m_data->gameData->m_playerDataList[m_data->gameData->m_gonnaPlayIndex[i]].GetJoystickId());
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