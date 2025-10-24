#include "FlagGame.hpp"

std::vector<FlagGamePlayer*> allPlayers;
short leftPlayer = 0;

void EleminatePlayer(short _playerID);
bool SomeOneWon(void);
short GetWinerID(void);
GamePadBindList GetRandomGamePadInput(void);
bool WhiteListGamePadInput(GamePadBindList _input);

void FlagGame::Load(void)
{
	m_data = new SceneData();
	m_data->round = 1;
	m_data->timerBeforeNextRound = 5.0f;
	m_data->titleText = new sf::Text();
	m_data->titleText->setFont(StringFormat::GetDefaultFont());
	m_data->titleText->setCharacterSize(24);
	m_data->titleText->setFillColor(sf::Color::White);
	m_data->titleText->setString("Flag Game Mini-Game");
	m_data->titleText->setPosition(SCREEN_WIDTH / 2, 0);
	m_data->titleText->setOrigin(m_data->titleText->getLocalBounds().width / 2, 0);

	m_data->timerText = new sf::Text();
	m_data->timerText->setFont(StringFormat::GetDefaultFont());
	m_data->timerText->setCharacterSize(25);
	m_data->timerText->setFillColor(sf::Color::White);
	m_data->timerText->setString("Time: 5");
	m_data->timerText->setPosition(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 3);
	m_data->timerText->setOrigin(m_data->timerText->getLocalBounds().width / 2, m_data->timerText->getLocalBounds().height / 2);

	m_data->countRoundText = new sf::Text();
	m_data->countRoundText->setFont(StringFormat::GetDefaultFont());
	m_data->countRoundText->setCharacterSize(25);
	m_data->countRoundText->setFillColor(sf::Color::Blue);
	m_data->countRoundText->setString("Round: 1");
	m_data->countRoundText->setPosition(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 4);
	m_data->countRoundText->setOrigin(m_data->countRoundText->getLocalBounds().width / 2, m_data->countRoundText->getLocalBounds().height / 2);

	m_data->noEnoughtPlayer = new sf::Text();
	m_data->noEnoughtPlayer->setFont(StringFormat::GetDefaultFont());
	m_data->noEnoughtPlayer->setCharacterSize(25);
	m_data->noEnoughtPlayer->setFillColor(sf::Color::Red);
	m_data->noEnoughtPlayer->setString("Not Enought Players Connected!");
	m_data->noEnoughtPlayer->setPosition(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
	m_data->noEnoughtPlayer->setOrigin(m_data->noEnoughtPlayer->getLocalBounds().width / 2, m_data->noEnoughtPlayer->getLocalBounds().height / 2);

	m_data->randomDelayforChangeInput = random::RandomFloat(0.1f, 3.0f);
	m_data->randomInputID = (short)GetRandomGamePadInput();
	m_data->InputText = new sf::Text();
	m_data->InputText->setFont(StringFormat::GetDefaultFont());
	m_data->InputText->setCharacterSize(25);
	m_data->InputText->setFillColor(sf::Color::Yellow);
	m_data->InputText->setString(StringFormat::Format("Input: %s",GetGamePadButtonName((GamePadBindList)m_data->randomInputID)));
	m_data->InputText->setPosition(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2.5f);
	m_data->InputText->setOrigin(m_data->InputText->getLocalBounds().width / 2, m_data->InputText->getLocalBounds().height / 2);

	for(short i = 0; i < 4; ++i)
	{
		if (sf::Joystick::isConnected(i))
		{
			new FlagGamePlayer(i);
			leftPlayer++;
		}
	}
}

void FlagGame::Unload(void)
{
	delete m_data->titleText;
	delete m_data->countRoundText;
	delete m_data->noEnoughtPlayer;
	delete m_data->timerText;
	delete m_data->InputText;
	m_data->titleText = nullptr;
	m_data->countRoundText = nullptr;
	m_data->noEnoughtPlayer = nullptr;
	m_data->timerText = nullptr;
	m_data->InputText = nullptr;
	delete m_data;
	m_data = nullptr;

	for (FlagGamePlayer* player : allPlayers)
	{
		delete player;
	}
}

void FlagGame::PollEvent(sf::Event& _event)
{
}

void FlagGame::Update(float _deltaTime)
{
	short countPlayer = 0;
	for(short i = 0; i < (short)sf::Joystick::Count; ++i)
	{
		if(sf::Joystick::isConnected(i))
		{
			countPlayer++;
		}
	}
	if(countPlayer >= 2)
	{
		if (m_data->round > MAX_ROUND || leftPlayer == 0)
		{
			if(SomeOneWon() && leftPlayer == 1)
			{
				std::cout << "We have a winner " << GetWinerID()+1 << std::endl;
			}
			else if(leftPlayer > 1)
			{
				std::cout << "No one won the game." << std::endl;
				for(FlagGamePlayer* player : allPlayers)
				{
					EleminatePlayer(player->GetID());
				}
			}
			ResetFlagGame();
			return;
		}
		m_data->timerBeforeNextRound -= _deltaTime;
		m_data->randomDelayforChangeInput -= _deltaTime;
		m_data->timerText->setString(StringFormat::Format("Time: %.2f", m_data->timerBeforeNextRound));
		m_data->timerText->setOrigin(m_data->timerText->getLocalBounds().width / 2, m_data->timerText->getLocalBounds().height / 2);
		if(m_data->timerBeforeNextRound <= 0.f)
		{
			NextRound();
		}
		if(m_data->randomDelayforChangeInput <= 0.f)
		{
			NextInput();
		}
		FlagGamePlayer::UpdateAllPlayers(_deltaTime);
	}
}

void FlagGame::Draw(sf::RenderWindow& _renderWindow)
{
	short countPlayer = 0;
	for (short i = 0; i < (short)sf::Joystick::Count; ++i)
	{
		if (sf::Joystick::isConnected(i))
		{
			countPlayer++;
		}
	}
	if (countPlayer < 2)
	{
		_renderWindow.draw(*m_data->noEnoughtPlayer);
		return;
	}

	_renderWindow.draw(*m_data->titleText);
	_renderWindow.draw(*m_data->countRoundText);
	_renderWindow.draw(*m_data->timerText);
	_renderWindow.draw(*m_data->InputText);
	FlagGamePlayer::DrawAllPlayers(_renderWindow);
}

FlagGamePlayer::FlagGamePlayer(short _id)
{
	id = _id;
	inGame = true;
	savedInput = (GamePadBindList)(-1);
	std::cout << "FlagGamePlayer " << id << " created." << std::endl;
	inputText = new sf::Text();
	inputText->setFont(StringFormat::GetDefaultFont());
	inputText->setCharacterSize(20);
	inputText->setFillColor(sf::Color::Green);
	inputText->setString(StringFormat::Format("Player %d Input", id + 1));
	inputText->setPosition(SCREEN_WIDTH/5*(id + 1), 100.f);
	inputText->setOrigin(inputText->getLocalBounds().width / 2, inputText->getLocalBounds().height / 2);

	allPlayers.push_back(this);
}

FlagGamePlayer::~FlagGamePlayer()
{
	delete inputText;
	inputText = nullptr;
	for (auto it = std::remove(allPlayers.begin(), allPlayers.end(), this); it != allPlayers.end(); ++it)
	{
		if (*it == this)
		{
			allPlayers.erase(it);
			return;
		}
	}
}

short FlagGamePlayer::GetID(void) const
{
	return id;
}

void FlagGamePlayer::UpdateAllPlayers(float _dt)
{
	for (FlagGamePlayer* player : allPlayers)
	{
		if (player) player->Update(_dt);
	}
}

void FlagGamePlayer::DrawAllPlayers(sf::RenderWindow& _renderWindow)
{
	for (FlagGamePlayer* player : allPlayers)
	{
		if (player) player->Draw(_renderWindow);
	}
}

void FlagGamePlayer::Update(float _dt)
{
	if(!inGame)
	{
		inputText->setFillColor(sf::Color::Red);
		return;
	}
	inputText->setFillColor(sf::Color::Green);
	for(GamePadBindList btn = GAMEPAD_A; btn < MAX_GAMEPAD_BIND_LIST; btn = (GamePadBindList)(btn + 1))
	{
		if(GetGamePadPressed(btn, id, true))
		{
			inputText->setString(StringFormat::Format("Player %d\nInput: %s", id + 1, GetGamePadButtonName(btn)));
			inputText->setOrigin(inputText->getLocalBounds().width / 2, inputText->getLocalBounds().height / 2);
			savedInput = btn;
			return;
		}
	}
}

void FlagGamePlayer::Draw(sf::RenderWindow& _renderWindow)
{
	_renderWindow.draw(*inputText);
}

bool FlagGamePlayer::IsInGame(void) const
{
	return inGame;
}

void FlagGamePlayer::SetInGame(bool _inGame)
{
	inGame = _inGame;
}

void FlagGame::NextRound(void)
{
	m_data->round++;
	m_data->countRoundText->setString(StringFormat::Format("Round: %d", m_data->round));
	m_data->countRoundText->setOrigin(m_data->countRoundText->getLocalBounds().width / 2, m_data->countRoundText->getLocalBounds().height / 2);
	m_data->timerBeforeNextRound = 5.0f;
	for(FlagGamePlayer* player : allPlayers)
	{
		if (player->IsInGame())
		{
			std::cout << "Player " << player->GetID() << " chose " << GetGamePadButtonName(player->GetSavedInput()) << ", required was " << GetGamePadButtonName((GamePadBindList)m_data->randomInputID) << std::endl;
			if (player->GetSavedInput() != (GamePadBindList)m_data->randomInputID)
			{
				EleminatePlayer(player->GetID());
			}
		}
	}
}

void EleminatePlayer(short _playerID)
{
	allPlayers[_playerID]->SetInGame(false);
	leftPlayer--;
}

bool SomeOneWon(void)
{
	for(FlagGamePlayer* player : allPlayers)
	{
		if (player->IsInGame())
		{
			return true;
		}
	}
	return false;
}

void FlagGame::ResetFlagGame(void)
{
	if(GetGamePadPressed(GAMEPAD_A,0,true))
	{
		for(FlagGamePlayer* player : allPlayers)
		{
			std::cout << "Resetting Player " << player->GetID() << std::endl;
			player->SetInGame(true);
		}
		leftPlayer = (short)allPlayers.size();
		m_data->timerBeforeNextRound = 5.0f;
		m_data->timerText->setString(StringFormat::Format("Time: %.2f", m_data->timerBeforeNextRound));
		m_data->round = 1;
		m_data->countRoundText->setString(StringFormat::Format("Round: %d", m_data->round));

	}
}

void FlagGame::NextInput(void)
{
	if (m_data->timerBeforeNextRound > 1.25f)
	{
		m_data->randomDelayforChangeInput = random::RandomFloat(0.1f, m_data->timerBeforeNextRound);
		m_data->randomInputID = (short)GetRandomGamePadInput();
		m_data->InputText->setString(StringFormat::Format("Input: %s", GetGamePadButtonName((GamePadBindList)m_data->randomInputID)));
		m_data->InputText->setOrigin(m_data->InputText->getLocalBounds().width / 2, m_data->InputText->getLocalBounds().height / 2);
	}
}

FlagGamePlayer& FlagGamePlayer::GetPlayerByID(short _id)
{
	for (FlagGamePlayer* player : allPlayers)
	{
		if (player->GetID() == _id)
		{
			return *player;
		}
	}
	throw std::runtime_error("Player with ID " + std::to_string(_id) + " not found.");
}

GamePadBindList GetRandomGamePadInput(void)
{
	short randInput = random::RandomInt(0, (short)MAX_GAMEPAD_BIND_LIST - 1);
	while(!WhiteListGamePadInput((GamePadBindList)randInput))
	{
		randInput = random::RandomInt(0, (short)MAX_GAMEPAD_BIND_LIST - 1);
	}
	return (GamePadBindList)randInput;
}

bool WhiteListGamePadInput(GamePadBindList _input)
{
	switch (_input)
	{
	case GAMEPAD_A: return true;
	case GAMEPAD_B: return true;
	case GAMEPAD_X: return true;
	case GAMEPAD_Y: return true;
	case GAMEPAD_LB: return true;
	case GAMEPAD_RB: return true;
	default: return false;
	}
}

short GetWinerID(void)
{
	for(FlagGamePlayer* player : allPlayers)
	{
		if (player->IsInGame())
		{
			return player->GetID();
		}
	}
	return -1;
}


GamePadBindList FlagGamePlayer::GetSavedInput(void) const
{
	return savedInput;
}