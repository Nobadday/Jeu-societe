#include "Board.hpp"

void BaseGame::Load(void)
{
	m_data = new SceneData;
	m_data->tile.InitTiled("Assets/Map/map.json");
	m_data->camera.Reset(((GameData*)this->m_keptData)->m_renderWindow->getDefaultView());

	MapLayer layer = m_data->tile.GetMapLayer("point");

	m_data->posCase = layer.GetObjects();

	m_data->players.push_back({});
	m_data->players.push_back({});
	m_data->players.push_back({});
	m_data->players.push_back({});

	m_data->state = START;

	for (int i = 0; i < m_data->players.size(); i++)
	{
		m_data->players[i].texture.loadFromFile("Assets/Images/Placeholder.png");
		m_data->players[i].sprite.setTexture(m_data->players[i].texture);
		m_data->players[i].boardPosition = m_data->posCase[0].GetPosition();
		m_data->players[i].currentCaseIndex = 0;
	}

	m_data->currentPlayerIndex = 0;

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

				// Calcul de la nouvelle position
				int newIndex = (m_data->players[m_data->currentPlayerIndex].currentCaseIndex + rando) % m_data->posCase.size();

				SetBoardState(DEPLACEMENT, newIndex);
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
				
				SetBoardState(DEPLACEMENT, newIndex);
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
	if (m_data->posCase[m_data->players[m_data->currentPlayerIndex].currentCaseIndex].GetType() == "Bonus" )
	{
		std::cout << "Landed on a Bonus case!" << std::endl;
		int rando = random::RandomInt(1, 3);

		int newIndex = (m_data->players[m_data->currentPlayerIndex].currentCaseIndex + rando) % m_data->posCase.size();

		SetBoardState(DEPLACEMENT_ACTION, newIndex);

	}
	else if (m_data->posCase[m_data->players[m_data->currentPlayerIndex].currentCaseIndex].GetType() == "Malus" )
	{
		std::cout << "Landed on a Malus case!" << std::endl;

		int rando = random::RandomInt(1, 3);

		int newIndex = (m_data->players[m_data->currentPlayerIndex].currentCaseIndex - rando) % m_data->posCase.size();

		SetBoardState(DEPLACEMENT_ACTION, newIndex);
	}
	else if (m_data->posCase[m_data->players[m_data->currentPlayerIndex].currentCaseIndex].GetType() == "Luck" )
	{
		std::cout << "Landed on a Luck case!" << std::endl;

		int rando = random::RandomInt(1, 3);

		if (rand() % 2 == 0)
		{
			rando = -rando;
		}

		int newIndex = (m_data->players[m_data->currentPlayerIndex].currentCaseIndex + rando) % m_data->posCase.size();

		SetBoardState(DEPLACEMENT_ACTION, newIndex);
	}
	else if (m_data->posCase[m_data->players[m_data->currentPlayerIndex].currentCaseIndex].GetType() == "Battle" )
	{
		std::cout << "Landed on a Battle case!" << std::endl;
	}
	else
	{
		m_data->state = START;
		m_data->currentPlayerIndex = (m_data->currentPlayerIndex + 1) % m_data->players.size();
	}
}

void BaseGame::SetBoardState(State _state ,int _newIndex = 0)
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
			sf::Vector2f startPos = m_data->players[m_data->currentPlayerIndex].boardPosition;
			sf::Vector2f endPos = m_data->posCase[_newIndex].GetPosition();

			m_data->animator.SetGoTo(startPos, endPos);
			m_data->animator.Restart();

			// Mise à jour de l'index
			m_data->players[m_data->currentPlayerIndex].currentCaseIndex = _newIndex;

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
	switch (m_data->state)
	{
		case START:
			// Attente du lancer de dé
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
			// Action de combat
			break;
		case CASE_ACTION_END:
			// Fin de l'action de la case
			m_data->state = START;
			m_data->currentPlayerIndex = (m_data->currentPlayerIndex + 1) % m_data->players.size();
			break;
		default:
			break;
	}
}
