#include "RandCard.hpp"
#include "../../../Utilities/MathPlus.hpp"

#define CARD_SIZE_X 40.f
#define CARD_SIZE_Y 80.f
#define BORDER_X 40.f


void RandCard::Load(void)
{
	m_data = new SceneData;

	m_data->gameData = (GameData*)this->m_keptData;
	m_data->gameData->m_assetManager->LoadManifest("Manifests/RandCard.json", "RandCard");
	m_data->audio = (AudioEngine*)m_data->gameData->m_audioEngine;

	m_data->timer.SetTimeTarget(3.f);

	int nbOfPlayers = (int)m_data->gameData->m_gonnaPlayIndex.size();
	std::cout << "nb of player " << nbOfPlayers << std::endl;

	//Copy players playig from GameData
	for (int i = 0; i < nbOfPlayers; ++i)
	{
		std::cout << "Add player\n";
		int playerId = (int)m_data->gameData->m_gonnaPlayIndex.at(i);
		m_data->players.push_back({ (short)playerId });
	}

	//Font
	m_data->text.setFont(*m_data->gameData->m_assetManager->GetAsset<sf::Font>("MenuFont"));
	m_data->text.setCharacterSize(100u);
	m_data->text.setOrigin({ 0.6f, 0.8f });
	m_data->text.setPosition({ SCREEN_WIDTH / 2.f, 0.8 * SCREEN_HEIGHT });
	m_data->text.setString("Choose card");


	//Anim Card
	m_data->cardChosenSprAnim.setTexture(*m_data->gameData->m_assetManager->GetAsset<TextureAnimated>("AnimCard", AssetManager::AssetType::TEXTURE_ANIMATED));
	m_data->cardUnchosenSprAnim.setTexture(*m_data->gameData->m_assetManager->GetAsset<TextureAnimated>("AnimCard", AssetManager::AssetType::TEXTURE_ANIMATED));

	m_data->cardChosenSprAnim.setScale(0.25f, 0.25f);
	m_data->cardChosenSprAnim.setOrigin({ 0.5f, 0.5f });
	m_data->cardUnchosenSprAnim.setScale(0.2f, 0.2f);
	m_data->cardUnchosenSprAnim.setOrigin({ 0.5f, 0.5f });

	//Background
	m_data->background.setTexture(*m_data->gameData->m_assetManager->GetAsset<sf::Texture>("MinigameBackground", AssetManager::AssetType::TEXTURE));

	//Icons
	m_data->iconsChara.setTexture(*m_data->gameData->m_assetManager->GetAsset<TextureAnimated>("Icone", AssetManager::AssetType::TEXTURE_ANIMATED));
	m_data->iconsChara.SetAnimation("Perso1-1");
	m_data->iconsChara.setOrigin({ 0.5f,0.5f });

	int nbOfNormalCard = 2 * int(m_data->players.size());
	int nbOfBomb = 1 * int(m_data->players.size());
	int totalCard = nbOfNormalCard + nbOfBomb;

	//Initialize random cards
	for (int i = 0; i < totalCard; ++i)
	{
		if (nbOfNormalCard > 0 && nbOfBomb > 1)
		{
			int randomNb = rand() % 2;
			if (randomNb == 0)
			{
				m_data->cards.push_back(NORMAL);
				nbOfNormalCard--;
			}
			else
			{
				m_data->cards.push_back(BOMB);
				nbOfBomb--;
			}
		}
		else if (nbOfNormalCard > 0)
		{
			m_data->cards.push_back(NORMAL);
			nbOfNormalCard--;
		}
		else
		{
			m_data->cards.push_back(BOMB);
			nbOfBomb--;
		}
	}
	//DEBUG : print all cards in terminal
	for (int i = 0; i < m_data->cards.size(); ++i)
	{
		std::cout << "Card n " << i << " : " << m_data->cards[i] << std::endl;
	}

	m_data->transition.SetTransition(TransitionClass::FADED_IN);
	m_data->transition.PlayTransition();
}
void RandCard::Unload(void)
{
	m_data->cards.clear();
	m_data->players.clear();
	m_data->deadPlayers.clear();

	delete this->m_data;
	this->m_data = NULL;
}

void RandCard::PollEvent(sf::Event& _event)
{
	switch (m_data->gameState)
	{
	case CHOOSE_CARD:

		switch (_event.type)
		{
		case sf::Event::JoystickButtonPressed:

			if (_event.joystickButton.joystickId == m_data->currentPlayer)
			{
				//Confirm choice
				if (m_data->cardChosen != -1)
				{
					CardType cardType = m_data->cards[m_data->cardChosen];
					m_data->audio->PlaySound("cardReturn");
					switch (cardType)
					{
					case NORMAL:
						m_data->cardChosenSprAnim.SetAnimation("NORMAL");
						m_data->text.setString("This card is safe");
						break;

					case BOMB:
						m_data->cardChosenSprAnim.SetAnimation("BOMB");
						m_data->text.setString("Oh no, you picked the bomb");
						break;
					}
					m_data->gameState = ANIMATION;
					m_data->cardChosenSprAnim.Restart();
				}
			}
			break;
		case sf::Event::KeyPressed:
			break;
		case sf::Event::JoystickMoved:

			//X Y joystick gauche
			//U V joystick droite
			//Z R pression des gachettes
			//La croix povX povY
			if (_event.joystickButton.joystickId == m_data->currentPlayer)
			{

				if (m_data->inputDelay > 0.1f)
				{
					switch (_event.joystickMove.axis)
					{
					case sf::Joystick::Axis::X:
					case sf::Joystick::Axis::U:

						//Move the position of card chosen
						AddCardChosen((int)roundf(_event.joystickMove.position / 100.f));
						m_data->inputDelay = 0.f;
						break;

					default:
						break;
					}
				}
			}
			break;

		default:
			break;
		}
		break;

	case END:
		if (_event.type == sf::Event::KeyPressed ||
			_event.type == sf::Event::JoystickButtonPressed)
		{
			m_data->transition.SetTransition(TransitionClass::FADED_OUT);
			m_data->gameState = TRANSITION;

		}
	}
}
void RandCard::Update(float _deltaTime)
{
	switch (m_data->gameState)
	{
	case WAITING_BETWEEN_PLAYER:

		m_data->timer.Update(_deltaTime);

		if (m_data->timer.IsFinished())
		{
			m_data->cards.erase(m_data->cards.begin() + m_data->cardChosen);

			m_data->cardChosenSprAnim.Update(_deltaTime);
			m_data->text.setString("Choose card");
			//Restart card
			m_data->cardChosenSprAnim.SetAnimation("IDLE");
			m_data->timer.Restart();
			NextPlayer();
		}
		break;

	case CHOOSE_CARD:

		m_data->inputDelay += _deltaTime;
		break;

	case ANIMATION:

		m_data->cardChosenSprAnim.Update(_deltaTime);
		if (m_data->cardChosenSprAnim.IsFinished())
		{
			if (m_data->cards[m_data->cardChosen] == BOMB)
			{
				m_data->deadPlayers.push_back(m_data->players.at(m_data->currentPlayer));

				m_data->players.erase(m_data->players.begin() + m_data->currentPlayer);
				m_data->audio->PlaySound("loose");
				m_data->gameState = WAITING_BETWEEN_PLAYER;
			}
			else
			{
				m_data->gameState = WAITING_BETWEEN_PLAYER;
			}
		}
		break;
	case TRANSITION:
		m_data->transition.Update(_deltaTime);

		if (m_data->transition.IsFinished())
		{
			//If this vector is empty, the transition finished is the first
			if ((int)m_data->deadPlayers.size() == 0)
			{
				m_data->gameState = CHOOSE_CARD;
			}
			else
			{
				//Return to main menu
				ChangeScene("RussianRoulette");
			}
		}



		break;

	}
}

void RandCard::Draw(sf::RenderWindow& _renderWindow)
{
	_renderWindow.draw(m_data->background);
	//Cards and icons are printed in all states
	PrintCards(_renderWindow);
	PrintIcons(_renderWindow);
	switch (m_data->gameState)
	{
		case TRANSITION:
		{
			m_data->transition.Draw(_renderWindow);
		}
		break;
		case WAITING_BETWEEN_PLAYER:
		case CHOOSE_CARD:
			_renderWindow.draw(m_data->text);
			break;
	}
}


void RandCard::PrintCards(sf::RenderWindow& _renderWindow)
{
	//For placement 
	float border = 200.f;
	float cardSpacing = (float)(SCREEN_WIDTH - 2.f * border) / ((float)m_data->cards.size() - 1);

	for (int i = 0; i < m_data->cards.size(); i++)
	{
		sf::Vector2f pos = { (float)(border + i * cardSpacing), SCREEN_HEIGHT / 2.f };

		if (i == m_data->cardChosen)
		{
			//m_data->cardChosenSprAnim.setPosition(BORDER_X + cardSpacing * i, SCREEN_HEIGHT / 2.f);
			m_data->cardChosenSprAnim.setPosition(pos);
			_renderWindow.draw(m_data->cardChosenSprAnim);
		}
		else
		{
			//m_data->cardUnchosenSprAnim.setPosition(BORDER_X + cardSpacing * i, SCREEN_HEIGHT / 2.f);
			m_data->cardUnchosenSprAnim.setPosition(pos);
			_renderWindow.draw(m_data->cardUnchosenSprAnim);
		}
	}
}
void RandCard::PrintIcons(sf::RenderWindow& _renderWindow)
{
	//For placement 
	float border = 200.f;
	float iconSpacing = (float)(SCREEN_WIDTH - 2.f * border) / ((float)m_data->players.size() - 1);

	for (int i = 0; i < (int)m_data->players.size(); i++)
	{
		sf::Vector2f pos = { (float)(border + i * iconSpacing), SCREEN_HEIGHT * 0.07f };

		m_data->iconsChara.SetAnimation(m_data->gameData->m_playerDataList[m_data->players[i].id].GetJoystickId());
		m_data->iconsChara.setPosition(pos);
		if (m_data->currentPlayer == i)
		{
			m_data->iconsChara.setScale({ 1.2f,1.2f });
			m_data->iconsChara.setColor({ 255,255,255,255 });
		}
		_renderWindow.draw(m_data->iconsChara);
		m_data->iconsChara.setScale({ 1.0f,1.0f });
		m_data->iconsChara.setColor({ 255,255,255,150 });
	}
}

void RandCard::SetCardChosen(int _card)
{
	m_data->cardChosen = mathp::ModuloPositiveI(_card, (int)m_data->cards.size());
}
void RandCard::AddCardChosen(int _value)
{
	this->SetCardChosen(m_data->cardChosen + _value);
}

void RandCard::NextPlayer(void)
{
	//Check if only one player left
	if (m_data->players.size() - 1 < 1)
	{
		m_data->deadPlayers.push_back(m_data->players.at(0));
		//Print if you want check
		m_data->text.setString("Press button to exit");
		m_data->gameState = END;

		//Save data
		int nbOfPlayers = (int)m_data->gameData->m_gonnaPlayIndex.size() - 1;

		for (int i = (int)m_data->deadPlayers.size() - 1; i >= 0; i--)
		{
			//Print if you want check
			m_data->gameData->AddPlayerWin(m_data->deadPlayers.at(i).id);
		}
		SceneBase::ChangeScene("Board", false);
		return;
	}
	if (m_data->currentPlayer + 1 > m_data->players.size() - 1)
	{
		//Check if the current player was the last in the vector 
		m_data->currentPlayer = 0;
	}
	else
	{
		//Next player
		m_data->currentPlayer++;
	}
	m_data->cardChosen = 0;
	m_data->gameState = CHOOSE_CARD;
}