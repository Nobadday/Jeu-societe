#include "Credits.hpp"


void Credits::Load()
{
	m_data->transition.PlayTransition();

	m_data->videoPlayer.loadFromFile("Assets/Video/Credits.mp4");
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
				m_data->state = CREDITS_TRANSITION_2;
				m_data->transition.SetTransition(TransitionClass::FADED_OUT);
				m_data->transition.PlayTransition();
			}
			break;

		case CREDITS_TRANSITION_2:

			m_data->transition.Update(_dt);

			if (m_data->transition.IsFinished())
			{
				ChangeScene("MainMenu");
			}
			break;
	}
}

void Credits::Draw(sf::RenderWindow& _window)
{
	switch (m_data->state)
	{
	case CREDITS_TRANSITION_1:
	case CREDITS_TRANSITION_2:

		m_data->transition.Draw(_window);
		break;

	case ANIMATION:

		sf::Sprite vid = m_data->videoPlayer.getSprite();
		_window.draw(vid);
		break;
	}
}
