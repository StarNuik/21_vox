#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include "Types.h"

bool RenderModelLess(RenderModel*& x, RenderModel*& y);

class RenderModel {
public:
	RenderModel(GLRenderer*, Shader*, Material*, Geometry*);
	~RenderModel();
	Shader* Use(Camera* camera);
	void ApplySelf(Camera*, Shader*);
	void ApplySelf(Shader*);
	void SetPosition(glm::vec3);
	void SetRotation(glm::quat);
	void SetScale(glm::vec3);
	void SetModelMatrix(glm::mat4);
	glm::vec3 GetPosition();
	glm::quat GetRotation();
	glm::vec3 GetScale();
	// glm::mat4 GetAphineMatrix();
	void RecalculateModelMatrix();
	uint GetPolygonCount();
	Geometry* GetGeometry();
	Material* GetMaterial();
	Shader* GetShader();
	glm::mat4 GetModelMatrix();
	bool operator<(const RenderModel&) const;
	uint _shaderId;
	uint _materialId;
private:
	// void Init(Shader*, Texture*, Texture*, Geometry*);
	Shader* _shader;
	Material* _material;
	Geometry* _geometry;
	glm::vec3 _position, _scale;
	glm::quat _rotation;
	GLRenderer* _renderer;
	uint _geometryId;
	glm::mat4 _model;
};
