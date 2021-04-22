#include <GL/glew.h>
#include <glm/gtx/matrix_decompose.hpp>

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
	_position = mathf::vec3(0.f);
	_scale = mathf::vec3(1.f);
	_rotation = glm::identity<glm::quat>();
	_model = glm::mat4(1.f);
	if (_renderer)
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
	shader->SetMatrix4("model", _model);
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
	shader->SetMatrix4("model", _model);
	// shader->SetMatrix4("view", camera->GetViewMatrix());
	// shader->SetMatrix4("projection", camera->GetProjectionMatrix());
	// shader->SetFloat3("cameraPos", camera->GetPosition());
};

Shader* RenderModel::Use(Camera* camera) {
	ApplySelf(camera, _shader);
	return _shader;
}

void RenderModel::RecalculateModelMatrix() {
	_model = glm::identity<glm::mat4>();

	_model = glm::translate(_model, _position.to_glm());
	_model = _model * glm::mat4_cast(_rotation);
	_model = glm::scale(_model, _scale.to_glm());
};

bool RenderModel::operator<(const RenderModel& t) const {
	return ((_shaderId << 8) | _materialId) < ((t._shaderId << 8) | t._materialId);
};

bool RenderModelLess(RenderModel*& x, RenderModel*& y) {
    return ((x->_shaderId << 8) | x->_materialId) < ((y->_shaderId << 8) | y->_materialId);
}

void RenderModel::SetModelMatrix(glm::mat4 matrix) {
	_model = matrix;
	glm::vec3 skew;
	glm::vec4 perspective;
	glm::vec3 pos;
	glm::vec3 scl;
	glm::decompose(_model, scl, _rotation, pos, skew, perspective);
	_position.x = pos.x;
	_position.y = pos.y;
	_position.z = pos.z;
	_scale.x = scl.x;
	_scale.y = scl.y;
	_scale.z = scl.z;
}

void RenderModel::SetPosition(mathf::vec3 position) {_position = position; RecalculateModelMatrix();};
void RenderModel::SetRotation(glm::quat rotation) {_rotation = rotation; RecalculateModelMatrix();};
void RenderModel::SetScale(mathf::vec3 scale) {_scale = scale; RecalculateModelMatrix();};
mathf::vec3 RenderModel::GetPosition() {return _position;};
glm::quat RenderModel::GetRotation() {return _rotation;};
mathf::vec3 RenderModel::GetScale() {return _scale;};
uint RenderModel::GetPolygonCount() {return _geometry->GetPolygonCount();};
Geometry* RenderModel::GetGeometry() {return _geometry;};
Material* RenderModel::GetMaterial() {return _material;};
Shader* RenderModel::GetShader() {return _shader;};
glm::mat4 RenderModel::GetModelMatrix() {return _model;};