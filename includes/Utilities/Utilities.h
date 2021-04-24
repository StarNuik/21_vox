#pragma once

#include <string>
#include <assimp/scene.h>
//#include <glm/glm.hpp>
#include "Mathf/Mathf.h"

class Utilities {
public:
	static char* ReadFile(std::string path);
	static mathf::mat4x4 mat4_cast(aiMatrix4x4);
};