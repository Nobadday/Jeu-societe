#pragma once
#ifndef _INC_CONTROLLER_HPP
#define _INC_CONTROLLER_HPP

#include <SFML/Window.hpp>
#include <nlohmann/json.hpp>


namespace sfMod
{

class Controller
{
	enum Button
	{
		A,
		B,
		X,
		Y
	};

	public:
		static bool LoadMap(const std::string& _filePath);
		static bool LoadMap(const std::fstream& _file);
};




}

#endif
// Controller Mappings SFML || v0.0