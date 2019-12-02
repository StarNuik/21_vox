#include "Base.h"
#include "Utilities/Log.h"

void TerminalLogger::Log(std::string text) {
	std::cout << TEXT_CYAN + text + TEXT_DEFAULT << std::endl;
}

void TerminalLogger::LogError(std::string text) {
	std::cout << TEXT_RED + text + TEXT_DEFAULT << std::endl;
}

void TerminalLogger::LogSuccess(std::string text) {
	std::cout << TEXT_GREEN + text + TEXT_DEFAULT << std::endl;
}

void TerminalLogger::LogWarning(std::string text) {
	std::cout << TEXT_YELLOW + text + TEXT_DEFAULT << std::endl;
}