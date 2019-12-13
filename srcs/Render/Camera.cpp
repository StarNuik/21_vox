#include "Render/GLRenderer.h"
#include "Render/Camera.h"

Camera::Camera(GLRenderer* renderer, float fov, float nearPlane, float farPlane) {
	glm::ivec2 winSize = renderer->GetWindowSize();
	_position = glm::vec3(0.f);
	_rotation = glm::identity<glm::quat>();
	_projectionMatrix = glm::perspective(glm::radians(fov), (float)winSize.x / (float)winSize.y, nearPlane, farPlane);
};

Camera::~Camera() {};

glm::mat4 Camera::GetViewMatrix() {
	glm::vec3 forward = glm::mat4_cast(_rotation) * glm::vec4(0.f, 0.f, -1.f, 0.f);
	return glm::lookAt(_position, _position + forward, glm::vec3(0.f, 1.f, 0.f));
}

glm::mat4 Camera::GetProjectionMatrix() {
	return _projectionMatrix;
}

void Camera::SetPosition(glm::vec3 position) {_position = position;};
void Camera::SetRotation(glm::quat rotation) {_rotation = rotation;};
glm::vec3 Camera::GetPosition() {return _position;};
glm::quat Camera::GetRotation() {return _rotation;};