#pragma once
#include <string>
#include <unordered_map>
#include <glm/glm.hpp>
#include <glfw/glfw3.h>
#include "Types.h"

class Input {
public:
	Input();
	~Input();
	void Update(GLFWwindow*);
	bool KeyPressed(uint32);
	// bool KeyJustPressed(uint32);
	glm::ivec2 MousePos();
private:
	glm::ivec2 _mousePos;
	std::unordered_map<uint32, bool> _pressMap;
	// std::unordered_map<uint32, bool> _justPressedMap;
};