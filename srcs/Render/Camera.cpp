#include "Render/GLRenderer.h"
#include "Render/Camera.h"

Camera::Camera(GLRenderer* renderer, float fov, float nearPlane, float farPlane) {
	_position = mathf::vec3(0.f);
	_rotation = mathf::quat::identity();
	if (!renderer)
		return;
	mathf::ivec2 winSize = renderer->GetWindowSize();
	_projectionMatrix = glm::perspective(glm::radians(fov), (float)winSize.x / (float)winSize.y, nearPlane, farPlane);
};

Camera::~Camera() {};

mathf::mat4x4 Camera::GetViewMatrix() {
	mathf::vec3 forward = mathf::vec3(mathf::mat4x4::cast(_rotation) * mathf::vec4(0.f, 0.f, -1.f, 0.f));
	return mathf::mat4x4::look_at(_position, (_position + forward), mathf::vec3(0.f, 1.f, 0.f));
}

mathf::mat4x4 Camera::GetProjectionMatrix() {
	return _projectionMatrix;
}

void Camera::SetPosition(mathf::vec3 position) {_position = position;};
void Camera::SetRotation(mathf::quat rotation) {_rotation = rotation;};
void Camera::SetProjectionMatrix(mathf::mat4x4 projection) {_projectionMatrix = projection;};
mathf::vec3 Camera::GetPosition() {return _position;};
mathf::quat Camera::GetRotation() {return _rotation;};