#pragma once

#ifndef _INC_CONTROLLER_HPP
#define _INC_CONTROLLER_HPP


#include <SFML/Window.hpp>
#include <nlohmann/json.hpp>

namespace sf
{

class ControllerManager
{

	public:
		static bool LoadMap(const std::string& _filePath);

};




}

#endif