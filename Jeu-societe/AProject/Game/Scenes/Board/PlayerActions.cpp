#include "Board.hpp"

void BaseGame::SwapPlayers(int _swapIndex)
{
	std::cout << "Swap de place avec : Player " << _swapIndex << std::endl;

	auto& player1 = m_data->players[m_data->currentPlayerIndex];
	auto& player2 = m_data->players[_swapIndex];

	std::string player1Name = player1.playeur.getString();
	std::string player2Name = player2.playeur.getString();

	std::cout << " swap :" << player1Name << " et " << player2Name << std::endl;

	std::swap(m_data->players[m_data->currentPlayerIndex].currentCaseIndex, m_data->players[_swapIndex].currentCaseIndex);
	std::swap(m_data->players[m_data->currentPlayerIndex].boardPosition, m_data->players[_swapIndex].boardPosition);

	CreateSmokeEffectForSwap(player1);
	CreateSmokeEffectForSwap(player2);

	SetBoardState(CASE_ACTION_END);
}

void BaseGame::CaseAvancePlus(int _rando)
{
	auto& player = m_data->players[m_data->currentPlayerIndex];

	std::cout << "Avance de : " << _rando << "!" << std::endl;

	// Initialiser le mouvement restant pour le déplacement case par case
	player.sprite.setScale({ 1.f, 1.f });
	player.pendingMovement = _rando;

	SetBoardState(DEPLACEMENT_ACTION_2);
}

void BaseGame::ImuniteMalus()
{
	std::cout << "Imunite au Malus" << std::endl;

	m_data->players[m_data->currentPlayerIndex].state = StatePlayer::IMMUN;
	SetBoardState(CASE_ACTION_END);
}

void BaseGame::CaseMoins(int _rando)
{
	auto& player = m_data->players[m_data->currentPlayerIndex];

	std::cout << "Recule de : " << _rando << "!" << std::endl;

	// Initialiser le mouvement restant pour le déplacement case par case
	player.pendingMovement = _rando;

	// Retourner le sprite pour indiquer le recul
	player.sprite.setScale({ -1.f, 1.f });

	SetBoardState(DEPLACEMENT_ACTION_BACK);
}

void BaseGame::Infecte()
{
	std::cout << "Infectes pas de bonus " << std::endl;
	m_data->players[m_data->currentPlayerIndex].state = StatePlayer::INFEC;
	SetBoardState(CASE_ACTION_END);
}

void BaseGame::PaseTour()
{
	std::cout << "Passe son tour" << std::endl;
	m_data->players[m_data->currentPlayerIndex].state = StatePlayer::CANT_PLAY;
	SetBoardState(CASE_ACTION_END);
}