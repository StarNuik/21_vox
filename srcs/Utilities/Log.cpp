// #include "Base.h"
#include "Utilities/Log.h"
#include <iostream>

// TerminalLogger::TerminalLogger() {
// 	LogSuccess("[TerminalLogger::TerminalLogger]\nInitialized Logger system successfully.");
// }

// void TerminalLogger::Log(std::string text) {
// 	std::cout << TEXT_DEFAULT + text + TEXT_DEFAULT << std::endl;
// }

// void TerminalLogger::LogError(std::string text) {
// 	std::cout << TEXT_RED + text + TEXT_DEFAULT << std::endl;
// }

// void TerminalLogger::LogSuccess(std::string text) {
// 	std::cout << TEXT_GREEN + text + TEXT_DEFAULT << std::endl;
// }

// void TerminalLogger::LogWarning(std::string text) {
// 	std::cout << TEXT_YELLOW + text + TEXT_DEFAULT << std::endl;
// }

void Log::Basic(std::string text) {std::cout << TEXT_DEFAULT + text + TEXT_DEFAULT << std::endl;}
void Log::Error(std::string text) {std::cout << TEXT_RED + text + TEXT_DEFAULT << std::endl;}
void Log::Warning(std::string text) {std::cout << TEXT_YELLOW + text + TEXT_DEFAULT << std::endl;}
void Log::Important(std::string text) {std::cout << TEXT_CYAN + text + TEXT_DEFAULT << std::endl;}
void Log::Success(std::string text) {std::cout << TEXT_GREEN + text + TEXT_DEFAULT << std::endl;}
void Log::ErrorBg(std::string text) {std::cout << BG_RED + text + BG_DEFAULT << std::endl;}
void Log::WarningBg(std::string text) {std::cout << BG_YELLOW + text + BG_DEFAULT << std::endl;}
void Log::ImportantBg(std::string text) {std::cout << BG_CYAN + text + BG_DEFAULT << std::endl;}
void Log::SuccessBg(std::string text) {std::cout << BG_GREEN + text + BG_DEFAULT << std::endl;}