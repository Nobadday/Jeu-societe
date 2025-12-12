#include "LoadingScreen.hpp"
#include "../Board/Board.hpp"

void LoadingScreen::Load(void)
{
    m_data = new LoadingData;
    m_gameData = (GameData*)this->m_keptData;

    // Configuration visuelle
    if (m_gameData->m_assetManager->GetAsset<sf::Font>("MainFont", AssetManager::AssetType::FONT))
    {
        m_data->loadingText.setFont(*m_gameData->m_assetManager->GetAsset<sf::Font>("MainFont", AssetManager::AssetType::FONT));
        m_data->background.setTexture(*m_gameData->m_assetManager->GetAsset<sf::Texture>("MinigameBackground", AssetManager::AssetType::TEXTURE));
    }
    // Ligne 17 : Remplacer le texte de chargement
    m_data->loadingText.setString("Loading...");
    m_data->loadingText.setCharacterSize(40);
    m_data->loadingText.setFillColor(sf::Color::White);
    
    sf::FloatRect textBounds = m_data->loadingText.getLocalBounds();
    m_data->loadingText.setOrigin(textBounds.width / 2.f, textBounds.height / 2.f);
    m_data->loadingText.setPosition(SCREEN_WIDTH / 2.f, SCREEN_HEIGHT / 2.f - 100.f);

    m_data->progressBarBackground.setSize(sf::Vector2f(600.f, 40.f));
    m_data->progressBarBackground.setFillColor(sf::Color(135, 206, 250));
    m_data->progressBarBackground.setPosition(SCREEN_WIDTH / 2.f - 300.f, SCREEN_HEIGHT / 2.f);

    m_data->progressBarFill.setSize(sf::Vector2f(0.f, 40.f));
    m_data->progressBarFill.setFillColor(sf::Color(255, 181, 190));
    m_data->progressBarFill.setPosition(SCREEN_WIDTH / 2.f - 300.f, SCREEN_HEIGHT / 2.f);

    // Configuration du chargement vers Board
    m_data->nextSceneName = "Board";

	m_gameData->m_tile = new Tiled();
    
    // CHANGEMENT : Charger uniquement le manifest sans créer d'instance temporaire
    m_data->loadFunction = [this]() {
        // Chargement du manifest Board dans le thread de chargement
        m_gameData->m_tile->InitTiled("Assets/Map/map.json");
        m_data->loadingProgress.store(0.3f);
        std::cout << "coucou\n";

        m_data->loadingVideo.loadFromFile("Assets/Video/TRANSITION_2.mp4");
		m_data->loadingProgress.store(0.6f);

        m_gameData->m_assetManager->LoadManifest("Manifests/Board.json", "Board");
        m_data->loadingProgress.store(1.f);
    };

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

    LoadResourcesAsync();
}

void LoadingScreen::Unload(void)
{
    // Attendre la fin du thread de chargement
    if (m_data->loadingThread.joinable())
    {
        m_data->loadingThread.join();
    }

    delete m_data;
    m_data = nullptr;
}

void LoadingScreen::PollEvent(sf::Event& _event)
{
    // Pas d'interaction pendant le chargement
}

void LoadingScreen::Update(float _deltaTime)
{
    // AMÉLIORATION : Interpolation fluide de la progression
    float targetProgress = m_data->loadingProgress.load();
    float currentProgress = m_data->progressBarFill.getSize().x / 600.f;
    
    // Vitesse d'interpolation (plus c'est petit, plus c'est lent et fluide)
    const float smoothSpeed = 1.2f ;
    
    // Interpolation linéaire (lerp)
    float newProgress = currentProgress + (targetProgress - currentProgress ) * smoothSpeed *_deltaTime;
    
    // Mise à jour de la barre avec la valeur interpolée
    m_data->progressBarFill.setSize(sf::Vector2f(600.f * newProgress, 40.f));

	std::cout << "Progression : " << newProgress << std::endl;
    // Vérifier si le chargement est terminé ET que la barre a rattrapé
    if (m_data->loadingComplete.load() && newProgress >= 0.999f && !m_data->nextSceneName.empty())
    {
        if (!m_data->videoLoaded)
        {
			m_data->videoLoaded = true;
            m_data->loadingVideo.play();
        }
        // Passer à la scène suivante
        if (m_data->loadingVideo.isFinish())
        {
            ChangeScene(m_data->nextSceneName, false);
        }
        else
        {
			m_data->loadingVideo.update(_deltaTime);
        }
       
    }
}

void LoadingScreen::Draw(sf::RenderWindow& _renderWindow)
{
    _renderWindow.draw(m_data->background);
    _renderWindow.draw(m_data->progressBarBackground);
    _renderWindow.draw(m_data->progressBarFill);
    _renderWindow.draw(m_data->loadingText);

    if (m_data->videoLoaded)
    {
        sf::Sprite videoSprite(m_data->loadingVideo.getSprite());
        // Obtenir la taille du sprite vidéo
        sf::FloatRect videoBounds = videoSprite.getLocalBounds();
        // Centrer l'origine du sprite
        videoSprite.setOrigin(videoBounds.width / 2.0f, videoBounds.height / 2.0f);
        // Positionner au centre de l'écran
        videoSprite.setPosition(SCREEN_WIDTH / 2.f, SCREEN_HEIGHT / 2.f);
		_renderWindow.draw(videoSprite,&m_data->chromaKeyShader);

    }

}

void LoadingScreen::SetupLoading(const std::string& _nextScene, std::function<void(std::atomic<float>&)> _loadFunc)
{
    m_data->nextSceneName = _nextScene;
    m_data->loadFunction = [_loadFunc, this]() {
        _loadFunc(m_data->loadingProgress);
    };
}

void LoadingScreen::LoadResourcesAsync()
{
    m_data->loadingThread = std::thread([this]() {
        if (m_data->loadFunction)
        {
            m_data->loadFunction();
        }
        m_data->loadingComplete.store(true);
    });
}