#pragma once
#include <iostream>
#include <string>
#include <algorithm>


namespace Utils {

#include <windows.h>
	inline std::string GetWorkingDirectory()
	{

#ifdef _WIN32

		TCHAR output[FILENAME_MAX];
		if (GetCurrentDirectory(FILENAME_MAX, output) == 0)
		{

			return{};
		}
		std::string retVal(output);
		std::replace(retVal.begin(), retVal.end(), '\\', '/');
		return retVal;
#else //this may not work on macOS
		char output[FILENAME_MAX];
		if (getcwd(output, FILENAME_MAX) == 0)
		{

			return{};
		}
		return{ output };
#endif //_WIN32
	}
}
