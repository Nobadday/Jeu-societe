#include "Board.hpp"

bool BaseGame::HasPathChoice(int caseIndex)
{
	const MapObject& currentCase = m_data->posCase[caseIndex];
	if (currentCase.GetPropertyByName("type") == nullptr)
	{
		return false;
	}
	std::string caseType = currentCase.GetPropertyByName("type")->GetStringValue();
	return (caseType == "split");
}

std::vector<int> BaseGame::GetAvailablePaths(int caseIndex)
{
	std::vector<int> paths;
	const MapObject& currentCase = m_data->posCase[caseIndex];
	std::string caseType = "";
	if (currentCase.GetPropertyByName("type") != nullptr)
	{
		caseType = currentCase.GetPropertyByName("type")->GetStringValue();
	}

	if (caseType == "split")
	{
		// Récupérer les deux chemins possibles
		std::string path1Str = currentCase.GetPropertyByName("nextPath1")->GetStringValue();
		std::string path2Str = currentCase.GetPropertyByName("nextPath2")->GetStringValue();

		if (!path1Str.empty())
			paths.push_back(std::stoi(path1Str));
		if (!path2Str.empty())
			paths.push_back(std::stoi(path2Str));
	}
	else if (caseType == "merge")
	{
		// Une seule sortie après convergence
		std::string nextPathStr = currentCase.GetPropertyByName("nextPath")->GetStringValue();
		if (!nextPathStr.empty())
			paths.push_back(std::stoi(nextPathStr));
	}
	else
	{
		// Chemin normal : case suivante
		std::string nextPathStr = currentCase.GetPropertyByName("nextPath")->GetStringValue();
		if (!nextPathStr.empty())
		{
			paths.push_back(std::stoi(nextPathStr));
		}
		else
		{
			// Fallback : index suivant (modulo pour boucler)
			paths.push_back(mathp::ModuloPositiveI(caseIndex + 1, (int)m_data->posCase.size()));
		}
	}

	return paths;
}

std::vector<int> BaseGame::GetAvailablePathsBack(int caseIndex)
{
	std::vector<int> paths;
	const MapObject& currentCase = m_data->posCase[caseIndex];

	// Chemin normal : case suivante
	std::string nextPathStr = currentCase.GetPropertyByName("backPath")->GetStringValue();
	if (!nextPathStr.empty())
	{
		paths.push_back(std::stoi(nextPathStr));
	}
	else
	{
		// Fallback : index suivant (modulo pour boucler)
		paths.push_back(mathp::ModuloPositiveI(caseIndex + 1, (int)m_data->posCase.size()));
	}

	return paths;
}

int BaseGame::GetNextCaseIndex(int currentIndex, int pathChoice)
{
	const MapObject& currentCase = m_data->posCase[currentIndex];

	std::string caseType = "";
	if (currentCase.GetPropertyByName("type") != nullptr)
	{
		caseType = currentCase.GetPropertyByName("type")->GetStringValue();
	}

	if (caseType == "split")
	{
		// Choix entre deux chemins
		if (pathChoice == 0)
		{
			std::string path1Str = currentCase.GetPropertyByName("nextPath1")->GetStringValue();
			return !path1Str.empty() ? std::stoi(path1Str) : -1;
		}
		else
		{
			std::string path2Str = currentCase.GetPropertyByName("nextPath2")->GetStringValue();
			return !path2Str.empty() ? std::stoi(path2Str) : -1;
		}
	}
	else if (caseType == "merge")
	{
		// Une seule sortie
		std::string nextPathStr = currentCase.GetPropertyByName("nextPath")->GetStringValue();
		return !nextPathStr.empty() ? std::stoi(nextPathStr) : -1;
	}
	else
	{
		// Chemin normal
		std::string nextPathStr = currentCase.GetPropertyByName("nextPath")->GetStringValue();
		if (!nextPathStr.empty())
			return std::stoi(nextPathStr);
		else
			return mathp::ModuloPositiveI(currentIndex + 1, (int)m_data->posCase.size());
	}
}

void BaseGame::ProcessPathChoice(int choiceIndex)
{
	auto& player = m_data->players[m_data->currentPlayerIndex];
	const MapObject& currentCase = m_data->posCase[player.currentCaseIndex];

	// Récupérer le prochain index
	int nextIndex = GetNextCaseIndex(player.currentCaseIndex, choiceIndex);

	if (nextIndex == -1)
	{
		std::cout << "Erreur : chemin invalide!" << std::endl;
		SetBoardState(CASE_ACTION_END);
		return;
	}

	// Mettre à jour le pathId si on est sur une bifurcation
	std::string caseType = "";
	if (currentCase.GetPropertyByName("type") != nullptr)
	{
		caseType = currentCase.GetPropertyByName("type")->GetStringValue();
	}

	if (caseType == "split")
	{
		std::string pathIdKey = (choiceIndex == 0) ? "pathId1" : "pathId2";
		if (currentCase.GetPropertyByName(pathIdKey) != nullptr)
		{
			std::string pathIdStr = currentCase.GetPropertyByName(pathIdKey)->GetStringValue();
			player.currentPathId = !pathIdStr.empty() ? std::stoi(pathIdStr) : -1;

			std::cout << "Joueur a choisi le chemin " << player.currentPathId << std::endl;
		}
	}

	std::cout << "Mouvement restant avant animation : " << player.pendingMovement << std::endl;

	m_data->pathChoices.clear();
}

void BaseGame::ProcessBridgeRoll()
{
	int diceRoll = randmt::RandomInt(1, 6);
	auto& player = m_data->players[m_data->currentPlayerIndex];

	std::cout << "Lancer de dé pour le pont : " << diceRoll << std::endl;

	if (diceRoll > 3)
	{
		// Réussite : le joueur peut traverser le pont
		std::cout << "Traversée réussie !" << std::endl;
		player.waitingBridgeRoll = false;

		m_data->smokeOff = true;

		switch (m_gameData->m_playerDataList[m_data->currentPlayerIndex].GetPlayerSkin())
		{
		case PlayerData::CHARACTER_1_1:
			[[fallthrough]];
		case PlayerData::CHARACTER_1_2:
			m_gameData->m_playerDataList[m_data->currentPlayerIndex].SetPlayerSkin(PlayerData::CHARACTER_1_2);
			m_data->players[m_data->currentPlayerIndex].texture = *m_gameData->m_assetManager->GetAsset<TextureAnimated>("Perso1-2", AssetManager::AssetType::TEXTURE_ANIMATED);
			break;
		case PlayerData::CHARACTER_2_1:
			[[fallthrough]];
		case PlayerData::CHARACTER_2_2:
			m_gameData->m_playerDataList[m_data->currentPlayerIndex].SetPlayerSkin(PlayerData::CHARACTER_2_2);
			m_data->players[m_data->currentPlayerIndex].texture = *m_gameData->m_assetManager->GetAsset<TextureAnimated>("Perso2-2", AssetManager::AssetType::TEXTURE_ANIMATED);
			break;
		case PlayerData::CHARACTER_3_1:
			[[fallthrough]];
		case PlayerData::CHARACTER_3_2:
			m_gameData->m_playerDataList[m_data->currentPlayerIndex].SetPlayerSkin(PlayerData::CHARACTER_3_2);
			m_data->players[m_data->currentPlayerIndex].texture = *m_gameData->m_assetManager->GetAsset<TextureAnimated>("Perso3-2", AssetManager::AssetType::TEXTURE_ANIMATED);
			break;
		case PlayerData::CHARACTER_4_1:
			[[fallthrough]];
		case PlayerData::CHARACTER_4_2:
			m_gameData->m_playerDataList[m_data->currentPlayerIndex].SetPlayerSkin(PlayerData::CHARACTER_4_2);
			m_data->players[m_data->currentPlayerIndex].texture = *m_gameData->m_assetManager->GetAsset<TextureAnimated>("Perso4-2", AssetManager::AssetType::TEXTURE_ANIMATED);
			break;
		default:
			m_gameData->m_playerDataList[m_data->currentPlayerIndex].SetPlayerSkin(PlayerData::CHARACTER_1_2);
			m_data->players[m_data->currentPlayerIndex].texture = *m_gameData->m_assetManager->GetAsset<TextureAnimated>("Perso1-1", AssetManager::AssetType::TEXTURE_ANIMATED);
			break;
		}

		std::cout << "Mouvement restant après traversée : " << player.pendingMovement << std::endl;

		// Si le joueur a encore du mouvement, continuer
		if (player.pendingMovement > 0)
		{
			SetBoardState(DEPLACEMENT_BRIGE);
		}
		else
		{
			// Plus de mouvement : terminer sur la case du pont
			SetBoardState(CASE_ACTION);
		}
	}
	else
	{
		// Échec : le joueur reste bloqué et perd son tour
		std::cout << "Échec ! Vous ne pouvez pas traverser le pont." << std::endl;
		player.waitingBridgeRoll = false;
		player.pendingMovement = 0;  // Annuler le mouvement restant

		SetBoardState(CASE_ACTION);
	}
}

void BaseGame::ProcessFinRoll()
{
	int diceRoll = randmt::RandomInt(1, 6);
	auto& player = m_data->players[m_data->currentPlayerIndex];

	std::cout << "Lancer de dé pour la ligne d'arrivée : " << diceRoll << std::endl;

	if (diceRoll > 4)
	{
		// Réussite : le joueur a gagné !
		std::cout << "VICTOIRE ! Le joueur " << m_data->currentPlayerIndex << " a gagné !" << std::endl;
		player.waitingBridgeRoll = false;

		// Préparer la liste des joueurs triés par position X décroissante
		std::vector<std::pair<int, float>> playerPositions;
		for (int i = 0; i < m_data->players.size(); i++)
		{
			playerPositions.push_back({ i, m_data->players[i].boardPosition.x });
		}

		// Trier par position X décroissante (les plus avancés en premier)
		std::sort(playerPositions.begin(), playerPositions.end(),
			[](const std::pair<int, float>& a, const std::pair<int, float>& b) {
				return a.second > b.second;
			});

		// Remplir m_winIndex avec le gagnant en premier, puis les autres
		m_gameData->m_winIndex.clear();

		// Le gagnant actuel en premier
		m_gameData->m_winIndex.push_back(m_data->currentPlayerIndex);

		// Ajouter les autres joueurs triés par position X
		for (const auto& playerPos : playerPositions)
		{
			if (playerPos.first != m_data->currentPlayerIndex)
			{
				m_gameData->m_winIndex.push_back(playerPos.first);
			}
		}

		// Afficher l'ordre final (debug)
		std::cout << "Ordre final pour le podium : ";
		for (int idx : m_gameData->m_winIndex)
		{
			std::cout << "P" << idx << " ";
		}
		std::cout << std::endl;

		// Transition vers le podium
		//ChangeScene("Podium", false);

		SetBoardState(END); // Bloquer le jeu en attendant le changement de scène
	}
	else
	{
		// Échec : le joueur reste bloqué et perd son tour
		std::cout << "Échec ! Vous ne pouvez pas franchir la ligne d'arrivée." << std::endl;
		player.waitingBridgeRoll = false;
		player.pendingMovement = 0;

		SetBoardState(CASE_ACTION);
	}
}