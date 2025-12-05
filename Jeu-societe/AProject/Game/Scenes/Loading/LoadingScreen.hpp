#pragma once
#include "../../../Common.hpp"
#include <thread>
#include <atomic>
#include <functional>

struct LoadingData
{
    GameData* gameData = nullptr;
    sf::RectangleShape background;
    sf::Text loadingText;
    sf::RectangleShape progressBarBackground;
    sf::RectangleShape progressBarFill;
    
    std::atomic<float> loadingProgress{0.0f};
    std::atomic<bool> loadingComplete{false};
    std::thread loadingThread;
    
    std::string nextSceneName;
    std::function<void()> loadFunction;
};

class LoadingScreen : public SceneBase
{
private:
    LoadingData* m_data = nullptr;
    GameData* m_gameData = nullptr;

public:
    LoadingScreen() = default;  // NOUVEAU : Constructeur par défaut
    
    void Load(void) override;
    void Unload(void) override;
    void PollEvent(sf::Event& _event) override;
    void Update(float _deltaTime) override;
    void Draw(sf::RenderWindow& _renderWindow) override;

    // Fonction pour configurer le chargement
    void SetupLoading(const std::string& _nextScene, std::function<void(std::atomic<float>&)> _loadFunc);

private:
    void LoadResourcesAsync();
};