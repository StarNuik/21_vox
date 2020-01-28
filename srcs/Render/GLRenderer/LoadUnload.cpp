#include "Render/GLRenderer.h"
#include "Render/Skybox.h"

GLFWwindow* GLRenderer::GetWindow() {
	return _static.window;
}

glm::ivec2 GLRenderer::GetWindowSize() {return _static.windowSize;};
void GLRenderer::SetActiveCamera(Camera* camera) {
	_static.activeCamera = camera;
	_static.skybox->SetActiveCamera(camera);
};
Camera* GLRenderer::GetActiveCamera() {return _static.activeCamera;};

void GLRenderer::AddModel(RenderModel* model) {
	_static.rendered.push_back(model);
};

void GLRenderer::RemoveModel(RenderModel* model) {
	std::vector<RenderModel*>::iterator pos;
	pos = std::find(_static.rendered.begin(), _static.rendered.end(), model);
	if (pos != _static.rendered.end()) {
		_static.rendered.erase(pos);
	}
};

void GLRenderer::SetCursor(bool state) {
	glfwSetInputMode(_static.window, GLFW_CURSOR, state ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
}
