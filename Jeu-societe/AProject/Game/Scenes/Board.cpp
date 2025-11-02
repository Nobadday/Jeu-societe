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

	for (int i = 0; i < m_data->players.size(); i++)
	{
		m_data->players[i].texture.loadFromFile("Assets/Images/Placeholder.png");
		m_data->players[i].sprite.setTexture(m_data->players[i].texture);
		m_data->players[i].boardPosition = m_data->posCase[0].GetPosition();
		m_data->players[i].currentCaseIndex = 0;
		m_data->players[i].isActive = false;
		m_data->players[i].isActiveBonus = false;

		std::cout << "Player " << i << " position: " << m_data->players[i].boardPosition.x << ", " << m_data->players[i].boardPosition.y << std::endl;
	}

	m_data->currentPlayerIndex = 0;

	// Configuration de l'animator
	m_data->animator.Modify(1.0f, 60.0f, false, 1.0f); // 1 seconde de durée, 60 FPS, pas de loop
	m_data->animator.SetAnimationEasing(anim::Animator::GOTO, anim::Easing::INOUTSINE);
}

void BaseGame::Unload(void)
{
	delete this->m_data;
	this->m_data = NULL;
}

void BaseGame::PollEvent(sf::Event& _event)
{
	if (_event.type == sf::Event::KeyPressed)
	{
		if (_event.key.code == sf::Keyboard::Space)
		{
			// Empêcher un nouveau lancer si une animation est en cours
			if (m_data->animator.IsFinished())
			{
				int rando = 1 + rand() % 6;
				std::cout << "Roll Dice: " << rando << std::endl;
				
				// Calcul de la nouvelle position
				int newIndex = (m_data->players[m_data->currentPlayerIndex].currentCaseIndex + rando) % m_data->posCase.size();
				sf::Vector2f startPos = m_data->players[m_data->currentPlayerIndex].boardPosition;
				sf::Vector2f endPos = m_data->posCase[newIndex].GetPosition();
				
				// Configuration de l'animation
				m_data->animator.SetGoTo(startPos, endPos);
				m_data->animator.Restart();
				
				// Mise à jour de l'index
				m_data->players[m_data->currentPlayerIndex].currentCaseIndex = newIndex;
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
	if (!m_data->animator.IsFinished())
	{
		m_data->players[m_data->currentPlayerIndex].boardPosition = m_data->animator.GetGoTo();
		m_data->players[m_data->currentPlayerIndex].isActive = true;
	}
	else if(m_data->players[m_data->currentPlayerIndex].isActive)
	{
		if(m_data->posCase[m_data->players[m_data->currentPlayerIndex].currentCaseIndex].GetType() == "Bonus" && !m_data->players[m_data->currentPlayerIndex].isActiveBonus) 
		{
			std::cout << "Landed on a Bonus case!" << std::endl;
			int rando = 1 + rand() % 3;


			int newIndex = (m_data->players[m_data->currentPlayerIndex].currentCaseIndex + rando) % m_data->posCase.size();
			sf::Vector2f startPos = m_data->players[m_data->currentPlayerIndex].boardPosition;
			sf::Vector2f endPos = m_data->posCase[newIndex].GetPosition();
			m_data->animator.SetGoTo(startPos, endPos);
			m_data->animator.Restart();

			// Mise à jour de l'index
			m_data->players[m_data->currentPlayerIndex].currentCaseIndex = newIndex;
			m_data->players[m_data->currentPlayerIndex].isActiveBonus = true;
		}
		else if (m_data->posCase[m_data->players[m_data->currentPlayerIndex].currentCaseIndex].GetType() == "Malus" && !m_data->players[m_data->currentPlayerIndex].isActiveBonus)
		{
			std::cout << "Landed on a Malus case!" << std::endl;
			int rando = 1 + rand() % 3;
			int newIndex = (m_data->players[m_data->currentPlayerIndex].currentCaseIndex - rando) % m_data->posCase.size();
			sf::Vector2f startPos = m_data->players[m_data->currentPlayerIndex].boardPosition;
			sf::Vector2f endPos = m_data->posCase[newIndex].GetPosition();
			m_data->animator.SetGoTo(startPos, endPos);
			m_data->animator.Restart();

			// Mise à jour de l'index
			m_data->players[m_data->currentPlayerIndex].currentCaseIndex = newIndex;
			m_data->players[m_data->currentPlayerIndex].isActiveBonus = true;
		}
		else if (m_data->posCase[m_data->players[m_data->currentPlayerIndex].currentCaseIndex].GetType() == "Luck" && !m_data->players[m_data->currentPlayerIndex].isActiveBonus)
		{
			std::cout << "Landed on a Luck case!" << std::endl;

			int rando = 1 + rand() % 3;

			if (rand() % 2 == 0)
			{
				rando = -rando;
			}

			int newIndex = (m_data->players[m_data->currentPlayerIndex].currentCaseIndex + rando) % m_data->posCase.size();
			sf::Vector2f startPos = m_data->players[m_data->currentPlayerIndex].boardPosition;
			sf::Vector2f endPos = m_data->posCase[newIndex].GetPosition();
			m_data->animator.SetGoTo(startPos, endPos);
			m_data->animator.Restart();

			// Mise à jour de l'index
			m_data->players[m_data->currentPlayerIndex].currentCaseIndex = newIndex;
			m_data->players[m_data->currentPlayerIndex].isActiveBonus = true;
		}
		else
		{
			m_data->players[m_data->currentPlayerIndex].isActive = false;
			m_data->players[m_data->currentPlayerIndex].isActiveBonus = false;
			m_data->currentPlayerIndex = (m_data->currentPlayerIndex + 1) % m_data->players.size();
		}
	
	}


	for (int i = 0; i < m_data->players.size(); i++)
	{
		std::cout << "Player " << i << " position: " << m_data->players[i].boardPosition.x << ", " << m_data->players[i].boardPosition.y << std::endl;
	}

	//std::cout << m_data->posCase[0].GetType() << std::endl;

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
		float screenY = player.boardPosition.y- referenceView.getCenter().y;

		player.sprite.setPosition({screenX,screenY});
		_renderWindow.draw(player.sprite);
	}
}

