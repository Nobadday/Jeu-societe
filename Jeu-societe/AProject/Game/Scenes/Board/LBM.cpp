#include "Board.hpp"

void BaseGame::UpdateLBM(float _dt)
{
	if (m_data->HudLBM.state != NONELBM)
	{
		m_data->timeLBM -= _dt;

		LBMDisplayUpdate(_dt);

		if (m_data->timeLBM <= 0 && m_data->HudLBM.active)
		{
			if (m_data->HudLBM.chosse == "CasePlus")
			{
				CaseAvancePlus(m_data->HudLBM.rando);
			}
			else if (m_data->HudLBM.chosse == "Immunite")
			{
				ImuniteMalus();
			}
			else if (m_data->HudLBM.chosse == "CaseMoin")
			{
				CaseMoins(m_data->HudLBM.rando);
			}
			else if (m_data->HudLBM.chosse == "Infection")
			{
				Infecte();
			}
			else if (m_data->HudLBM.chosse == "PaseTour")
			{
				PaseTour();
			}
			else if (m_data->HudLBM.chosse == "Swap")
			{
				SwapPlayers(m_data->HudLBM.swap);
			}
			else if (m_data->HudLBM.chosse == "Confus")
			{
				SetBoardState(CASE_ACTION_END);
			}
			else if (m_data->HudLBM.chosse == "ConfusSkip")
			{
				SetBoardState(CASE_ACTION_END);
			}

			m_data->HudLBM.state = NONELBM;
			m_data->timeLBM = TIME_LBM_DISPLAY;
			m_data->HudLBM.active = false;
			m_data->HudLBM.text.setString("");
			m_data->HudLBM.rando = 0;
			m_data->HudLBM.swap = 0;
			m_data->HudLBM.name = "";
			m_data->HudLBM.chosse = "";
		}
	}
}

void BaseGame::LBMDisplayUpdate(float _dt)
{
	if (m_data->timeLBM <= TIME_LBM_DISPLAY * 0.75f && !m_data->HudLBM.active)
	{
		m_data->HudLBM.sprite.SetAnimation(m_data->HudLBM.name + "Face");
		m_data->HudLBM.sprite.Update(_dt);
		sf::Vector2u size = m_data->HudLBM.sprite.getTexture()->getSize();

		sf::Vector2f size2 = m_gameData->m_renderWindow->getView().getCenter();

		m_data->HudLBM.sprite.setScale({ 0.25f , 0.25f });
		m_data->HudLBM.sprite.setOrigin({ 0.5,0.5 });
		m_data->HudLBM.sprite.setPosition({ SCREEN_WIDTH / 2  , SCREEN_WIDTH / 10 });

		if (m_data->HudLBM.chosse == "CasePlus")
		{
			m_data->HudLBM.rando = randmt::RandomInt(1, 3);
			m_data->HudLBM.text.setString("Move Forward " + std::to_string(m_data->HudLBM.rando) + " Space" + (m_data->HudLBM.rando > 1 ? "s" : ""));
			m_data->HudLBM.text.setPosition({ SCREEN_WIDTH / 2  , SCREEN_WIDTH / 10 });
			m_data->HudLBM.active = true;
		}
		else if (m_data->HudLBM.chosse == "Immunite")
		{
			m_data->HudLBM.text.setString("Immune to\nPenalties for 2 Turns");
			m_data->HudLBM.text.setPosition({ SCREEN_WIDTH / 2  , SCREEN_WIDTH / 10 });
			m_data->HudLBM.active = true;
		}
		else if (m_data->HudLBM.chosse == "Swap")
		{
			m_data->HudLBM.swap = randmt::RandomInt(0, (int)m_data->players.size() - 1);
			while (m_data->HudLBM.swap == m_data->currentPlayerIndex)
			{
				m_data->HudLBM.swap = randmt::RandomInt(0, (int)m_data->players.size() - 1);
			}
			m_data->HudLBM.text.setString("Swap Position with " + m_data->players[m_data->HudLBM.swap].playeur.getString());
			m_data->HudLBM.text.setPosition({ SCREEN_WIDTH / 2  , SCREEN_WIDTH / 10 });
			m_data->HudLBM.active = true;
		}
		else if (m_data->HudLBM.chosse == "CaseMoin")
		{
			m_data->HudLBM.rando = randmt::RandomInt(1, 3);
			m_data->HudLBM.text.setString("Move Backward " + std::to_string(m_data->HudLBM.rando) + " Space" + (m_data->HudLBM.rando > 1 ? "s" : ""));
			m_data->HudLBM.text.setPosition({ SCREEN_WIDTH / 2  , SCREEN_WIDTH / 10 });
			m_data->HudLBM.active = true;
		}
		else if (m_data->HudLBM.chosse == "Infection")
		{
			m_data->HudLBM.text.setString("Infected! No Bonuses for 2 Turns");
			m_data->HudLBM.text.setPosition({ SCREEN_WIDTH / 2  , SCREEN_WIDTH / 10 });
			m_data->HudLBM.active = true;
		}
		else if (m_data->HudLBM.chosse == "PaseTour")
		{
			m_data->HudLBM.text.setString("Skip Your Turn Next Round");
			m_data->HudLBM.text.setPosition({ SCREEN_WIDTH / 2  , SCREEN_WIDTH / 10 });
			m_data->HudLBM.active = true;
		}
		else if (m_data->HudLBM.chosse == "Confus")
		{
			m_data->HudLBM.text.setString("Confused! Next Turn, Your Dice Roll Makes You Move Backward");
			m_data->HudLBM.text.setPosition({ SCREEN_WIDTH / 2  , SCREEN_WIDTH / 10 });
			m_data->HudLBM.active = true;
		}
		else if (m_data->HudLBM.chosse == "ConfusSkip")
		{
			m_data->HudLBM.text.setString("Confusion Avoided! You're Lucky");
		 m_data->HudLBM.text.setPosition({ SCREEN_WIDTH / 2  , SCREEN_WIDTH / 10 });
			m_data->HudLBM.active = true;
		}

		m_data->HudLBM.active = true;
		m_data->HudLBM.text.setOrigin({ 0.5f , 0.5f });
	}
}

void BaseGame::DrawLBM(sf::RenderWindow& _renderWindow)
{
	if (m_data->HudLBM.state != NONELBM)
	{
		m_gameData->m_renderWindow->ResetView();
		m_gameData->m_renderWindow->draw(m_data->HudLBM.sprite);
		m_gameData->m_renderWindow->draw(m_data->HudLBM.text);
	}
}

void BaseGame::BonusMalusLuck(bool _malus)
{
	int chance = randmt::RandomInt(0, 100);

	if (_malus)
	{
		Malus(chance);
	}
	else
	{
		Bonus(chance);
	}
}

void BaseGame::Bonus(int _chance)
{
	if (_chance <= 50)
	{
		m_data->HudLBM.chosse = "CasePlus";
		SetBoardState(STATE);
	}
	else if (_chance <= 80)
	{
		m_data->HudLBM.chosse = "Immunite";
		SetBoardState(STATE);
	}
	else if (_chance <= 1000)
	{
		m_data->HudLBM.chosse = "Swap";
		SetBoardState(STATE);
	}
}

void BaseGame::Malus(int _chance)
{
	if (_chance <= 35)
	{
		m_data->HudLBM.chosse = "CaseMoin";
		SetBoardState(STATE);
	}
	else if (_chance <= 60)
	{
		if (randmt::Chance(0.5f))
		{
			m_data->HudLBM.chosse = "PaseTour";
		}
		else
		{
			m_data->HudLBM.chosse = "Infection";
		}
		SetBoardState(STATE);
	}
	else if (_chance <= 70)
	{
		m_data->HudLBM.chosse = "Swap";
		SetBoardState(STATE);
	}
	else if (_chance <= 1000)
	{
		if (randmt::Chance(0.75f))
		{
			m_data->players[m_data->currentPlayerIndex].state = StatePlayer::CONFUSED;
			m_data->HudLBM.chosse = "Confus";
		}
		else
		{
			m_data->HudLBM.chosse = "ConfusSkip";
		}
		SetBoardState(STATE);
	}
}