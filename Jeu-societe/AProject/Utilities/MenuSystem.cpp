//#include "MenuSystem.hpp"
//
//class Button;
//
//
//
//
//
//
//#pragma region MENU_HOLDER
//MenuHolder::MenuHolder(bool _looping = true)
//	: m_selectionLooping(_looping),
//	m_selection(-1)
//{
//
//
//
//
//}
//MenuHolder::~MenuHolder(void)
//{
//	//Destroy list
//
//	for (auto buttons : m_buttons)
//	{
//		//ButtonDestroy(buttons.second);
//
//
//		//Web3school, for each in loop example
//		//std::cout << buttons.first << " is: " << buttons.second << "\n";
//	}
//
//
//	m_buttons.clear();
//
//	//Original
//	//for (int i = PyDictGetLen(_menu->buttons) - 1; i >= 0; i--)
//	//{
//	//	ButtonDestroy(PyDictPopIndex(_menu->buttons, i));
//	//}
//	//PyDictDestroy(_menu->buttons);
//	//free(_menu);
//}
//
//void MenuHolder::PollEvent(sf::Event& _event)
//{
//	if (_event.type == sf::Event::MouseButtonPressed)
//	{
//		ResetSelection();
//	}
//	for (auto buttons : m_buttons)
//	{
//		//ButtonCheckEvent(buttons.second, _event);
//	}
//}
//void MenuHolder::Update(float _dt)
//{
//	//int i = 0;
//	//for (auto buttons : m_buttons)
//	//{
//	//	Button& btnREF = buttons.second;
//	//	if (i == m_selection)
//	//	{
//	//		btnREF.stateIsON = true;
//	//		btnREF.UpdateTexture();
//	//	}
//	//	btnREF.Update(_deltaTime);
//	//	i++;
//	//}
//}
//void MenuHolder::Draw(sf::RenderWindow& _renderWindow, sf::RenderStates _states)
//{
//	for (auto buttons : m_buttons)
//	{
//		(buttons.second).draw(_renderWindow, _states);
//
//	}
//}
//
//void MenuHolder::AddButton(std::string _name, Button& _button)
//{
//}
//Button& MenuHolder::GetButton(std::string _name)
//{
//	// TODO: insérer une instruction return ici
//}
//Button& MenuHolder::GetButton(int _value)
//{
//	// TODO: insérer une instruction return ici
//}
//
//Button& MenuHolder::PopButton(float _name)
//{
//	// TODO: insérer une instruction return ici
//}
//Button& MenuHolder::PopButton(int _value)
//{
//	// TODO: insérer une instruction return ici
//}
//
//int MenuHolder::GetButtonCount(void)
//{
//	return 0;
//}
//
//void MenuHolder::SetSelection(int _selection)
//{
//}
//void MenuHolder::AddSelection(int _value)
//{
//}
//void MenuHolder::ResetSelection(void)
//{
//}
//void MenuHolder::ConfirmSelection(void)
//{
//}
//#pragma endregion