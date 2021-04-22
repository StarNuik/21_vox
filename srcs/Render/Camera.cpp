#include "Render/GLRenderer.h"
#include "Render/Camera.h"

Camera::Camera(GLRenderer* renderer, float fov, float nearPlane, float farPlane) {
	_position = mathf::vec3(0.f);
	_rotation = glm::identity<glm::quat>();
	if (!renderer)
		return;
	mathf::ivec2 winSize = renderer->GetWindowSize();
	_projectionMatrix = glm::perspective(glm::radians(fov), (float)winSize.x / (float)winSize.y, nearPlane, farPlane);
};

Camera::~Camera() {};

glm::mat4 Camera::GetViewMatrix() {
	mathf::vec3 forward = mathf::vec3(glm::mat4_cast(_rotation) * mathf::vec4(0.f, 0.f, -1.f, 0.f).to_glm());
	return glm::lookAt(_position.to_glm(), (_position + forward).to_glm(), mathf::vec3(0.f, 1.f, 0.f).to_glm());
}

glm::mat4 Camera::GetProjectionMatrix() {
	return _projectionMatrix;
}

void Camera::SetPosition(mathf::vec3 position) {_position = position;};
void Camera::SetRotation(glm::quat rotation) {_rotation = rotation;};
void Camera::SetProjectionMatrix(glm::mat4 projection) {_projectionMatrix = projection;};
mathf::vec3 Camera::GetPosition() {return _position;};
glm::quat Camera::GetRotation() {return _rotation;};