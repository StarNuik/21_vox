#include "base.h"
#include "Utilities/text.h"

void	Log(std::string text) {
	std::cout << TEXT_CYAN + text + TEXT_DEFAULT << std::endl;
}

void	LogError(std::string text) {
	std::cout << TEXT_RED + text + TEXT_DEFAULT << std::endl;
}

void	LogSuccess(std::string text) {
	std::cout << TEXT_GREEN + text + TEXT_DEFAULT << std::endl;
}

void	LogWarning(std::string text) {
	std::cout << TEXT_YELLOW + text + TEXT_DEFAULT << std::endl;
}