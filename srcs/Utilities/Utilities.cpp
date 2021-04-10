#include "Utilities/Utilities.h"
#include "Utilities/Log.h"
#include <fstream>

char* Utilities::ReadFile(std::string path)
{
	std::ifstream file;

	file.open(path);
	if (!file.is_open()) {
		Log::Error("[Utilities::ReadFile]\nCouldn't open file: " + path);
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

glm::mat4 Utilities::mat4_cast(aiMatrix4x4 matrix) {
	glm::mat4 result(0.f);

	result[0][0] = matrix[0][0];
	result[1][0] = matrix[0][1];
	result[2][0] = matrix[0][2];
	result[3][0] = matrix[0][3];
	result[0][1] = matrix[1][0];
	result[1][1] = matrix[1][1];
	result[2][1] = matrix[1][2];
	result[3][1] = matrix[1][3];
	result[0][2] = matrix[2][0];
	result[1][2] = matrix[2][1];
	result[2][2] = matrix[2][2];
	result[3][2] = matrix[2][3];
	result[0][3] = matrix[3][0];
	result[1][3] = matrix[3][1];
	result[2][3] = matrix[3][2];
	result[3][3] = matrix[3][3];
	return result;
}
