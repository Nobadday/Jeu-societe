#include "Board.hpp"

void BaseGame::UpdateCameraToShowAllPlayers()
{
	if (m_data->players.empty())
		return;

	// Calculer la boîte englobante de tous les joueurs
	sf::Vector2f minPos = m_data->players[0].boardPosition;
	sf::Vector2f maxPos = m_data->players[0].boardPosition;

	for (const auto& player : m_data->players)
	{
		minPos.x = std::min(minPos.x, player.boardPosition.x);
		minPos.y = std::min(minPos.y, player.boardPosition.y);
		maxPos.x = std::max(maxPos.x, player.boardPosition.x);
		maxPos.y = std::max(maxPos.y, player.boardPosition.y);
	}

	// Calculer le centre
	sf::Vector2f center = (minPos + maxPos) / 2.0f;

	sf::FloatRect viewRect = m_gameData->m_tile->GetMapLayer("Camera").GetObject(0).GetBounds();

	m_data->camera.SetLimitations(viewRect);

	m_data->camera.SetCenter(center);
	m_data->camera.SetZoom(1.0f);
}

void BaseGame::UpdateCameraFollowPlayer(float _deltaTime)
{
	if (m_data->players.empty() || m_data->state == START)
		return;

	// Position cible : le joueur actuel (sans offset)
	sf::Vector2f targetPos = m_data->players[m_data->currentPlayerIndex].boardPosition;

	// Position actuelle de la caméra
	sf::Vector2f currentPos = m_data->camera.GetCenter();

	// Interpolation fluide (lerp) pour un mouvement doux
	const float smoothSpeed = 3.0f;
	sf::Vector2f newPos;
	newPos.x = currentPos.x + (targetPos.x - currentPos.x) * smoothSpeed * _deltaTime;
	newPos.y = currentPos.y + (targetPos.y - currentPos.y) * smoothSpeed * _deltaTime;

	m_data->camera.SetCenter(newPos);
}