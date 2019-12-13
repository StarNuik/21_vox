#pragma once

#include <string>

#define TEXT_RED std::string("\e[91m")
#define TEXT_GREEN std::string("\e[92m")
#define TEXT_YELLOW std::string("\e[93m")
#define TEXT_MAGENTA std::string("\e[95m")
#define TEXT_CYAN std::string("\e[96m")
#define TEXT_BLACK std::string("\e[30m")
#define TEXT_DEFAULT std::string("\e[39m")
#define BG_RED std::string("\e[41m")
#define BG_YELLOW std::string("\e[43m")
#define BG_GREEN std::string("\e[42m")
#define BG_GRAY std::string("\e[47m")
#define BG_DEFAULT std::string("\e[49m")

class ILogger {
public:
	virtual ~ILogger() {};
	virtual void Log(std::string) = 0;
	virtual void LogSuccess(std::string) = 0;
	virtual void LogWarning(std::string) = 0;
	virtual void LogError(std::string) = 0;
};

class NullLogger : public ILogger {
public:
	virtual void Log(std::string) {};
	virtual void LogSuccess(std::string) {};
	virtual void LogWarning(std::string) {};
	virtual void LogError(std::string) {};
};

class TerminalLogger : public ILogger {
	public:
	TerminalLogger();
	virtual void Log(std::string);
	virtual void LogSuccess(std::string);
	virtual void LogWarning(std::string);
	virtual void LogError(std::string);
};

// class FileLogger : public Logger {
// 	public:
// 	virtual void Log(std::string);
// 	virtual void LogSuccess(std::string);
// 	virtual void LogWarning(std::string);
// 	virtual void LogError(std::string);
// };