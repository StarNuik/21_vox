#include "Base.h"
#include "Utilities/Log.h"



TerminalLogger::TerminalLogger() {
	LogSuccess("[TerminalLogger::TerminalLogger]\nInitialized Logger system successfully.");
}

void TerminalLogger::Log(std::string text) {
	std::cout << TEXT_DEFAULT + text + TEXT_DEFAULT << std::endl;
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