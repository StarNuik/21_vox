#include <string>
#include <GL/glew.h>
#include "Render/GLRenderer.h"
#include "Render/Skybox.h"
#include "Render/Shader.h"
#include "Render/Texture.h"
#include "Render/Framebuffer.h"
#include "World/ResourceLoader.h"

void GLRenderer::SetActiveCamera(Camera* camera) {
	_static.activeCamera = camera;
	_static.skybox->SetActiveCamera(camera);
};

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

void GLRenderer::SetPostShader(std::string newShaderName) {
	_static.postShader = _static.rs->GetShader(newShaderName);
	_static.postShader->Use();
	_static.postShader->SetInt("screenTexture", 0);
	_static.postShader->SetInt("bloomTexture", 1);
}

GLFWwindow* GLRenderer::GetWindow() {return _static.window;}
glm::ivec2 GLRenderer::GetWindowSize() {return _static.windowSize;};
Camera* GLRenderer::GetActiveCamera() {return _static.activeCamera;};
float GLRenderer::GetBloomCutoff() {return _static.bloomCutoff;};
void GLRenderer::SetBloomCutoff(float val) {_static.bloomCutoff = val;};
