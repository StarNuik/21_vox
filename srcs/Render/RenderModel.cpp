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
	_shaderId = _shader->GetId();
	_material = material;
	_materialId = _material->GetId();
	_geometry = geometry;
	_geometryId = _geometry->GetId();
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

void RenderModel::ApplySelf(Camera* camera, Shader* shader) {
	// glUseProgram(_shaderId);
	shader->Use();
	_geometry->Use();
	// glBindVertexArray(_geometryId);
	// _material->Use(shader);
	shader->SetMatrix4("model", GetAphineMatrix());
	// shader->SetMatrix4("view", camera->GetViewMatrix());
	// shader->SetMatrix4("projection", camera->GetProjectionMatrix());
	// shader->SetFloat3("cameraPos", camera->GetPosition());
};

void RenderModel::ApplySelf(Shader* shader) {
	// glUseProgram(_shaderId);
	// shader->Use();
	_geometry->Use();
	// glBindVertexArray(_geometryId);
	// _material->Use(shader);
	shader->SetMatrix4("model", GetAphineMatrix());
	// shader->SetMatrix4("view", camera->GetViewMatrix());
	// shader->SetMatrix4("projection", camera->GetProjectionMatrix());
	// shader->SetFloat3("cameraPos", camera->GetPosition());
};

Shader* RenderModel::Use(Camera* camera) {
	ApplySelf(camera, _shader);
	return _shader;
}

glm::mat4 RenderModel::GetAphineMatrix() {
	glm::mat4 aphine = glm::identity<glm::mat4>();

	aphine = glm::translate(aphine, _position);
	aphine = aphine * glm::mat4_cast(_rotation);
	// aphine = glm::scale(aphine, _scale);
	return aphine;
};

bool RenderModel::operator<(const RenderModel& t) const {
	return ((_shaderId << 8) | _materialId) < ((t._shaderId << 8) | t._materialId);
};

void RenderModel::SetPosition(glm::vec3 position) {_position = position;};
void RenderModel::SetRotation(glm::quat rotation) {_rotation = rotation;};
void RenderModel::SetScale(glm::vec3 scale) {_scale = scale;};
glm::vec3 RenderModel::GetPosition() {return _position;};
glm::quat RenderModel::GetRotation() {return _rotation;};
glm::vec3 RenderModel::GetScale() {return _scale;};
uint RenderModel::GetPolygonCount() {return _geometry->GetPolygonCount();};
Geometry* RenderModel::GetGeometry() {return _geometry;};
Material* RenderModel::GetMaterial() {return _material;};
Shader* RenderModel::GetShader() {return _shader;};