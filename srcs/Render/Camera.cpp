#include "Render/GLRenderer.h"
#include "Render/Camera.h"

Camera::Camera(GLRenderer* renderer, float fov, float nearPlane, float farPlane) {
	_fov = fov;
	_position = glm::vec3(0.f);
	_rotation = glm::identity<glm::quat>();
	_nearPlane = nearPlane;
	_farPlane = farPlane;
	// _pointTestCache = 2 * tanf(fov * 0.5f);
	if (!renderer)
		return;
	glm::ivec2 winSize = renderer->GetWindowSize();
	_ratio = (float)winSize.x / (float)winSize.y;
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

bool Camera::IsInView(const glm::mat4& vp, const glm::vec3& point) {
	glm::vec3 projected = glm::vec3(vp * glm::vec4(point, 1.f));
	// float w = projected.z * _pointTestCache;
	if (projected.z < _nearPlane or projected.z > _farPlane)
		return false;
	float w = projected.z * 2 * tanf(_fov * 0.5f);
	float h = w / _ratio;
	w *= 0.5;
	h *= 0.5;
	if (projected.x < -w or projected.x > w)
		return false;
	if (projected.y < -h or projected.y > h)
		return false;
	return true;
};

void Camera::SetPosition(glm::vec3 position) {_position = position;};
void Camera::SetRotation(glm::quat rotation) {_rotation = rotation;};
void Camera::SetProjectionMatrix(glm::mat4 projection) {_projectionMatrix = projection;};
glm::vec3 Camera::GetPosition() {return _position;};
glm::quat Camera::GetRotation() {return _rotation;};
float Camera::GetFOV() {return _fov;};
float Camera::GetRatio() {return _ratio;};