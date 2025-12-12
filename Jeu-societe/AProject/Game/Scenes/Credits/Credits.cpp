#include "Credits.hpp"


void Credits::Load()
{
	m_data = new SceneData;
	m_data->gameData = (GameData*)this->m_keptData;

	m_data->transition.SetTransition(TransitionClass::FADED_IN);
	m_data->transition.PlayTransition();

	m_data->videoPlayer.loadFromFile("Assets/Video/Credits.mp4");


	m_data->background.setTexture(*m_data->gameData->m_assetManager->GetAsset<sf::Texture>("MinigameBackground", AssetManager::AssetType::TEXTURE));

}

void Credits::Unload()
{

}

void Credits::PollEvent(sf::Event& _event)
{
}
void Credits::Update(float _dt)
{
	switch (m_data->state)
	{
		case CREDITS_TRANSITION_1:

			m_data->transition.Update(_dt);

			if (m_data->transition.IsFinished())
			{
				m_data->state = ANIMATION;
				m_data->videoPlayer.play();
				m_data->videoPlayer.update(1.f);
			}
			break;

		case ANIMATION:

			m_data->videoPlayer.update(_dt);

			if (m_data->videoPlayer.isFinish())
			{
				ChangeScene("Menu");
			}
			break;
	}
}

void Credits::Draw(sf::RenderWindow& _window)
{
	_window.draw(m_data->background);
	switch (m_data->state)
	{
	case CREDITS_TRANSITION_1:

		m_data->transition.Draw(_window);
		break;

	case ANIMATION:

		sf::Sprite vid = m_data->videoPlayer.getSprite();
		_window.draw(vid);
		break;
	}
}
