#include "Types.h"
#include "Input/Input.h"
#include <GLFW/glfw3.h>


Input::Input() {
	for (int i = 0; i <= GLFW_KEY_LAST; i++) {
		_pressMap[i] = false;
	}
	for (int i = 0; i <= GLFW_KEY_LAST; i++) {
		_justPressedMap[i] = false;
	}
	for (int i = 0; i <= GLFW_MOUSE_BUTTON_LAST; i++) {
		_mousePressMap[i] = false;
	}
	for (int i = 0; i <= GLFW_MOUSE_BUTTON_LAST; i++) {
		_mouseJustPressMap[i] = false;
	}
};

bool Input::KeyPressed(uint keycode) {return _pressMap[keycode];};
bool Input::KeyJustPressed(uint keycode) {return _justPressedMap[keycode];};
bool Input::MouseKeyPressed(uint keycode) {return _mousePressMap[keycode];};
bool Input::MouseKeyJustPressed(uint keycode) {return _mouseJustPressMap[keycode];};
glm::ivec2 Input::MousePos() {return _mousePos;};
glm::ivec2 Input::MousePosDelta() {return _mousePosDelta;};

void Input::Update(GLFWwindow* window) {
	glfwPollEvents();
	for (int i = 0; i <= GLFW_KEY_LAST; i++) {
		//? Get state
		bool state = (glfwGetKey(window, i) == GLFW_PRESS);
		//? If [justPressed] last tick, change [justPressed] to false
		if (_justPressedMap[i])
			_justPressedMap[i] = false;

		//? If key wasn't pressed and now is pressed, change [justPressed] to true
		if (state == true && _pressMap[i] == false)
			_justPressedMap[i] = true;

		_pressMap[i] = state;

	}
	for (int i = 0; i <= GLFW_MOUSE_BUTTON_LAST; i++) {
		bool state = (glfwGetMouseButton(window, i) == GLFW_PRESS);
		if (_mouseJustPressMap[i])
			_mouseJustPressMap[i] = false;
		if (state == true && _mousePressMap[i] == false)
			_mouseJustPressMap[i] = true;
		_mousePressMap[i] = state;
	}

	double xPos, yPos;
	glfwGetCursorPos(window, &xPos, &yPos);
	glm::ivec2 mousePos = glm::ivec2(xPos, yPos);
	_mousePosDelta = mousePos - _mousePos;
	_mousePos = mousePos;
}

Input::~Input() {};