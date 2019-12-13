#pragma once

// #include <string>
#include <unordered_map>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

#include "Types.h"

class Input {
public:
	Input();
	~Input();
	void Update(GLFWwindow*);
	bool KeyPressed(uint);
	bool KeyJustPressed(uint);
	glm::ivec2 MousePos();
	glm::ivec2 MousePosDelta();
private:
	glm::ivec2 _mousePos;
	glm::ivec2 _mousePosDelta;
	std::unordered_map<uint32, bool> _pressMap;
	std::unordered_map<uint32, bool> _justPressedMap;
};