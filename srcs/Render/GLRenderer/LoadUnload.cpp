#include "Render/Render.h"

GLFWwindow* GLRenderer::GetWindow() {
	return _window;
}

glm::ivec2 GLRenderer::GetWindowSize() {return glm::ivec2(_width, _height);};
void GLRenderer::SetActiveCamera(Camera* camera) {_activeCamera = camera;};
Camera* GLRenderer::GetActiveCamera() {return _activeCamera;};

void GLRenderer::AddModel(RenderModel* model) {
	_rendered.push_back(model);
};

void GLRenderer::RemoveModel(RenderModel* model) {
	std::vector<RenderModel*>::iterator pos;
	pos = std::find(_rendered.begin(), _rendered.end(), model);
	if (pos != _rendered.end())
	{
		_rendered.erase(pos);
	}
};
