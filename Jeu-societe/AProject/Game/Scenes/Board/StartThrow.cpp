#include "Board.hpp"

void BaseGame::SortStart(float _dt)
{
	int somme = 1;
	m_data->currentPlayerIndex = 0;


	for (const auto& player : m_data->players)
	{
		if (player.startRandom != 0)
			m_data->currentPlayerIndex = (m_data->currentPlayerIndex + 1) % m_data->players.size();

		somme *= player.startRandom;
	}

	if (somme != 0)
	{
		m_data->timeStart -= _dt;
		if (m_data->timeStart <= 0 )
		{
			for (size_t i = 0; i < m_data->players.size(); i++)
			{
				std::cout << "Player : " << m_data->players[i].startRandom << " ";
				std::cout << " id manette : " << m_gameData->m_playerDataList[i].m_joystickId << " " << std::endl;
			}

			// Tri des joueurs par ordre décroissant de jet de dé
			// Créer un vecteur d'indices
			std::vector<size_t> indices(m_data->players.size());
			std::iota(indices.begin(), indices.end(), 0);

			// Trier les indices par ordre décroissant de startRandom
			std::sort(indices.begin(), indices.end(),
				[this](size_t a, size_t b) { return m_data->players[a].startRandom > m_data->players[b].startRandom; });

			// Réorganiser m_data->players
			std::vector<Player> sortedPlayers(m_data->players.size());
			for (size_t i = 0; i < indices.size(); i++) {
				sortedPlayers[i] = m_data->players[indices[i]];
			}
			m_data->players = std::move(sortedPlayers);

			// CORRECTION : Réassigner les textures aux sprites après le déplacement
			for (auto& player : m_data->players) {
				player.sprite.setTexture(player.texture);
			}

			// Réorganiser m_gameData->m_playerDataList
			std::vector<PlayerData> sortedPlayerData(m_gameData->m_playerDataList.size());
			for (size_t i = 0; i < indices.size(); ++i) {
				sortedPlayerData[i] = m_gameData->m_playerDataList[indices[i]];
			}
			m_gameData->m_playerDataList = std::move(sortedPlayerData);


			for (size_t i = 0; i < m_data->players.size(); i++)
			{
				std::cout << "Player : " << m_data->players[i].startRandom << " ";
				std::cout << " id manette : " << m_gameData->m_playerDataList[i].m_joystickId << " " << std::endl;
			}


			m_data->state = PLAY;
			if (m_data->players[m_data->currentPlayerIndex].botAI != nullptr)
			{
				m_data->players[m_data->currentPlayerIndex].botAI->ResetForNewTurn();
			}
			ShowTextDisplay(m_data->players[m_data->currentPlayerIndex].playeur.getString() + " turn!\nPress A to roll the dice", 2.5f);
		}
	}
}