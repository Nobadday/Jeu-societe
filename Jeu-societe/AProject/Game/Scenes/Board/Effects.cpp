#include "Board.hpp"

void BaseGame::CreateSmokeEffectForSwap(Player& _player)
{
	for (int i = 0; i < 63; i++)
	{
		// Créer un effet visuel ici 
		if (i % 3 == 0)
		{
			sf::Vector2f posEffect = _player.boardPosition;
			sf::Vector2u bit = _player.texture.GetTexture().getSize();
			posEffect.y -= bit.y * 2.f / 6.f;

			Effect effect(m_data->smoke, posEffect + sf::Vector2f(-20.f + randmt::RandomFloat(0, 40), -20.f + randmt::RandomFloat(0, 40)), randmt::RandomFloat(0.5f, 1.f), 360 * randmt::RandomFloat(0, 360));
			m_data->effectSwap.push_back(effect);
		}
		else if (i % 2 == 0)
		{
			sf::Vector2f posEffect = _player.boardPosition;
			sf::Vector2u bit = _player.texture.GetTexture().getSize();
			posEffect.y -= bit.y * 1.5f / 6.f;

			Effect effect(m_data->smoke, posEffect + sf::Vector2f(-20.f + randmt::RandomFloat(0.f, 40.f), -20.f + randmt::RandomFloat(0.f, 40.f)), randmt::RandomFloat(0.5f, 1.f), 360 * randmt::RandomFloat(0, 360));
			m_data->effectSwap.push_back(effect);
		}
		else
		{
			sf::Vector2f posEffect = _player.boardPosition;
			sf::Vector2u bit = _player.texture.GetTexture().getSize();
			posEffect.y -= bit.y * 1.f / 6.f;

			Effect effect(m_data->smoke, posEffect + sf::Vector2f(-20.f + randmt::RandomFloat(0, 40), -20.f + randmt::RandomFloat(0, 40)), randmt::RandomFloat(0.5f, 1.f), 360 * randmt::RandomFloat(0, 360));
			m_data->effectSwap.push_back(effect);
		}
	}
}

void BaseGame::CreateSmokeEffectAnotherPart(sf::Vector2f _posMin, sf::Vector2f _posMax)
{
	for (int i = 0; i < 1002; i++)
	{
		sf::Vector2f posEffect;
		posEffect.x = randmt::RandomFloat(_posMin.x + 512.f / 2.f, _posMax.x);
		posEffect.y = randmt::RandomFloat(0, SCREEN_HEIGHT);

		Effect effect(m_data->smoke, posEffect + sf::Vector2f(-20.f + randmt::RandomInt(0, 40), -20.f + randmt::RandomInt(0, 40)), randmt::RandomFloat(0.5f, 1.f), 360 * randmt::RandomFloat(0, 360));
		m_data->effectsMap.push_back(effect);
	}
}