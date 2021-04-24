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
	_position = mathf::vec3(0.f);
	_scale = mathf::vec3(1.f);
	_rotation = mathf::quat::identity();
	_model = mathf::mat4x4::identity();
	if (_renderer)
		_renderer->AddModel(this);
}

RenderModel::~RenderModel() {
	if (_renderer) {
		_renderer->RemoveModel(this);
	}
};

void RenderModel::ApplySelf(Camera* camera, Shader* shader) {
	shader->Use();
	_geometry->Use();
	shader->SetMatrix4("model", _model);
};

void RenderModel::ApplySelf(Shader* shader) {
	_geometry->Use();
	shader->SetMatrix4("model", _model);
};

Shader* RenderModel::Use(Camera* camera) {
	ApplySelf(camera, _shader);
	return _shader;
}

void RenderModel::RecalculateModelMatrix() {
	_model = mathf::mat4x4::identity();

	_model = mathf::mat4x4::translate(_model, _position);
	_model = _model * mathf::mat4x4::cast(_rotation);
	_model = mathf::mat4x4::scale(_model, _scale);
};

bool RenderModel::operator<(const RenderModel& t) const {
	return ((_shaderId << 8) | _materialId) < ((t._shaderId << 8) | t._materialId);
};

bool RenderModelLess(RenderModel*& x, RenderModel*& y) {
    return ((x->_shaderId << 8) | x->_materialId) < ((y->_shaderId << 8) | y->_materialId);
}

void RenderModel::SetModelMatrix(mathf::mat4x4 matrix) {
	_model = matrix;
	mathf::mat4x4::decompose(_model, _scale, _rotation, _position);

}

void RenderModel::SetPosition(mathf::vec3 position) {_position = position; RecalculateModelMatrix();};
void RenderModel::SetRotation(mathf::quat rotation) {_rotation = rotation; RecalculateModelMatrix();};
void RenderModel::SetScale(mathf::vec3 scale) {_scale = scale; RecalculateModelMatrix();};
mathf::vec3 RenderModel::GetPosition() {return _position;};
mathf::quat RenderModel::GetRotation() {return _rotation;};
mathf::vec3 RenderModel::GetScale() {return _scale;};
uint RenderModel::GetPolygonCount() {return _geometry->GetPolygonCount();};
Geometry* RenderModel::GetGeometry() {return _geometry;};
Material* RenderModel::GetMaterial() {return _material;};
Shader* RenderModel::GetShader() {return _shader;};
mathf::mat4x4 RenderModel::GetModelMatrix() {return _model;};