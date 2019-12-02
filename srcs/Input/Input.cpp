#include "Input/Input.h"
#include <glfw/glfw3.h>
#include "Types.h"

Input::Input() {
	for (int i = 0; i <= GLFW_KEY_LAST; i++) {
		_pressMap[i] = false;
	}
};

bool Input::KeyPressed(uint32 keycode) {
	return _pressMap[keycode];
};

glm::ivec2 Input::MousePos() {
	return _mousePos;
}

// bool Input::KeyJustPressed(uint32 keycode) {
// 	return _justPressedMap[keycode];
// }

void Input::Update(GLFWwindow* window) {
	glfwPollEvents();
	for (int i = 0; i <= GLFW_KEY_LAST; i++) {
		int state = glfwGetKey(window, i);
		if (state == GLFW_PRESS) {
			_pressMap[i] = true;
		} else {
			_pressMap[i] = false;
		}
	}
	double xPos, yPos;
	glfwGetCursorPos(window, &xPos, &yPos);
	_mousePos = glm::ivec2(xPos, yPos);
}

Input::~Input() {};