#include "FileTools.hpp"


namespace filetools
{


	std::string GetDirName(const std::string& _path)
	{
		// stackoverflow.com/questions/8518743/get-directory-from-file-path-c
		size_t pos = _path.find_last_of("\\/");
		if (std::string::npos == pos)
		{
			return "";
		}
		return _path.substr(0, pos+1);
	}
}