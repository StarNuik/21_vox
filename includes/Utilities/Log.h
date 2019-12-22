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
#define BG_CYAN std::string("\e[46m")
#define BG_DEFAULT std::string("\e[49m")

class Log {
public:
	static void Basic(std::string);
	static void Error(std::string);
	static void Warning(std::string);
	static void Important(std::string);
	static void Success(std::string);
	static void ErrorBg(std::string);
	static void WarningBg(std::string);
	static void ImportantBg(std::string);
	static void SuccessBg(std::string);
};