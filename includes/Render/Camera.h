#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

class Camera {
public:
	Camera(GLRenderer *, float FOV, float nearPlane, float farPlane);
	~Camera();
	glm::mat4 GetViewMatrix();
	glm::mat4 GetProjectionMatrix();
	glm::vec3 GetPosition();
	glm::quat GetRotation();
	void SetPosition(glm::vec3);
	void SetRotation(glm::quat);
private:
	glm::vec3 _position;
	glm::quat _rotation;
	glm::mat4 _projectionMatrix;
};