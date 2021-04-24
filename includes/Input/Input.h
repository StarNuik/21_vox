#pragma once

// #include <string>
#include <unordered_map>
#include "Mathf/Mathf.h"
#include <GLFW/glfw3.h>

#include "Types.h"

class Input {
public:
	Input();
	~Input();
	void Update(GLFWwindow*);
	bool KeyPressed(uint);
	bool KeyJustPressed(uint);
	bool MouseKeyPressed(uint);
	bool MouseKeyJustPressed(uint);
	mathf::ivec2 MousePos();
	mathf::ivec2 MousePosDelta();
private:
	mathf::ivec2 _mousePos;
	mathf::ivec2 _mousePosDelta;
	std::unordered_map<uint32, bool> _pressMap;
	std::unordered_map<uint32, bool> _justPressedMap;
	std::unordered_map<uint32, bool> _mousePressMap;
	std::unordered_map<uint32, bool> _mouseJustPressMap;
};