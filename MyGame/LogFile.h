#pragma once

#include <string>


class LogFile
{
public:
	enum LogTypes
	{
		Information,
		Warning,
		Error
	};

	static void WriteLog(std::string text, LogTypes logType = Information);
};
