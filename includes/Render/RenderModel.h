#pragma once

#include "Mathf/Mathf.h"

#include "Mathf/Mathf.h"

#include "Types.h"

bool RenderModelLess(RenderModel*& x, RenderModel*& y);

class RenderModel {
public:
	RenderModel(GLRenderer*, Shader*, Material*, Geometry*);
	~RenderModel();
	Shader* Use(Camera* camera);
	void ApplySelf(Camera*, Shader*);
	void ApplySelf(Shader*);
	void SetPosition(mathf::vec3);
	void SetRotation(mathf::quat);
	void SetScale(mathf::vec3);
	void SetModelMatrix(mathf::mat4x4);
	mathf::vec3 GetPosition();
	mathf::quat GetRotation();
	mathf::vec3 GetScale();
	// mathf::mat4x4 GetAphineMatrix();
	void RecalculateModelMatrix();
	uint GetPolygonCount();
	Geometry* GetGeometry();
	Material* GetMaterial();
	Shader* GetShader();
	mathf::mat4x4 GetModelMatrix();
	bool operator<(const RenderModel&) const;
	uint _shaderId;
	uint _materialId;
private:
	// void Init(Shader*, Texture*, Texture*, Geometry*);
	Shader* _shader;
	Material* _material;
	Geometry* _geometry;
	mathf::vec3 _position, _scale;
	mathf::quat _rotation;
	GLRenderer* _renderer;
	uint _geometryId;
	mathf::mat4x4 _model;
};
