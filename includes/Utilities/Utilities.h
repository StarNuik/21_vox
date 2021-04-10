#pragma once

#include <string>
#include <assimp/scene.h>
#include <glm/glm.hpp>

class Utilities {
public:
	static char* ReadFile(std::string path);
	static glm::mat4 mat4_cast(aiMatrix4x4);
};