#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include "Types.h"

class RenderModel {
public:
	RenderModel(GLRenderer*, Shader*, Material*, Geometry*);
	~RenderModel();
	Shader* Use(Camera* camera);
	void ApplySelf(Camera*, Shader*);
	void SetPosition(glm::vec3);
	void SetRotation(glm::quat);
	void SetScale(glm::vec3);
	glm::vec3 GetPosition();
	glm::quat GetRotation();
	glm::vec3 GetScale();
	glm::mat4 GetAphineMatrix();
	uint GetPolygonCount();
	Geometry* GetGeometry();
private:
	// void Init(Shader*, Texture*, Texture*, Geometry*);
	Shader* _shader;
	Material* _material;
	Geometry* _geometry;
	glm::vec3 _position, _scale;
	glm::quat _rotation;
	GLRenderer* _renderer;
};
