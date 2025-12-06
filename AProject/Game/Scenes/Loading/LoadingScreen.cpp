#include "LoadingScreen.hpp"
#include "../Board/Board.hpp"

void LoadingScreen::Load(void)
{
    m_data = new LoadingData;
    m_gameData = (GameData*)this->m_keptData;

    // Configuration visuelle
    m_data->background.setSize(sf::Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT));
    m_data->background.setFillColor(sf::Color(20, 20, 30));

    if (m_gameData->m_assetManager->GetAsset<sf::Font>("MainFont", AssetManager::AssetType::FONT))
    {
        m_data->loadingText.setFont(*m_gameData->m_assetManager->GetAsset<sf::Font>("MainFont", AssetManager::AssetType::FONT));
    }
    m_data->loadingText.setString("Chargement en cours...");
    m_data->loadingText.setCharacterSize(40);
    m_data->loadingText.setFillColor(sf::Color::White);
    
    sf::FloatRect textBounds = m_data->loadingText.getLocalBounds();
    m_data->loadingText.setOrigin(textBounds.width / 2.f, textBounds.height / 2.f);
    m_data->loadingText.setPosition(SCREEN_WIDTH / 2.f, SCREEN_HEIGHT / 2.f - 100.f);

    m_data->progressBarBackground.setSize(sf::Vector2f(600.f, 40.f));
    m_data->progressBarBackground.setFillColor(sf::Color(50, 50, 60));
    m_data->progressBarBackground.setPosition(SCREEN_WIDTH / 2.f - 300.f, SCREEN_HEIGHT / 2.f);

    m_data->progressBarFill.setSize(sf::Vector2f(0.f, 40.f));
    m_data->progressBarFill.setFillColor(sf::Color(100, 200, 255));
    m_data->progressBarFill.setPosition(SCREEN_WIDTH / 2.f - 300.f, SCREEN_HEIGHT / 2.f);

    // Configuration du chargement vers Board
    m_data->nextSceneName = "ArmWrestling";

	m_gameData->m_tile = new Tiled();
    
    // CHANGEMENT : Charger uniquement le manifest sans créer d'instance temporaire
    m_data->loadFunction = [this]() {
        // Chargement du manifest Board dans le thread de chargement
        m_gameData->m_tile->InitTiled("Assets/Map/map.json");
        m_data->loadingProgress.store(0.5f);
        m_gameData->m_assetManager->LoadManifest("Manifests/Board.json", "Board");
        m_data->loadingProgress.store(1.f);
    };

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
    const float smoothSpeed = 5.0f;
    
    // Interpolation linéaire (lerp)
    float newProgress = currentProgress + (targetProgress - currentProgress) * smoothSpeed * _deltaTime;
    
    // Mise à jour de la barre avec la valeur interpolée
    m_data->progressBarFill.setSize(sf::Vector2f(600.f * newProgress, 40.f));

    // Vérifier si le chargement est terminé ET que la barre a rattrapé
    if (m_data->loadingComplete.load() && newProgress >= 0.99f && !m_data->nextSceneName.empty())
    {
        // Passer à la scène suivante
        ChangeScene(m_data->nextSceneName, false);
    }
}

void LoadingScreen::Draw(sf::RenderWindow& _renderWindow)
{
    _renderWindow.draw(m_data->background);
    _renderWindow.draw(m_data->progressBarBackground);
    _renderWindow.draw(m_data->progressBarFill);
    _renderWindow.draw(m_data->loadingText);
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