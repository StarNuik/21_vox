#include <GL/glew.h>

#include "Render/RenderModel.h"
#include "Render/Shader.h"
#include "Render/Geometry.h"
#include "Render/Material.h"
#include "Render/Camera.h"
#include "Render/GLRenderer.h"

RenderModel::RenderModel(GLRenderer* renderer, Shader* shader, Material* material, Geometry* geometry) {
	_renderer = renderer;
	_shader = shader;
	_material = material;
	_geometry = geometry;
	_position = glm::vec3(0.f);
	_scale = glm::vec3(1.f);
	_rotation = glm::identity<glm::quat>();
	_renderer->AddModel(this);
}

RenderModel::~RenderModel() {
	if (_renderer) {
		_renderer->RemoveModel(this);
	}
};

Shader* RenderModel::Use(Camera* camera) {
	_shader->Use();
	_geometry->Use();
	_material->Use(_shader);
	_shader->SetMatrix4("model", GetAphineMatrix());
	_shader->SetMatrix4("view", camera->GetViewMatrix());
	_shader->SetMatrix4("projection", camera->GetProjectionMatrix());
	_shader->SetFloat3("cameraPos", camera->GetPosition());
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