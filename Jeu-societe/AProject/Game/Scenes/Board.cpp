#include "Board.hpp"

void BaseGame::Load(void)
{
	m_data = new SceneData;
	m_data->tile.InitTiled("Assets/Map/map.json");
	m_data->camera.Reset(((GameData*)this->m_keptData)->m_renderWindow->getDefaultView());

	MapLayer layer = m_data->tile.GetMapLayer("point");

	m_data->posCase = layer.GetObjects();

	GameData* data = (GameData*)this->m_keptData;

	//m_data->players.resize(data->m_playerDataList.size());*


	m_data->players.resize(data->m_playerDataList.size());

	m_data->state = START;

	for (int i = 0; i < m_data->players.size(); i++)
	{
		m_data->players[i].texture.loadFromFile("Assets/Images/Placeholder.png");
		m_data->players[i].sprite.setTexture(m_data->players[i].texture);
		m_data->players[i].boardPosition = m_data->posCase[0].GetPosition();
		m_data->players[i].currentCaseIndex = 0;
		m_data->players[i].startRandom = 0;
	}

	m_data->currentPlayerIndex = 0;

	m_data->camera.SetCenter(m_data->players[m_data->currentPlayerIndex].boardPosition);

	// Configuration de l'animator
	m_data->animator.Modify(1.0f, 60.0f, false, 1.0f); // 1 seconde de durée, 60 FPS, pas de loop
	m_data->animator.SetAnimationEasing(anim::Animator::GOTO, anim::Easing::INOUTSINE);

	//m_data->animator.Restart();
	m_data->animator.End();
}

void BaseGame::Unload(void)
{
	delete this->m_data;
	this->m_data = NULL;
}

void BaseGame::PollEvent(sf::Event& _event)
{
	if (_event.type == sf::Event::JoystickButtonPressed)
	{
		if (_event.joystickButton.button == 1)
		{
			// Empêcher un nouveau lancer si une animation est en cours
			if (m_data->animator.IsFinished())
			{
				int rando = random::RandomInt(1, 6);
				std::cout << "Roll Dice: " << rando << std::endl;

				if (m_data->state != START)
				{
					// Calcul de la nouvelle position
					int newIndex = (m_data->players[m_data->currentPlayerIndex].currentCaseIndex + rando) % m_data->posCase.size();
					SetBoardState(DEPLACEMENT, newIndex);
				}
				else
				{
					m_data->players[m_data->currentPlayerIndex].startRandom = rando;
				}
			}
		}
	}

	if (_event.type == sf::Event::KeyPressed)
	{
		if (_event.key.code == sf::Keyboard::Space)
		{
			// Empêcher un nouveau lancer si une animation est en cours
			if (m_data->animator.IsFinished())
			{
				int rando = random::RandomInt(1, 6);
				std::cout << "Roll Dice: " << rando << std::endl;

				// Calcul de la nouvelle position
				int newIndex = (m_data->players[m_data->currentPlayerIndex].currentCaseIndex + rando) % m_data->posCase.size();

				if (m_data->state != START)
				{
					// Calcul de la nouvelle position
					int newIndex = (m_data->players[m_data->currentPlayerIndex].currentCaseIndex + rando) % m_data->posCase.size();
					SetBoardState(DEPLACEMENT, newIndex);
				}
				else
				{
					m_data->players[m_data->currentPlayerIndex].startRandom = rando;
				}
			}
		}
	}
}

void BaseGame::Update(float _deltaTime)
{
	sf::Vector2f movement(0.f, 0.f);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
	{
		movement.y -= 100.f * _deltaTime;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		movement.y += 100.f * _deltaTime;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
	{
		movement.x -= 100.f * _deltaTime;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		movement.x += 100.f * _deltaTime;
	}

	// Mise à jour de l'animator
	m_data->animator.Update(_deltaTime);

	// Récupération de la position interpolée et mise à jour du joueur
	BoardStateUpdate();

	//m_data->camera.SetCenter(m_data->players[m_data->currentPlayerIndex].sprite.getPosition());

	std::cout << std::endl;

	std::cout << "State: " << m_data->state << std::endl;

	m_data->camera.Move(movement);
}

void BaseGame::Draw(sf::RenderWindow& _renderWindow)
{
	const sf::View& referenceView = m_data->camera.GetView();
	_renderWindow.setView(referenceView);
	m_data->tile.DrawMapLayers(_renderWindow, referenceView.getCenter());

	for (auto& player : m_data->players)
	{
		float screenX = player.boardPosition.x - referenceView.getCenter().x;
		float screenY = player.boardPosition.y - referenceView.getCenter().y;

		player.sprite.setPosition({ screenX, screenY });
		_renderWindow.draw(player.sprite);
	}
}

void BaseGame::CaseAction()
{
	if (m_data->posCase[m_data->players[m_data->currentPlayerIndex].currentCaseIndex].GetType() == "Bonus")
	{
		std::cout << "Landed on a Bonus case!" << std::endl;
		int rando = random::RandomInt(1, 3);

		int newIndex = (m_data->players[m_data->currentPlayerIndex].currentCaseIndex + rando) % m_data->posCase.size();

		SetBoardState(DEPLACEMENT_ACTION, newIndex);

	}
	else if (m_data->posCase[m_data->players[m_data->currentPlayerIndex].currentCaseIndex].GetType() == "Malus")
	{
		std::cout << "Landed on a Malus case!" << std::endl;

		int rando = random::RandomInt(1, 3);

		int newIndex = (m_data->players[m_data->currentPlayerIndex].currentCaseIndex - rando) % m_data->posCase.size();

		SetBoardState(DEPLACEMENT_ACTION, newIndex);
	}
	else if (m_data->posCase[m_data->players[m_data->currentPlayerIndex].currentCaseIndex].GetType() == "Luck")
	{
		std::cout << "Landed on a Luck case!" << std::endl;

		int rando = random::RandomInt(1, 3);

		if (random::RandomInt(0,2) == 0)
		{
			rando = -rando;
		}

		int newIndex = (m_data->players[m_data->currentPlayerIndex].currentCaseIndex + rando) % m_data->posCase.size();

		SetBoardState(DEPLACEMENT_ACTION, newIndex);
	}
	else if (m_data->posCase[m_data->players[m_data->currentPlayerIndex].currentCaseIndex].GetType() == "Battle")
	{
		std::cout << "Landed on a Battle case!" << std::endl;

		SetBoardState(BATTLE_ACTION, 0);
	}
	else
	{
		if (OnSameCase() != -1)
		{
			SetBoardState(DUEL, 0);
		}
		else
		{
			SetBoardState(PLAY, 0);
			m_data->currentPlayerIndex = (m_data->currentPlayerIndex + 1) % m_data->players.size();
		}
	}
}

void BaseGame::SetBoardState(State _state, int _newIndex)
{
	m_data->state = _state;
	switch (m_data->state)
	{
	case START:
		// Initialisation du tour
		break;
	case DEPLACEMENT:
		[[fallthrough]];
	case DEPLACEMENT_ACTION:
	{
		sf::Vector2f startPos = m_data->players[m_data->currentPlayerIndex].boardPosition;
		sf::Vector2f endPos = m_data->posCase[_newIndex].GetPosition();

		m_data->animator.SetGoTo(startPos, endPos);
		m_data->animator.Restart();

		// Mise à jour de l'index
		m_data->players[m_data->currentPlayerIndex].currentCaseIndex = _newIndex;
	}
	break;
	case CASE_ACTION:
		// Action de la case
		break;
	case CASE_ACTION_END:
		// Fin de l'action de la case
		break;
	default:
		break;
	}
}

void BaseGame::BoardStateUpdate()
{
	GameData* data = (GameData*)this->m_keptData;

	switch (m_data->state)
	{
		case START:
		{
			int somme = 1;

			m_data->currentPlayerIndex = 0;

			for (const auto& player : m_data->players)
			{
				if (player.startRandom != 0)
				{
					m_data->currentPlayerIndex = (m_data->currentPlayerIndex + 1) % m_data->players.size();
				}

				std::cout << "Player start roll: " << player.startRandom << std::endl;

				somme *= player.startRandom;
			}

			if (somme != 0)
			{
				std::sort(m_data->players.begin(), m_data->players.end(),
					[](const BaseGame::Player& a, const BaseGame::Player& b)
					{
						return a.startRandom > b.startRandom;
					});
				m_data->state = PLAY;
			}
		}
		break;
		case DEPLACEMENT:
			// Déplacement en cours
			m_data->players[m_data->currentPlayerIndex].boardPosition = m_data->animator.GetGoTo();
			if (m_data->animator.IsFinished())
			{
				m_data->state = CASE_ACTION;
			}

			break;
		case DEPLACEMENT_ACTION:
			// Déplacement en cours
			m_data->players[m_data->currentPlayerIndex].boardPosition = m_data->animator.GetGoTo();
			if (m_data->animator.IsFinished())
			{
				m_data->state = CASE_ACTION_END;
			}
			break;
		case CASE_ACTION:
			// Action de la case
			CaseAction();
			break;
		case BATTLE_ACTION:
			data->InitMiniGamePlayer();
			for (int i = 0; i < m_data->players.size(); i++)
			{
				data->AddPlayerPlaying(i);
			}
			ChangeScene("rockPaperSizor", true);
			break;
		case DUEL:
			data->InitMiniGamePlayer();
			data->AddPlayerPlaying(m_data->currentPlayerIndex);
			data->AddPlayerPlaying(OnSameCase());
			ChangeScene("rockPaperSizor", true);
			break;
		case CASE_ACTION_END:
			// Fin de l'action de la case
			m_data->state = PLAY;
			m_data->currentPlayerIndex = (m_data->currentPlayerIndex + 1) % m_data->players.size();
			break;
		default:
			break;
	}
}

int BaseGame::OnSameCase()
{
	int Index = 0;

	while (Index < m_data->players.size())
	{
		if (m_data->players[Index].currentCaseIndex == m_data->players[m_data->currentPlayerIndex].currentCaseIndex && Index != m_data->currentPlayerIndex)
		{
			return Index;
		}
		Index++;
	}

	return -1;
}
