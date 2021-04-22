#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include "Mathf/Mathf.h"

#include "Types.h"
// #include "Render/Render.h"

class Camera {
public:
	Camera(GLRenderer* ren, float FOV, float nearPlane, float farPlane);
	~Camera();
	glm::mat4 GetViewMatrix();
	glm::mat4 GetProjectionMatrix();
	void SetProjectionMatrix(glm::mat4); //? This is a costil, isn't it
	void SetPosition(mathf::vec3);
	void SetRotation(glm::quat);
	mathf::vec3 GetPosition();
	glm::quat GetRotation();
private:
	mathf::vec3 _position;
	glm::quat _rotation;
	glm::mat4 _projectionMatrix;
};