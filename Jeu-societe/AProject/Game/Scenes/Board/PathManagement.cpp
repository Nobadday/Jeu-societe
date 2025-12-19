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


// Méthode pour traiter le lancer de dé sur le pont
void BaseGame::ProcessBridgeRoll()
{
	auto& player = m_data->players[m_data->currentPlayerIndex];

	int rando = randmt::RandomInt(1, 6);
	std::cout << "Bridge Roll: " << rando << std::endl;

	m_data->diceResult = rando;
	player.waitingBridgeRoll = false;

	// Lancer l'animation vidéo du dé
	if (rando >= 1 && rando <= 6)
	{
		m_data->currentDiceVideo = m_data->diceVideos[rando - 1];
		m_data->currentDiceVideo->play();
		m_data->diceAnimationPlaying = true;

		// Nouveau état pour l'animation du dé sur le pont
		m_data->state = DICE_ANIMATION_BRIDGE;
	}
}

void BaseGame::ProcessBridgeRollD()
{
	auto& player = m_data->players[m_data->currentPlayerIndex];

	int rando =  6 ;
	std::cout << "Bridge Roll: " << rando << std::endl;

	m_data->diceResult = rando;
	player.waitingBridgeRoll = false;

	// Lancer l'animation vidéo du dé
	if (rando >= 1 && rando <= 6)
	{
		m_data->currentDiceVideo = m_data->diceVideos[rando - 1];
		m_data->currentDiceVideo->play();
		m_data->diceAnimationPlaying = true;

		// Nouveau état pour l'animation du dé sur le pont
		m_data->state = DICE_ANIMATION_BRIDGE;
	}
}


// Méthode pour traiter le lancer de dé sur la ligne d'arrivée
void BaseGame::ProcessFinRoll()
{
	auto& player = m_data->players[m_data->currentPlayerIndex];

	int rando = randmt::RandomInt(1, 6);
	std::cout << "Fin Roll: " << rando << std::endl;

	m_data->diceResult = rando;
	player.waitingBridgeRoll = false;

	// Lancer l'animation vidéo du dé
	if (rando >= 1 && rando <= 6)
	{
		m_data->currentDiceVideo = m_data->diceVideos[rando - 1];
		m_data->currentDiceVideo->play();
		m_data->diceAnimationPlaying = true;

		// Nouveau état pour l'animation du dé sur la ligne d'arrivée
		m_data->state = DICE_ANIMATION_END;
	}
}

void BaseGame::ProcessFinRollD()
{
	auto& player = m_data->players[m_data->currentPlayerIndex];

	int rando = randmt::RandomInt(1, 6);
	std::cout << "Fin Roll: " << rando << std::endl;

	m_data->diceResult = rando;
	player.waitingBridgeRoll = false;

	// Lancer l'animation vidéo du dé
	if (rando >= 1 && rando <= 6)
	{
		m_data->currentDiceVideo = m_data->diceVideos[rando - 1];
		m_data->currentDiceVideo->play();
		m_data->diceAnimationPlaying = true;

		// Nouveau état pour l'animation du dé sur la ligne d'arrivée
		m_data->state = DICE_ANIMATION_END;
	}
}