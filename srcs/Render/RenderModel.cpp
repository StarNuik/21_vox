#include <GL/glew.h>

#include "Render/RenderModel.h"
#include "Render/Shader.h"
#include "Render/Geometry.h"
#include "Render/Texture.h"
#include "Render/Camera.h"
#include "Render/GLRenderer.h"

RenderModel::RenderModel(Shader* shader, Texture* diffuseTexture, Texture* normalTexture, Geometry* geometry) {
	Init(shader, diffuseTexture, normalTexture, geometry);
}

RenderModel::RenderModel(Shader* shader, Texture* diffuseTexture, Geometry* geometry) {
	Init(shader, diffuseTexture, nullptr, geometry);
}

RenderModel::RenderModel(GLRenderer* renderer, Shader* shader, Texture* diffuseTexture, Texture* normalTexture, Geometry* geometry) {
	Init(shader, diffuseTexture, normalTexture, geometry);
	_renderer = renderer;
	_renderer->AddModel(this);
}

RenderModel::RenderModel(GLRenderer* renderer, Shader* shader, Texture* diffuseTexture, Geometry* geometry) {
	Init(shader, diffuseTexture, nullptr, geometry);
	_renderer = renderer;
	_renderer->AddModel(this);
}

RenderModel::~RenderModel() {
	if (_renderer) {
		_renderer->RemoveModel(this);
	}
};

void RenderModel::Init(Shader* shader, Texture* diffuseTexture, Texture* normalTexture, Geometry* geometry) {
	_shader = shader;
	_diffuseTexture = diffuseTexture;
	_normalTexture = normalTexture;
	_geometry = geometry;
	_position = glm::vec3(0.f);
	_scale = glm::vec3(1.f);
	_rotation = glm::identity<glm::quat>();
}

Shader* RenderModel::Use(Camera* camera) {
	_shader->Use();
	_geometry->Use();
	glActiveTexture(GL_TEXTURE0);
	_diffuseTexture->Use();
	if (_normalTexture) {
		//! IDK if this works
		glActiveTexture(GL_TEXTURE1);
		_normalTexture->Use();
		glActiveTexture(GL_TEXTURE0);
	}
	_shader->SetMatrix4("model", GetAphineMatrix());
	_shader->SetMatrix4("view", camera->GetViewMatrix());
	_shader->SetMatrix4("projection", camera->GetProjectionMatrix());
	_shader->SetFloat3("viewPos", camera->GetPosition());
	return _shader;
}

glm::mat4 RenderModel::GetAphineMatrix() {
	glm::mat4 aphine = glm::identity<glm::mat4>();

	aphine = glm::translate(aphine, _position);
	aphine = aphine * glm::mat4_cast(_rotation);
	aphine = glm::scale(aphine, _scale);
	return aphine;
};

void RenderModel::SetPosition(glm::vec3 position) {_position = position;};
void RenderModel::SetRotation(glm::quat rotation) {_rotation = rotation;};
void RenderModel::SetScale(glm::vec3 scale) {_scale = scale;};
glm::vec3 RenderModel::GetPosition() {return _position;};
glm::quat RenderModel::GetRotation() {return _rotation;};
glm::vec3 RenderModel::GetScale() {return _scale;};
uint RenderModel::GetPolygonCount() {return _geometry->GetPolygonCount();};
Geometry* RenderModel::GetGeometry() {return _geometry;};