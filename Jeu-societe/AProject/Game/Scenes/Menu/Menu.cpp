#include "Menu.hpp"

//3 = 4 players
#define MAX_PLAYERS 3
//1 = 2 players
#define MIN_PLAYERS 0
//Delay to scroll in buttons with controler
#define INPUT_DELAY 0.5f



void Menu::Load(void)
{
	m_data = new SceneData;
	m_data->gameData = (GameData*)this->m_keptData;
	m_data->gameData->m_assetManager->LoadManifest("Manifests/Menu.json", "Menu");
	m_data->audio = (AudioEngine*)m_data->gameData->m_audioEngine;
	LoadUI();
	m_data->audio->SetMusicVolume(25.f);

	m_data->introVideo.loadFromFile("Assets/Video/Intro.mov");
	m_data->introVideo.play();
	m_data->introVideo.update(1.f);
}

void Menu::LoadUI(void)
{
	LoadButtons();
	LoadSprites();
	LoadCharacterIcons();
	LoadText();
	PositionMainMenuButtons();
	
	// Initialisation des éléments de sélection de bots
	m_data->ui.botCount = 0;
	m_data->ui.botCountText.setFont(*m_data->gameData->m_assetManager->GetAsset<sf::Font>("MenuFont"));
	m_data->ui.botCountText.setOutlineColor(sf::Color::Black);
	m_data->ui.botCountText.setOutlineThickness(1.5f);
	m_data->ui.botCountText.setCharacterSize(150u);
	m_data->ui.botCountText.setPosition({SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2});
	m_data->ui.botCountText.setOrigin({0.5f, 0.5f});
	
	// NOUVEAU : Initialisation de la difficulté des bots (liste)
	m_data->ui.botDifficulties.clear();
	m_data->ui.currentBotIndex = 0;
	
	m_data->ui.botDifficultyText.setFont(*m_data->gameData->m_assetManager->GetAsset<sf::Font>("MenuFont"));
	m_data->ui.botDifficultyText.setOutlineColor(sf::Color::Black);
	m_data->ui.botDifficultyText.setOutlineThickness(1.5f);
	m_data->ui.botDifficultyText.setCharacterSize(90u);
	m_data->ui.botDifficultyText.setPosition({SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2});
	m_data->ui.botDifficultyText.setOrigin({0.5f, 0.5f});
}

void Menu::LoadButtons()
{
	const std::vector<std::string> buttonNames = {
		"playBtn", "settingsBtn", "leaveBtn", "creditsBtn", "moinsBtn", "plusBtn"
	};
	
	for (const auto& name : buttonNames)
	{
		m_data->ui.buttonMap[name].setTexture(
			*m_data->gameData->m_assetManager->GetAsset<TextureAnimated>(name)
		);
		m_data->ui.buttonMap[name].setOrigin({0.5f, 0.5f});
	}
	
	m_data->ui.buttonMap["creditsBtn"].setScale({0.8f, 0.8f});
}

void Menu::LoadSprites()
{
	// Background
	m_data->ui.background.setTexture(
		*m_data->gameData->m_assetManager->GetAsset<sf::Texture>("MinigameBackground")
	);
	
	// Game logo
	m_data->ui.logoGame.setTexture(
		*m_data->gameData->m_assetManager->GetAsset<sf::Texture>("LogoGame")
	);
	sf::Vector2u logoSize = m_data->gameData->m_assetManager->GetAsset<sf::Texture>("LogoGame")->getSize();
	m_data->ui.logoGame.setOrigin(sf::Vector2f(logoSize.x / 2.f, 1.f));
	m_data->ui.logoGame.setPosition({SCREEN_WIDTH / 2, 0});
	m_data->ui.logoGame.setScale({0.8f, 0.8f});
	
	// Crea logo
	m_data->ui.logoCrea.setTexture(
		*m_data->gameData->m_assetManager->GetAsset<sf::Texture>("LogoCrea")
	);
	logoSize = m_data->gameData->m_assetManager->GetAsset<sf::Texture>("LogoCrea")->getSize();
	m_data->ui.logoCrea.setOrigin(sf::Vector2f(1.f, logoSize.y / 2.f));
	m_data->ui.logoCrea.setPosition({10, SCREEN_HEIGHT / 1.3});
	m_data->ui.logoCrea.setScale({0.3f, 0.3f});
}

void Menu::LoadCharacterIcons()
{
	m_data->ui.charaAvaible = {"Perso1-1", "Perso2-1", "Perso3-1", "Perso4-1"};
	
	m_data->ui.iconsChara.setTexture(
		*m_data->gameData->m_assetManager->GetAsset<TextureAnimated>("Icone", AssetManager::AssetType::TEXTURE_ANIMATED)
	);
	m_data->ui.iconsChara.SetAnimation("Perso1-1");
	m_data->ui.iconsChara.setOrigin({0.5f, 0.5f});
}

void Menu::LoadText()
{
	m_data->ui.playerCount.setFont(
		*m_data->gameData->m_assetManager->GetAsset<sf::Font>("MenuFont")
	);
	m_data->ui.playerCount.setOutlineColor(sf::Color::Black);
	m_data->ui.playerCount.setOutlineThickness(1.5f);
	m_data->ui.playerCount.setCharacterSize(200u);
	m_data->ui.playerCount.setPosition({SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2});
	m_data->ui.playerCount.setOrigin({0.6f, 0.8f});
	m_data->ui.playerCount.setString(std::to_string(m_data->gameSettings.playerCount + 1));
	
    
    m_data->ui.playerText.setFont(
		*m_data->gameData->m_assetManager->GetAsset<sf::Font>("MenuFont")
	);
	m_data->ui.playerText.setOutlineColor(sf::Color::Black);
	m_data->ui.playerText.setOutlineThickness(1.5f);
	m_data->ui.playerText.setCharacterSize(100u);
	m_data->ui.playerText.setPosition({SCREEN_WIDTH / 2 + 150, SCREEN_HEIGHT / 4});
	m_data->ui.playerText.setOrigin({0.6f, 0.8f});
	m_data->ui.playerText.setString("Select Number Of Player");
}

void Menu::PositionMainMenuButtons()
{
	sf::FloatRect buttonRect = m_data->ui.buttonMap["playBtn"].getGlobalBounds();
	m_data->ui.buttonMap["playBtn"].setPosition({SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2});
	m_data->ui.buttonMap["settingsBtn"].setPosition({SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + buttonRect.height});
	m_data->ui.buttonMap["leaveBtn"].setPosition({SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 2 * buttonRect.height});
	m_data->ui.buttonMap["creditsBtn"].setPosition({SCREEN_WIDTH - buttonRect.width / 2, SCREEN_HEIGHT / 2 + 2 * buttonRect.height});
}

void Menu::Unload(void)
{

}

void Menu::PollEvent(sf::Event& _event)
{
	if (m_data->state != INTRO)
	{
		//Update poll event button
		ButtonsPollEvent(_event);

		//Poll event of controlers
		switch (_event.type)
		{
			//If we press Enter, it's like press button on controler
			//Unusefull if we play only on controler
			case sf::Event::KeyPressed:
				if (_event.key.code == sf::Keyboard::Escape)
				{
					ReturnPressed();
				}
				else if (_event.key.code != sf::Keyboard::Enter)
				{
					break;
				}
			case sf::Event::JoystickButtonPressed:

				if (_event.joystickButton.button == 1)
				{
					ReturnPressed();
					return;
				}

				m_data->audio->PlaySound("uiSoundClick");
				PressSelection(_event.joystickButton.joystickId);
				//_event.joystick
				break;

			case sf::Event::JoystickMoved:

				//X Y joystick gauche
				//U V joystick droite
				//Z R pression des gachettes
				//La croix povX povY		

				if (m_data->inputDelay > INPUT_DELAY)
				{
					switch (_event.joystickMove.axis)
					{
						//All directions
						case sf::Joystick::Axis::X:
						case sf::Joystick::Axis::Y:
						case sf::Joystick::Axis::U:
						case sf::Joystick::Axis::V:

							//std::cout << "jostick : " << _event.joystickMove.position << "btn : " << m_data->controlerBtn << std::endl;

							if (_event.joystickMove.position > 20)
							{
								ChangeSelection(1, _event.joystickMove.joystickId);
								m_data->inputDelay = 0.f;
							}
							else if (_event.joystickMove.position < -20)
							{
								ChangeSelection(-1, _event.joystickMove.joystickId);
								m_data->inputDelay = 0.f;
							}
							break;
					}
				}
				break;
		}
	}
}
void Menu::ButtonsPollEvent(sf::Event& _event)
{
    // Liste des boutons pour chaque état
    static const std::vector<std::string> mainMenuButtons = {"playBtn", "settingsBtn", "leaveBtn", "creditsBtn"};
    static const std::vector<std::string> optionsButtons = {"playBtn", "plusBtn", "moinsBtn"};
    static const std::vector<std::string> botSelectionButtons = {"playBtn", "plusBtn", "moinsBtn"}; // NOUVEAU

    const std::vector<std::string>* buttons = nullptr;
    
    switch (m_data->state)
    {
        case MAIN_MENU:
            buttons = &mainMenuButtons;
            break;
        case OPTIONS:
        case PLAYER_NB_SELECTION:
        case BOT_SELECTION:
        case BOT_DIFFICULTY_SELECTION: // NOUVEAU
            buttons = &optionsButtons;
            break;
    }

    if (buttons)
    {
        for (const auto& btnName : *buttons)
        {
            m_data->ui.buttonMap[btnName].CheckEvent(_event);
        }
    }
}

void Menu::Update(float _deltaTime)
{
	if (m_data->state == INTRO)
	{
		m_data->introVideo.update(_deltaTime);

		if (m_data->introVideo.isFinish())
		{
			m_data->state = MAIN_MENU;
			m_data->audio->PlayMusic("Music1", true);
		}
		return;
	}
	else
	{
		ButtonsUpdate(_deltaTime);

		//std::cout << "current chara = " << m_data->currentCharaSelected << std::endl;
		//std::cout << "current state = " << m_data->state << std::endl;
		//Update timer for delay between input
		m_data->inputDelay += _deltaTime;
	}
}
void Menu::ButtonsUpdate(float _dt)
{
    static const std::vector<std::string> mainMenuButtons = {"playBtn", "settingsBtn", "leaveBtn", "creditsBtn"};
    static const std::vector<std::string> optionsButtons = {"playBtn", "plusBtn", "moinsBtn"};

    const std::vector<std::string>* buttons = nullptr;
    
    switch (m_data->state)
    {
        case MAIN_MENU:
            buttons = &mainMenuButtons;
            break;
        case OPTIONS:
        case PLAYER_NB_SELECTION:
        case BOT_SELECTION:
        case BOT_DIFFICULTY_SELECTION: // NOUVEAU
            buttons = &optionsButtons;
            break;
    }

    if (buttons)
    {
        for (const auto& btnName : *buttons)
        {
            m_data->ui.buttonMap[btnName].Update(_dt);
        }
    }
}

void Menu::Draw(sf::RenderWindow& _renderWindow)
{
	sfMod::RenderWindow* bWindow = m_data->gameData->m_renderWindow;

	if (m_data->state == INTRO)
	{
		sf::Sprite vid = m_data->introVideo.getSprite();
		bWindow->draw(vid);
	}
	else
	{
		bWindow->draw(m_data->ui.background);
		DrawUI(bWindow);
	}
}
void Menu::DrawUI(sfMod::RenderWindow* _renderWindow)
{
	sf::Vector2i mousePos = sf::Mouse::getPosition(*_renderWindow);
	switch (m_data->state)
	{
	case MAIN_MENU:
		_renderWindow->draw(m_data->ui.buttonMap["playBtn"]);
		_renderWindow->draw(m_data->ui.buttonMap["settingsBtn"]);
		_renderWindow->draw(m_data->ui.buttonMap["leaveBtn"]);
		_renderWindow->draw(m_data->ui.buttonMap["creditsBtn"]);
		_renderWindow->draw(m_data->ui.logoGame);

		if (m_data->ui.buttonMap["leaveBtn"].HasBeenClicked())
		{
			_renderWindow->close();
		}
		break;

	case OPTIONS:
		PrintOptions(_renderWindow);
		break;

	case PLAYER_NB_SELECTION:
		_renderWindow->draw(m_data->ui.buttonMap["playBtn"]);
		_renderWindow->draw(m_data->ui.buttonMap["moinsBtn"]);
		_renderWindow->draw(m_data->ui.playerCount);
		_renderWindow->draw(m_data->ui.playerText);
		_renderWindow->draw(m_data->ui.buttonMap["plusBtn"]);
		break;

	case BOT_SELECTION:
		PrintBotSelection(_renderWindow);
		break;

	// NOUVEAU : Écran de sélection de difficulté des bots
	case BOT_DIFFICULTY_SELECTION:
		PrintBotDifficultySelection(_renderWindow);
		break;

	case PLAYER_SELECTION:
		PrintIcons(_renderWindow);
		break;
	}
}

void Menu::PrintBotSelection(sfMod::RenderWindow* _renderWindow)
{
	// Titre
	m_data->ui.botCountText.setCharacterSize(100u);
	m_data->ui.botCountText.setOrigin({ 0.6f, 0.8f });
	m_data->ui.botCountText.setPosition(SCREEN_WIDTH / 2.f + 100, SCREEN_HEIGHT / 4.f);
	m_data->ui.botCountText.setString("How many bots?");
	_renderWindow->draw(m_data->ui.botCountText);

	// Nombre de bots
    m_data->ui.botCountText.setOutlineThickness(1.5f);
    m_data->ui.botCountText.setCharacterSize(200u);
    m_data->ui.botCountText.setPosition({ SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 });
    m_data->ui.botCountText.setOrigin({ 0.6f, 0.8f });
	m_data->ui.botCountText.setString(std::to_string(m_data->ui.botCount));
	_renderWindow->draw(m_data->ui.botCountText);

	// Boutons +/-
	_renderWindow->draw(m_data->ui.buttonMap["playBtn"]);
	_renderWindow->draw(m_data->ui.buttonMap["moinsBtn"]);
	_renderWindow->draw(m_data->ui.buttonMap["plusBtn"]);
}

// NOUVELLE FONCTION : Afficher l'écran de sélection de difficulté des bots
void Menu::PrintBotDifficultySelection(sfMod::RenderWindow* _renderWindow)
{
	// Titre avec numéro du bot
	m_data->ui.botDifficultyText.setCharacterSize(80u);
	m_data->ui.botDifficultyText.setOrigin({ 0.6f, 0.8f });
	m_data->ui.botDifficultyText.setPosition(SCREEN_WIDTH / 2.f + 100  , SCREEN_HEIGHT / 5.f);
	
	char titleBuffer[100];
	std::snprintf(titleBuffer, 100, "Bot %d on %d - Select Difficulty", 
	              m_data->ui.currentBotIndex + 1, m_data->ui.botCount);
	m_data->ui.botDifficultyText.setString(titleBuffer);
	m_data->ui.botDifficultyText.setFillColor(sf::Color::White);
	_renderWindow->draw(m_data->ui.botDifficultyText);

	// Difficulté actuelle avec couleur
	m_data->ui.botDifficultyText.setCharacterSize(100u);
	m_data->ui.botDifficultyText.setPosition({ SCREEN_WIDTH / 2 + 20, SCREEN_HEIGHT / 2 });
	m_data->ui.botDifficultyText.setOrigin({ 0.6f, 0.8f });
	
	int currentDifficulty = m_data->ui.botDifficulties[m_data->ui.currentBotIndex];
	std::string difficultyName = GetDifficultyName(currentDifficulty);
	m_data->ui.botDifficultyText.setString(difficultyName);
	
	// Couleur selon la difficulté
	if (currentDifficulty == 0) // Easy
		m_data->ui.botDifficultyText.setFillColor(sf::Color::Green);
	else if (currentDifficulty == 1) // Medium
		m_data->ui.botDifficultyText.setFillColor(sf::Color::Yellow);
	else // Hard
		m_data->ui.botDifficultyText.setFillColor(sf::Color::Red);
	
	_renderWindow->draw(m_data->ui.botDifficultyText);
	
	// Réinitialiser la couleur pour les prochains affichages
	m_data->ui.botDifficultyText.setFillColor(sf::Color::White);

	// Indicateur de progression
	m_data->ui.botDifficultyText.setCharacterSize(50u);
	m_data->ui.botDifficultyText.setPosition({ SCREEN_WIDTH / 2, SCREEN_HEIGHT - 150.f });
	m_data->ui.botDifficultyText.setOrigin({ 0.5f, 0.5f });
	
	char progressBuffer[100];
	std::snprintf(progressBuffer, 100, "Configured: %d on %d bots", 
	              m_data->ui.currentBotIndex, m_data->ui.botCount);
	m_data->ui.botDifficultyText.setString(progressBuffer);
	_renderWindow->draw(m_data->ui.botDifficultyText);

	// Boutons +/-
	_renderWindow->draw(m_data->ui.buttonMap["playBtn"]);
	_renderWindow->draw(m_data->ui.buttonMap["moinsBtn"]);
	_renderWindow->draw(m_data->ui.buttonMap["plusBtn"]);
}

void Menu::PressSelection(int _id)
{
    m_data->audio->PlaySound("uiSoundClick");
    switch (m_data->controlerBtn)
    {
    case PLAY:
        switch (m_data->state)
        {
        case MAIN_MENU:
            m_data->state = PLAYER_NB_SELECTION;
            m_data->controlerBtn = PLAY_SELECTION;
            m_data->ui.buttonMap["plusBtn"].setScale(1, 1);
            m_data->ui.buttonMap["moinsBtn"].setScale(1, 1);
            m_data->ui.buttonMap["playBtn"].setPosition({SCREEN_WIDTH / 2, SCREEN_HEIGHT / 1.5});
            m_data->ui.buttonMap["moinsBtn"].setPosition({SCREEN_WIDTH / 2 - 0.2f * SCREEN_WIDTH, SCREEN_HEIGHT / 2});
            m_data->ui.buttonMap["plusBtn"].setPosition({SCREEN_WIDTH / 2 + 0.2f * SCREEN_WIDTH, SCREEN_HEIGHT / 2});
            m_data->ui.playerCount.setString(std::to_string(m_data->gameSettings.playerCount + 1));
            m_data->ui.playerCount.setPosition({SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2});
            break;

        case PLAYER_NB_SELECTION:
            break;

        case PLAYER_SELECTION:
        {
            std::cout << "id = " << _id << " size of datalist = " << m_data->gameData->m_playerDataList.size() << std::endl;

            int totalHumans = m_data->gameSettings.playerCount + 1;
            
            if (_id >= totalHumans)
            {
                std::cout << "IGNORE : Ceci est un bot (id=" << _id << ", totalHumans=" << totalHumans << ")" << std::endl;
                return;
            }

            PlayerData::PlayerSkin selectedSkin = GetPlayerSkinFromIndex(m_data->currentCharaSelected[_id]);
            m_data->gameData->m_playerDataList[_id].SetPlayerSkin(selectedSkin);
            m_data->charaSelected[_id] = true;

            int humanPlayersSelected = 0;
            for (int i = 0; i < totalHumans; i++)
            {
                if (m_data->charaSelected[i])
                {
                    humanPlayersSelected++;
                }
            }

            std::cout << "DEBUG: " << humanPlayersSelected << "/" << totalHumans << " joueurs humains ont validé" << std::endl;

            if (humanPlayersSelected == totalHumans)
            {
                std::cout << "Tous les humains ont validé, attribution des skins aux bots..." << std::endl;
                
                // AMÉLIORATION : Collecter les skins pris par les humains
                std::vector<int> takenSkins;
                for (int i = 0; i < totalHumans; i++)
                {
                    takenSkins.push_back(m_data->currentCharaSelected[i]);
                }
                
                // AMÉLIORATION : Créer une liste des skins disponibles
                std::vector<int> availableSkins;
                for (int skin = 0; skin < 4; skin++)
                {
                    if (std::find(takenSkins.begin(), takenSkins.end(), skin) == takenSkins.end())
                    {
                        availableSkins.push_back(skin);
                    }
                }
                
                std::cout << "Skins disponibles pour les bots : ";
                for (int skin : availableSkins)
                {
                    std::cout << skin << " ";
                }
                std::cout << std::endl;
                
                // AMÉLIORATION : Attribuer les skins disponibles aléatoirement aux bots
                for (int i = totalHumans; i < m_data->gameData->m_playerDataList.size(); i++)
                {
                    int botSkin;
                    
                    if (!availableSkins.empty())
                    {
                        // Choisir un skin aléatoire parmi les disponibles
                        int randomIndex = randmt::RandomInt(0, (int)availableSkins.size() - 1);
                        botSkin = availableSkins[randomIndex];
                        
                        // Retirer ce skin de la liste des disponibles
                        availableSkins.erase(availableSkins.begin() + randomIndex);
                    }
                    else
                    {
                        // Sécurité : si tous les skins sont pris (ne devrait jamais arriver avec max 4 joueurs)
                        botSkin = randmt::RandomInt(0, 3);
                        std::cout << "AVERTISSEMENT : Tous les skins sont pris, attribution forcée du skin " << botSkin << std::endl;
                    }
                    
                    takenSkins.push_back(botSkin);
                    
                    PlayerData::PlayerSkin skin = GetPlayerSkinFromIndex(botSkin);
                    m_data->gameData->m_playerDataList[i].SetPlayerSkin(skin);
                    
                    std::cout << "Bot " << i << " (bot #" << (i - totalHumans + 1) << ") a reçu le skin " << botSkin << std::endl;
                }
                
                std::cout << "Démarrage du jeu..." << std::endl;
                SceneBase::ChangeScene("Lo");
            }
            break;
        }
        }
        break;

    case SETTINGS:
        m_data->state = OPTIONS;
        m_data->controlerBtn = MORE;
        m_data->ui.buttonMap["plusBtn"].setScale(0.8f, 0.8f);
        m_data->ui.buttonMap["moinsBtn"].setScale(0.8f, 0.8f);
        m_data->ui.buttonMap["plusBtn"].setPosition(SCREEN_WIDTH / 1.2f, 2.1f / 4.f * SCREEN_HEIGHT);
        m_data->ui.buttonMap["moinsBtn"].setPosition(SCREEN_WIDTH / 1.5f, 2.1f / 4.f * SCREEN_HEIGHT);
        m_data->ui.buttonMap["playBtn"].setPosition(SCREEN_WIDTH / 2.f, 3.f / 4.f * SCREEN_HEIGHT);
        m_data->controlerBtn = PLAY_SELECTION;
        break;

    case CREDITS_BTN:
        SceneBase::ChangeScene("Credits");
        break;

    case LEAVE:
        m_data->gameData->m_renderWindow->close();
        break;

    case LESS:
        switch (m_data->state)
        {
        case PLAYER_NB_SELECTION:
            if (m_data->gameSettings.playerCount > MIN_PLAYERS)
            {
                m_data->gameSettings.playerCount -= 1;
                m_data->ui.playerCount.setString(std::to_string(m_data->gameSettings.playerCount + 1));
            }
            else
            {
                m_data->gameSettings.playerCount = 3;
                m_data->ui.playerCount.setString(std::to_string(m_data->gameSettings.playerCount + 1));
            }
            break;

        case BOT_SELECTION:
        {
            int totalPlayers = m_data->gameSettings.playerCount + 1;
            int minBots = (totalPlayers == 1) ? 1 : 0;
            int maxBots = CalculateMaxBots(totalPlayers);
            
            if (m_data->ui.botCount > minBots)
            {
                m_data->ui.botCount -= 1;
            }
            else
            {
                m_data->ui.botCount = maxBots;
            }
            break;
        }

        // MODIFIÉ : Changer la difficulté du bot ACTUEL
        case BOT_DIFFICULTY_SELECTION:
            if (m_data->ui.botDifficulties[m_data->ui.currentBotIndex] > 0)
            {
                m_data->ui.botDifficulties[m_data->ui.currentBotIndex] -= 1;
            }
            else
            {
                m_data->ui.botDifficulties[m_data->ui.currentBotIndex] = 2; // Wrap vers Hard
            }
            break;

        case OPTIONS:
            m_data->audio->AddMusicVolume(-10.f);
            m_data->audio->AddSoundVolume(-10.f);
            break;
        }
        break;

    case PLAY_SELECTION:
    {
        switch (m_data->state)
        {
        case PLAYER_NB_SELECTION:
        {
            int totalPlayers = m_data->gameSettings.playerCount + 1;
            
            if (totalPlayers == 1)
            {
                m_data->ui.botCount = 1;
                m_data->state = BOT_SELECTION;
                m_data->controlerBtn = PLAY_SELECTION;
                
                m_data->ui.buttonMap["playBtn"].setPosition({SCREEN_WIDTH / 2, SCREEN_HEIGHT / 1.5});
                m_data->ui.buttonMap["moinsBtn"].setPosition({SCREEN_WIDTH / 2 - 0.2f * SCREEN_WIDTH, SCREEN_HEIGHT / 2});
                m_data->ui.buttonMap["plusBtn"].setPosition({SCREEN_WIDTH / 2 + 0.2f * SCREEN_WIDTH, SCREEN_HEIGHT / 2});
            }
            else if (totalPlayers == 2)
            {
                m_data->ui.botCount = 0;
                m_data->state = BOT_SELECTION;
                m_data->controlerBtn = PLAY_SELECTION;
                
                m_data->ui.buttonMap["playBtn"].setPosition({SCREEN_WIDTH / 2, SCREEN_HEIGHT / 1.5});
                m_data->ui.buttonMap["moinsBtn"].setPosition({SCREEN_WIDTH / 2 - 0.2f * SCREEN_WIDTH, SCREEN_HEIGHT / 2});
                m_data->ui.buttonMap["plusBtn"].setPosition({SCREEN_WIDTH / 2 + 0.2f * SCREEN_WIDTH, SCREEN_HEIGHT / 2});
            }
            else if (totalPlayers == 3)
            {
                m_data->ui.botCount = 0;
                m_data->state = BOT_SELECTION;
                m_data->controlerBtn = PLAY_SELECTION;
                
                m_data->ui.buttonMap["playBtn"].setPosition({SCREEN_WIDTH / 2, SCREEN_HEIGHT / 1.5});
                m_data->ui.buttonMap["moinsBtn"].setPosition({SCREEN_WIDTH / 2 - 0.2f * SCREEN_WIDTH, SCREEN_HEIGHT / 2});
                m_data->ui.buttonMap["plusBtn"].setPosition({SCREEN_WIDTH / 2 + 0.2f * SCREEN_WIDTH, SCREEN_HEIGHT / 2});
            }
            else if (totalPlayers == 4)
            {
                m_data->ui.botCount = 0;
                
                for (int i = 0; i < totalPlayers; i++)
                {
                    PlayerData newPlayer;
                    newPlayer.m_joystickId = i;
                    m_data->gameData->m_playerDataList.push_back(newPlayer);
                    m_data->charaSelected.push_back(false);
                    m_data->currentCharaSelected.push_back(0);
                }
                
                m_data->state = PLAYER_SELECTION;
                m_data->controlerBtn = PLAY;
            }
            break;
        }

        // MODIFIÉ : Préparer la sélection individuelle de difficulté
        case BOT_SELECTION:
        {
            int totalPlayers = m_data->gameSettings.playerCount + 1;
            int minBots = (totalPlayers == 1) ? 1 : 0;
            int maxBots = CalculateMaxBots(totalPlayers);
            
            if (m_data->ui.botCount < minBots)
            {
                m_data->ui.botCount = minBots;
            }
            
            if (m_data->ui.botCount > maxBots)
            {
                m_data->ui.botCount = maxBots;
            }
            
            // Si des bots sont sélectionnés, préparer la configuration individuelle
            if (m_data->ui.botCount > 0)
            {
                // NOUVEAU : Initialiser la liste des difficultés avec Medium par défaut
                m_data->ui.botDifficulties.clear();
                for (int i = 0; i < m_data->ui.botCount; i++)
                {
                    m_data->ui.botDifficulties.push_back(1); // 1 = Medium
                }
                m_data->ui.currentBotIndex = 0;
                
                m_data->state = BOT_DIFFICULTY_SELECTION;
                m_data->controlerBtn = PLAY_SELECTION;
                
                m_data->ui.buttonMap["playBtn"].setPosition({SCREEN_WIDTH / 2, SCREEN_HEIGHT / 1.5});
                m_data->ui.buttonMap["moinsBtn"].setPosition({SCREEN_WIDTH / 2 - 0.2f * SCREEN_WIDTH, SCREEN_HEIGHT / 2});
                m_data->ui.buttonMap["plusBtn"].setPosition({SCREEN_WIDTH / 2 + 0.2f * SCREEN_WIDTH, SCREEN_HEIGHT / 2});
            }
            else
            {
                // Pas de bots : passer directement à la sélection des personnages
                for (int i = 0; i < totalPlayers; i++)
                {
                    PlayerData newPlayer;
                    newPlayer.m_joystickId = i;
                    m_data->gameData->m_playerDataList.push_back(newPlayer);
                    m_data->charaSelected.push_back(false);
                    m_data->currentCharaSelected.push_back(0);
                }
                
                m_data->state = PLAYER_SELECTION;
                m_data->controlerBtn = PLAY;
            }
            break;
        }

        // MODIFIÉ : Validation de la difficulté du bot actuel
        case BOT_DIFFICULTY_SELECTION:
        {
            m_data->ui.currentBotIndex++;
            
            // Si tous les bots ont été configurés
            if (m_data->ui.currentBotIndex >= m_data->ui.botCount)
            {
                int totalPlayers = m_data->gameSettings.playerCount + 1;
                int totalFinal = totalPlayers + m_data->ui.botCount;
                int humanPlayers = totalPlayers;
                
                std::cout << "Création : Humains=" << humanPlayers 
                          << ", Bots=" << m_data->ui.botCount 
                          << ", Total Final=" << totalFinal << std::endl;
                
                if (humanPlayers < 0 || totalFinal < 2 || totalFinal > 4)
                {
                    std::cout << "ERREUR CRITIQUE : Configuration invalide !" << std::endl;
                    return;
                }

                // Créer les joueurs humains
                for (int i = 0; i < humanPlayers; i++)
                {
                    PlayerData newPlayer;
                    newPlayer.m_joystickId = i;
                    m_data->gameData->m_playerDataList.push_back(newPlayer);
                    m_data->charaSelected.push_back(false);
                    m_data->currentCharaSelected.push_back(0);
                }

                // MODIFIÉ : Créer les bots avec leur difficulté INDIVIDUELLE
                for (int i = humanPlayers; i < totalFinal; i++)
                {
                    int botIndex = i - humanPlayers;
                    
                    PlayerData botPlayer;
                    
                    // NOUVEAU : Initialiser correctement le bot avec son index et sa difficulté
                    BotDifficulty selectedDifficulty = GetBotDifficultyEnum(m_data->ui.botDifficulties[botIndex]);
                    botPlayer.InitializeAsBot(i, selectedDifficulty);
                    
                    m_data->gameData->m_playerDataList.push_back(botPlayer);
                    m_data->charaSelected.push_back(true);
                    
                    int randomSkin = randmt::RandomInt(0, 3);
                    m_data->currentCharaSelected.push_back(randomSkin);
                    
                    std::cout << "Bot " << (botIndex + 1) << " (joueur " << i << ") créé avec difficulté " 
                              << GetDifficultyName(m_data->ui.botDifficulties[botIndex]) 
                              << " (enum=" << static_cast<int>(selectedDifficulty) << ")" << std::endl;
                }

                m_data->state = PLAYER_SELECTION;
                m_data->controlerBtn = PLAY;
                std::cout << "Transition vers PLAYER_SELECTION réussie avec " 
                          << m_data->gameData->m_playerDataList.size() << " joueurs" << std::endl;
            }
            // Sinon, afficher le prochain bot à configurer (ne rien faire ici)
            break;
        }

        case OPTIONS:
            m_data->ui.playerCount.setCharacterSize(200u);
            m_data->ui.playerCount.setOrigin({0.6f, 0.8f});
            m_data->state = MAIN_MENU;
            m_data->controlerBtn = PLAY;
            sf::FloatRect buttonRect = m_data->ui.buttonMap["playBtn"].getLocalBounds();
            m_data->ui.buttonMap["playBtn"].setPosition({SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2});
            m_data->ui.buttonMap["settingsBtn"].setPosition({SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + buttonRect.height});
            m_data->ui.buttonMap["leaveBtn"].setPosition({SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 2 * buttonRect.height});
            m_data->ui.buttonMap["creditsBtn"].setPosition({SCREEN_WIDTH - buttonRect.width / 2, SCREEN_HEIGHT / 2 + 2 * buttonRect.height});
            break;
        }
        break;
    }

    case MORE:
        switch (m_data->state)
        {
        case PLAYER_NB_SELECTION:
            if (m_data->gameSettings.playerCount < MAX_PLAYERS)
            {
                m_data->gameSettings.playerCount += 1;
                m_data->ui.playerCount.setString(std::to_string(m_data->gameSettings.playerCount + 1));
            }
            else
            {
                m_data->gameSettings.playerCount = MIN_PLAYERS;
                m_data->ui.playerCount.setString(std::to_string(m_data->gameSettings.playerCount + 1));
            }
            break;

        case BOT_SELECTION:
        {
            int totalPlayers = m_data->gameSettings.playerCount + 1;
            int minBots = (totalPlayers == 1) ? 1 : 0;
            int maxBots = CalculateMaxBots(totalPlayers);
            
            if (m_data->ui.botCount < maxBots)
            {
                m_data->ui.botCount += 1;
            }
            else
            {
                m_data->ui.botCount = minBots;
            }
            break;
        }

        // MODIFIÉ : Changer la difficulté du bot ACTUEL
        case BOT_DIFFICULTY_SELECTION:
            if (m_data->ui.botDifficulties[m_data->ui.currentBotIndex] < 2)
            {
                m_data->ui.botDifficulties[m_data->ui.currentBotIndex] += 1;
            }
            else
            {
                m_data->ui.botDifficulties[m_data->ui.currentBotIndex] = 0; // Wrap vers Easy
            }
            break;

        case OPTIONS:
            m_data->audio->AddMusicVolume(10.f);
            m_data->audio->AddSoundVolume(10.f);
            break;
        }
        break;
    }
}

void Menu::ReturnPressed(void)
{
	m_data->audio->PlaySound("uiSoundClick");
	switch (m_data->state)
	{
	case OPTIONS:
	case PLAYER_NB_SELECTION:
	{
		m_data->state = MAIN_MENU;
		sf::FloatRect buttonRect = m_data->ui.buttonMap["playBtn"].getGlobalBounds();
		m_data->ui.buttonMap["playBtn"].setPosition({SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2});
		m_data->ui.buttonMap["settingsBtn"].setPosition({SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + buttonRect.height});
		m_data->ui.buttonMap["leaveBtn"].setPosition({SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 2 * buttonRect.height});
		m_data->ui.buttonMap["creditsBtn"].setPosition({SCREEN_WIDTH - buttonRect.width / 2, SCREEN_HEIGHT / 2 + 2 * buttonRect.height});
	}
	break;

	case BOT_SELECTION:
		m_data->state = PLAYER_NB_SELECTION;
		m_data->controlerBtn = PLAY_SELECTION;
		m_data->ui.buttonMap["plusBtn"].setScale(1, 1);
		m_data->ui.buttonMap["moinsBtn"].setScale(1, 1);
		m_data->ui.buttonMap["playBtn"].setPosition({SCREEN_WIDTH / 2, SCREEN_HEIGHT / 1.5});
		m_data->ui.buttonMap["moinsBtn"].setPosition({SCREEN_WIDTH / 2 - 0.2f * SCREEN_WIDTH, SCREEN_HEIGHT / 2});
		m_data->ui.buttonMap["plusBtn"].setPosition({SCREEN_WIDTH / 2 + 0.2f * SCREEN_WIDTH, SCREEN_HEIGHT / 2});
		m_data->ui.playerCount.setString(std::to_string(m_data->gameSettings.playerCount + 1));
		m_data->ui.playerCount.setCharacterSize(200u);
		m_data->ui.playerCount.setOrigin({0.6f, 0.8f});
		m_data->ui.playerCount.setPosition({SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2});
		break;

	// MODIFIÉ : Retour depuis l'écran de difficulté
	case BOT_DIFFICULTY_SELECTION:
		// Si on était sur le premier bot, retourner à la sélection du nombre
		if (m_data->ui.currentBotIndex == 0)
		{
			m_data->state = BOT_SELECTION;
			m_data->controlerBtn = PLAY_SELECTION;
			m_data->ui.buttonMap["plusBtn"].setScale(1, 1);
			m_data->ui.buttonMap["moinsBtn"].setScale(1, 1);
			m_data->ui.buttonMap["playBtn"].setPosition({SCREEN_WIDTH / 2, SCREEN_HEIGHT / 1.5});
			m_data->ui.buttonMap["moinsBtn"].setPosition({SCREEN_WIDTH / 2 - 0.2f * SCREEN_WIDTH, SCREEN_HEIGHT / 2});
			m_data->ui.buttonMap["plusBtn"].setPosition({SCREEN_WIDTH / 2 + 0.2f * SCREEN_WIDTH, SCREEN_HEIGHT / 2});
			
			// Nettoyer les difficultés
			m_data->ui.botDifficulties.clear();
		}
		else
		{
			// Sinon, revenir au bot précédent
			m_data->ui.currentBotIndex--;
		}
		break;

	case PLAYER_SELECTION:
	{
		int totalPlayers = m_data->gameSettings.playerCount + 1;
		
		m_data->gameData->m_playerDataList.clear();
		m_data->charaSelected.clear();
		m_data->currentCharaSelected.clear();
		
		// MODIFIÉ : Retour vers la difficulté du dernier bot
		if (totalPlayers == 4)
		{
			m_data->state = PLAYER_NB_SELECTION;
		}
		else if (m_data->ui.botCount > 0)
		{
			// Revenir à la configuration du dernier bot
			m_data->ui.currentBotIndex = m_data->ui.botCount - 1;
			m_data->state = BOT_DIFFICULTY_SELECTION;
		}
		else
		{
			m_data->state = BOT_SELECTION;
		}
		
		m_data->controlerBtn = PLAY_SELECTION;
		m_data->ui.buttonMap["plusBtn"].setScale(1, 1);
		m_data->ui.buttonMap["moinsBtn"].setScale(1, 1);
		m_data->ui.buttonMap["playBtn"].setPosition({SCREEN_WIDTH / 2, SCREEN_HEIGHT / 1.5});
		m_data->ui.buttonMap["moinsBtn"].setPosition({SCREEN_WIDTH / 2 - 0.2f * SCREEN_WIDTH, SCREEN_HEIGHT / 2});
		m_data->ui.buttonMap["plusBtn"].setPosition({SCREEN_WIDTH / 2 + 0.2f * SCREEN_WIDTH, SCREEN_HEIGHT / 2});
		
		if (m_data->state == PLAYER_NB_SELECTION)
		{
			m_data->ui.playerCount.setString(std::to_string(m_data->gameSettings.playerCount + 1));
			m_data->ui.playerCount.setCharacterSize(200u);
		 m_data->ui.playerCount.setOrigin({0.6f, 0.8f});
			m_data->ui.playerCount.setPosition({SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2});
		}
	}
	break;
	}
}

// MODIFIÉE : Afficher les difficultés individuelles
void Menu::PrintIcons(sfMod::RenderWindow* _renderWindow)
{
    int totalPlayers = m_data->gameSettings.playerCount + 1;
    int totalPlayersWithBots = (int)m_data->gameData->m_playerDataList.size();
    int humanPlayers = totalPlayers;
    
    std::cout << "DEBUG PrintIcons: humanPlayers=" << humanPlayers 
              << ", totalPlayersWithBots=" << totalPlayersWithBots 
              << ", botCount=" << m_data->ui.botCount << std::endl;

    sf::Color tempColor = m_data->ui.iconsChara.getColor();

    float border = 200.f;
    float iconSpacing = (SCREEN_WIDTH - 2 * border) / std::max(1, humanPlayers - 1);
    if (humanPlayers == 1)
    {
        iconSpacing = 0;
    }

    for (int i = 0; i < humanPlayers; i++)
    {
        sf::Vector2f iconPos[] =
        {
            {(float)(border + i * iconSpacing), SCREEN_HEIGHT / 2.f - SCREEN_HEIGHT / 4.f},
            {(float)(border + i * iconSpacing), SCREEN_HEIGHT / 2.f },
            {(float)(border + i * iconSpacing), SCREEN_HEIGHT / 2.f + SCREEN_HEIGHT / 4.f}
        };

        sf::Vector2f pos = { iconPos[0].x, iconPos[0].y - 70.f };
        m_data->ui.playerCount.setPosition(pos);

        char buffer[50];
        std::snprintf(buffer, 50, "Player : %d", i + 1);
        m_data->ui.playerCount.setString(buffer);
        m_data->ui.playerCount.setCharacterSize(60u);
        _renderWindow->draw(m_data->ui.playerCount);

        int currentSelection = m_data->currentCharaSelected[i];
        int totalCharacters = (int)m_data->ui.charaAvaible.size();

        int leftIndex = (currentSelection - 1 + totalCharacters) % totalCharacters;
        int centerIndex = currentSelection;
        int rightIndex = (currentSelection + 1) % totalCharacters;

        tempColor.a = 100;
        m_data->ui.iconsChara.setColor(tempColor);
        m_data->ui.iconsChara.SetAnimation(m_data->ui.charaAvaible[leftIndex]);
        m_data->ui.iconsChara.setPosition(iconPos[0]);
        _renderWindow->draw(m_data->ui.iconsChara);

        tempColor.a = 255;
        if (m_data->charaSelected[i])
        {
            m_data->ui.iconsChara.setColor(sf::Color(150, 150, 150, 255));
        }
        else
        {
            m_data->ui.iconsChara.setColor(tempColor);
        }
        m_data->ui.iconsChara.SetAnimation(m_data->ui.charaAvaible[centerIndex]);
        m_data->ui.iconsChara.setPosition(iconPos[1]);
        _renderWindow->draw(m_data->ui.iconsChara);

        tempColor.a = 100;
        m_data->ui.iconsChara.setColor(tempColor);
        m_data->ui.iconsChara.SetAnimation(m_data->ui.charaAvaible[rightIndex]);
        m_data->ui.iconsChara.setPosition(iconPos[2]);
        _renderWindow->draw(m_data->ui.iconsChara);
    }

    // MODIFIÉ : Afficher les difficultés individuelles des bots
    if (m_data->ui.botCount > 0)
    {
        m_data->ui.playerCount.setCharacterSize(35u);
        m_data->ui.playerCount.setPosition({SCREEN_WIDTH / 2.f, SCREEN_HEIGHT - 120.f});
        
        char botBuffer[200];
        std::string botDetails = "";
        
        for (int i = 0; i < m_data->ui.botCount; i++)
        {
            botDetails += "Bot " + std::to_string(i + 1) + ": " + 
                          GetDifficultyName(m_data->ui.botDifficulties[i]);
            
            if (i < m_data->ui.botCount - 1)
            {
                botDetails += " | ";
            }
        }
        
        std::snprintf(botBuffer, 200, "%d Bot(s) en attente (skins aleatoires)\n%s", 
                      m_data->ui.botCount, botDetails.c_str());
        m_data->ui.playerCount.setString(botBuffer);
        m_data->ui.playerCount.setOrigin({m_data->ui.playerCount.getGlobalBounds().width / 2.f, 0});
        _renderWindow->draw(m_data->ui.playerCount);
    }

    m_data->ui.iconsChara.setColor(sf::Color(255, 255, 255, 255));
}

void Menu::PrintOptions(sfMod::RenderWindow* _renderWindow)
{
    m_data->ui.playerCount.setCharacterSize(50u);
    m_data->ui.playerCount.setOrigin({ 0.f,0.f });
    m_data->ui.playerCount.setPosition(0.f, 1.8f / 4.f * SCREEN_HEIGHT);
    m_data->ui.playerCount.setCharacterSize(100u);
    char buffer[100];
    std::snprintf(buffer, 100, "Game volume : %0.0f", m_data->audio->GetMusicVolume());
    m_data->ui.playerCount.setString(buffer);
    _renderWindow->draw(m_data->ui.playerCount);

    m_data->ui.playerCount.setCharacterSize(150u);
    m_data->ui.playerCount.setOrigin({ 0.6f,0.8f });
    m_data->ui.playerCount.setPosition(SCREEN_WIDTH / 2.f, 1.f / 4.f * SCREEN_HEIGHT);
    m_data->ui.playerCount.setString("Settings");
    _renderWindow->draw(m_data->ui.playerCount);

    _renderWindow->draw(m_data->ui.buttonMap["plusBtn"]);
    _renderWindow->draw(m_data->ui.buttonMap["moinsBtn"]);
    _renderWindow->draw(m_data->ui.buttonMap["playBtn"]);
}

// Ajouter ces fonctions à la fin du fichier Menu.cpp

int Menu::CalculateMaxBots(int totalPlayers)
{
    if (totalPlayers == 1)
    {
        return 3;
    }
    else if (totalPlayers == 2)
    {
        return 2;
    }
    else if (totalPlayers == 3)
    {
        return 1;
    }
    else if (totalPlayers == 4)
    {
        return 0;
    }

    return 0;
}

std::string Menu::GetDifficultyName(int difficulty) const
{
    switch (difficulty)
    {
    case 0: return "Easy";
    case 1: return "Medium";
    case 2: return "Hard";
    default: return "Medium";
    }
}

BotDifficulty Menu::GetBotDifficultyEnum(int difficulty) const
{
    switch (difficulty)
    {
    case 0: return BotDifficulty::EASY;
    case 1: return BotDifficulty::MEDIUM;
    case 2: return BotDifficulty::HARD;
    default: return BotDifficulty::MEDIUM;
    }
}

PlayerData::PlayerSkin Menu::GetPlayerSkinFromIndex(int _index) const
{
    static const PlayerData::PlayerSkin skins[] = {
        PlayerData::CHARACTER_1_1,
        PlayerData::CHARACTER_2_1,
        PlayerData::CHARACTER_3_1,
        PlayerData::CHARACTER_4_1
    };

    return (_index >= 0 && _index < 4) ? skins[_index] : PlayerData::CHARACTER_1_1;
}

void Menu::ChangeSelection(int _value, int _joystick)
{
    m_data->audio->PlaySound("uiSoundON");

    switch (m_data->state)
    {
    case MAIN_MENU:
        HandleMainMenuSelection(_value);
        break;
    case OPTIONS:
    case PLAYER_NB_SELECTION:
    case BOT_SELECTION:
    case BOT_DIFFICULTY_SELECTION:
        HandleOptionsSelection(_value);
        break;
    case PLAYER_SELECTION:
        HandlePlayerSelection(_value, _joystick);
        break;
    }
}

void Menu::HandleMainMenuSelection(int _value)
{
    int newBtn = m_data->controlerBtn + _value;

    // Wrap circulaire
    if (newBtn < PLAY)
        m_data->controlerBtn = CREDITS_BTN;
    else if (newBtn > CREDITS_BTN)
        m_data->controlerBtn = PLAY;
    else
        m_data->controlerBtn = (ControlerCurrentButton)newBtn;

    sf::Vector2f mouseNewPos = GetButtonPosition(m_data->controlerBtn);
    sf::Mouse::setPosition(sf::Vector2i(mouseNewPos), *m_data->gameData->m_renderWindow);
}

void Menu::HandleOptionsSelection(int _value)
{
    int newBtn = m_data->controlerBtn + _value;

    // Wrap circulaire entre LESS et MORE
    if (newBtn > MORE)
        m_data->controlerBtn = LESS;
    else if (newBtn < LESS)
        m_data->controlerBtn = MORE;
    else
        m_data->controlerBtn = (ControlerCurrentButton)newBtn;

    sf::Vector2f mouseNewPos = GetButtonPosition(m_data->controlerBtn);
    sf::Mouse::setPosition(sf::Vector2i(mouseNewPos), *m_data->gameData->m_renderWindow);
}

void Menu::HandlePlayerSelection(int _value, int _joystick)
{
    if (m_data->charaSelected[_joystick])
        return;

    int totalCharacters = (int)m_data->ui.charaAvaible.size();
    int newSelection = m_data->currentCharaSelected[_joystick] + _value;

    // Wrap circulaire
    m_data->currentCharaSelected[_joystick] =
        (newSelection + totalCharacters) % totalCharacters;
}

sf::Vector2f Menu::GetButtonPosition(ControlerCurrentButton _button) const
{
    static const std::map<ControlerCurrentButton, std::string> buttonNameMap = {
        {PLAY, "playBtn"},
        {SETTINGS, "settingsBtn"},
        {LEAVE, "leaveBtn"},
        {CREDITS_BTN, "creditsBtn"},
        {MORE, "plusBtn"},
        {LESS, "moinsBtn"},
        {PLAY_SELECTION, "playBtn"}
    };

    auto it = buttonNameMap.find(_button);
    if (it != buttonNameMap.end())
    {
        return m_data->ui.buttonMap.at(it->second).getPosition();
    }
    return sf::Vector2f(0, 0);
}