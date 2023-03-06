#include "LogFile.h"
#include <fstream>
#include <map>


const std::map<LogFile::LogTypes, const char*> TYPES = {
	{ LogFile::Information, "INFO" },
	{ LogFile::Warning, "WARNING" },
	{ LogFile::Error, "ERROR" }
};

void LogFile::WriteLog(std::string text, LogTypes logType)
{
	std::ofstream file("log.log", std::ios_base::out | std::ios_base::app);
	if (file.is_open())
	{
		time_t currTime = time(nullptr);
		tm now;
		localtime_s(&now, &currTime);
		char timeStr[23];
		strftime(timeStr, sizeof(timeStr), "%d/%m/%Y  %H:%M:%S  ", &now);

		file << timeStr << TYPES.at(logType) << "  " << text << std::endl;

		file.close();
	}
}
