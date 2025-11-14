#include "Menu.hpp"



Button* CreateButton(std::string _texturePath, TextureAnimated::AnimationType _textureType)
{
	return new Button();
}

void Menu::LoadUI(void)
{
	//m_data->menuSystem = new MenuSystem();
	//m_data->menuSystem->MenuAdd("Menu", true);

	//m_data->ui.playBtnTexAnim = new TextureAnimated();
	//m_data->ui.playBtnTexAnim->LoadFromFile("Assets/Sprites/Menu/PlayButton.anim", TextureAnimated::AnimationType::ANIMATION_ANIM);
	//m_data->ui.settingsBtnTexAnim = new TextureAnimated();
	//m_data->ui.settingsBtnTexAnim->LoadFromFile("Assets/Sprites/Menu/PlayButton.anim", TextureAnimated::AnimationType::ANIMATION_ANIM);
	//m_data->ui.leaveBtnTexAnim = new TextureAnimated();
	//m_data->ui.leaveBtnTexAnim->LoadFromFile("Assets/Sprites/Menu/leave.anim", TextureAnimated::AnimationType::ANIMATION_ANIM);
	//	
	//Button& play = m_data->menuSystem->MenuCreateButton("Menu", "Play");
	//play.setTexture(*m_data->ui.playBtnTexAnim);
	//Button& settings = m_data->menuSystem->MenuCreateButton("Menu", "Settings");
	//settings.setTexture(*m_data->ui.settingsBtnTexAnim);
	//Button& leave = m_data->menuSystem->MenuCreateButton("Menu", "Leave");
	//leave.setTexture(*m_data->ui.leaveBtnTexAnim);


	//m_data->menuSystem->SetMenuHolder("Menu");


}

void Menu::CheckButtons(void)
{
	//if (m_data->menuSystem->GetButton("Play").HasBeenClicked())
	//{
	//	std::cout << "Eheh you click on play" << std::endl;

	//}

}

void Menu::Load(void)
{
	m_data = new SceneData;
	m_data->gameData = (GameData*)this->m_keptData;






	//TextureAnimated* playBtnAnim = new TextureAnimated();
	//playBtnAnim->LoadFromFile("Assets/Sprites/PlayButton.anim", TextureAnimated::AnimationType::ANIMATION_ANIM);
	//playBtnAnim->LoadFr("Assets/Sprites/PlayButton.anim", TextureAnimated::AnimationType::ANIMATION_ANIM);


	//m_data->buttonTest = new Button();
	//
	//m_data->textanim = new TextureAnimated();
	//m_data->textanim->LoadFromFile("Assets/Sprites/ButtonPlaceHolder.anim", TextureAnimated::AnimationType::ANIMATION_ANIM);
	//m_data->buttonTest->setTexture(*m_data->textanim);



}
void Menu::Unload(void)
{

}

void Menu::PollEvent(sf::Event& _event)
{
	//m_data->menuSystem->PollEvent(_event);

}
void Menu::Update(float _deltaTime)
{
	CheckButtons();
	//m_data->menuSystem->Update(_deltaTime);
}
void Menu::Draw(sf::RenderWindow& _renderWindow)
{
	//m_data->menuSystem->Draw(_renderWindow, sf::RenderStates::Default);

}