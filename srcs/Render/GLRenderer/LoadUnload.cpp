#include "Render/Render.h"

GLFWwindow* GLRenderer::GetWindow() {
	return _window;
}

void GLRenderer::AddShader(std::string shaderName, Shader* shader) {
	_shaders[shaderName] = shader;
};

void GLRenderer::RemoveShader(std::string shaderName) {
	_shaders.erase(shaderName);
};

Shader* GLRenderer::GetShader(std::string shaderName) {
	return _shaders[shaderName];
};


void GLRenderer::AddTexture(std::string textureName, Texture* texture) {
	_textures[textureName] = texture;
};

void GLRenderer::RemoveTexture(std::string textureName) {
	_textures.erase(textureName);
};

Texture* GLRenderer::GetTexture(std::string textureName) {
	return _textures[textureName];
};


void GLRenderer::AddGeometry(std::string geometryName, Geometry* geometry) {
	_geometries[geometryName] = geometry;
};

void GLRenderer::RemoveGeometry(std::string geometryName) {
	_geometries.erase(geometryName);
};

Geometry* GLRenderer::GetGeometry(std::string geometryName) {
	return _geometries[geometryName];
};


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
