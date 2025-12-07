#include "Board.hpp"

void BaseGame::DrawIconePlayer(sf::RenderWindow& _renderWindow, int _i)
{
	m_gameData->m_renderWindow->ResetView();
	switch (m_data->players[_i].posIcone)
	{
	case UP_LEFT:
		if (_i != m_data->currentPlayerIndex)
		{
			m_data->icone.setPosition({ 72.5f, 305.f * 0.40f });
			m_data->iconeAura.setPosition({ 72.5f ,305.f * 0.41f });

		}
		else
		{
			m_data->icone.setPosition({ 72.5f ,  305.f * 0.50f });
			m_data->iconeAura.setPosition({ 72.5f ,  305.f * 0.51f });
			m_data->iconeState.setPosition({ 72.5f * 2.f ,  305.f * 0.50f });
		}
		break;
	case UP_RIGHT:
		if (_i != m_data->currentPlayerIndex)
		{
			m_data->icone.setPosition({ SCREEN_WIDTH - 72.5f ,  305.f * 0.40f });
			m_data->iconeAura.setPosition({ SCREEN_WIDTH - 72.5f , 305.f * 0.41f });


		}
		else
		{
			m_data->icone.setPosition({ SCREEN_WIDTH - 72.5f , 305.f * 0.50f });
			m_data->iconeAura.setPosition({ SCREEN_WIDTH - 72.5f ,  305.f * 0.51f });
			m_data->iconeState.setPosition({ SCREEN_WIDTH - 72.5f * 2.f , 305.f * 0.50f });
		}
		break;
	case DOWN_LEFT:
		if (_i != m_data->currentPlayerIndex)
		{
			m_data->icone.setPosition({ 72.5f  , SCREEN_HEIGHT - 10 });
			m_data->iconeAura.setPosition({ 72.5f  ,  SCREEN_HEIGHT - 8 });

		}
		else
		{
			m_data->icone.setPosition({ 72.5f  ,   SCREEN_HEIGHT - 10 });
			m_data->iconeAura.setPosition({ 72.5f  , SCREEN_HEIGHT - 8 });
			m_data->iconeState.setPosition({ 72.5f * 2.f ,  SCREEN_HEIGHT - 8 });
		}
		break;
	case DONW_RIGHT:
		if (_i != m_data->currentPlayerIndex)
		{
			m_data->icone.setPosition({ SCREEN_WIDTH - 72.5f  ,  SCREEN_HEIGHT - 10 });
			m_data->iconeAura.setPosition({ SCREEN_WIDTH - 72.5f  , SCREEN_HEIGHT - 8 });
			m_data->iconeState.setPosition({ SCREEN_WIDTH - 72.5f * 2.f , SCREEN_HEIGHT - 8 });
		}
		else
		{
			m_data->icone.setPosition({ SCREEN_WIDTH - 72.5f  ,  SCREEN_HEIGHT - 10 });
			m_data->iconeAura.setPosition({ SCREEN_WIDTH - 72.5f  , SCREEN_HEIGHT - 8 });
			m_data->iconeState.setPosition({ SCREEN_WIDTH - 72.5f * 2.f , SCREEN_HEIGHT - 8 });
		}
		break;
	default:
		break;
	}

	switch (m_gameData->m_playerDataList[_i].GetPlayerSkin())
	{
	case PlayerData::CHARACTER_1_1:
		m_data->icone.SetAnimation("Perso1-1");
		m_data->iconeAura.SetAnimation("Perso1-1");
		break;
	case PlayerData::CHARACTER_1_2:
		m_data->icone.SetAnimation("Perso1-2");
		m_data->iconeAura.SetAnimation("Perso1-2");
		break;
	case PlayerData::CHARACTER_2_1:
		m_data->icone.SetAnimation("Perso2-1");
		m_data->iconeAura.SetAnimation("Perso2-1");
		break;
	case PlayerData::CHARACTER_2_2:
		m_data->icone.SetAnimation("Perso2-2");
		m_data->iconeAura.SetAnimation("Perso2-2");
		break;
	case PlayerData::CHARACTER_3_1:
		m_data->icone.SetAnimation("Perso3-1");
		m_data->iconeAura.SetAnimation("Perso3-1");
		break;
	case PlayerData::CHARACTER_3_2:
		m_data->icone.SetAnimation("Perso3-2");
		m_data->iconeAura.SetAnimation("Perso3-2");
		break;
	case PlayerData::CHARACTER_4_1:
		m_data->icone.SetAnimation("Perso4-1");
		m_data->iconeAura.SetAnimation("Perso4-1");
		break;
	case PlayerData::CHARACTER_4_2:
		m_data->icone.SetAnimation("Perso4-2");
		m_data->iconeAura.SetAnimation("Perso4-2");
		break;
	default:
		break;
	}


	switch (m_data->players[m_data->currentPlayerIndex].state)
	{
	case INFEC:
		m_data->iconeState.SetTextureFrame("Infected");
		break;
	case IMMUN:
		m_data->iconeState.SetTextureFrame("Immune");
		break;
	case CONFUSED:
		m_data->iconeState.SetTextureFrame("Confusion");
		break;

	default:
		break;
	}


	if (_i != m_data->currentPlayerIndex)
	{
		m_data->icone.setColor({ 255,255,255,155 });
		m_data->icone.setScale({ 0.5f,0.5f });
		m_data->iconeAura.setColor({ 255,255,255,0 });
		m_data->iconeState.setColor({ 255,255,255,0 });
		m_data->iconeAura.setScale({ 0.55f,0.55f });
	}

	m_gameData->m_renderWindow->draw(m_data->iconeAura);
	m_gameData->m_renderWindow->draw(m_data->icone);
	if (_i == m_data->currentPlayerIndex)
	{
		if (m_data->players[m_data->currentPlayerIndex].state != NONE)
		{
			m_gameData->m_renderWindow->draw(m_data->iconeState);
		}
	}


	m_data->icone.setColor({ 255,255,255,255 });
	m_data->icone.setScale({ 0.75f,0.75f });
	m_data->iconeAura.setColor({ 255,255,255,255 });
	m_data->iconeAura.setScale({ 0.78f,0.78f });
	m_data->iconeState.setColor({ 255,255,255,255 });
	m_data->iconeState.setScale({ 0.2f,0.2f });
}
