#pragma once

#ifndef _INC_FILETOOLS_HPP
#define _INC_FILETOOLS_HPP

#include <fstream>
#include <filesystem>

namespace filetools
{

	// Get the firectory path from the given path
	// "./Assets/Files/Data.json" -> "./Assets/Files/"
	std::string GetDirName(const std::string& _path);

}
#endif

// FileTools C++ v1.0