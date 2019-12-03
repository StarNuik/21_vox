#include "Utilities/Utilities.h"
#include "Engine/Locator.hpp"
#include <fstream>

char* Utilities::ReadFile(std::string path)
{
	std::ifstream file;

	file.open(path);
	if (!file.is_open()) {
		Locator::getLogger()->LogError("[Utilities::ReadFile]\nCouldn't open file: " + path);
	}
	file.seekg(0, std::ios::end);
	const int len = file.tellg();
	file.seekg(0, std::ios::beg);
	char* buffer = new char[len + 1];
	file.read(buffer, len);
	buffer[len] = '\0';
	file.close();
	return (buffer);
}