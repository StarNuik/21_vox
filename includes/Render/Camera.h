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
	mathf::mat4x4 GetViewMatrix();
	mathf::mat4x4 GetProjectionMatrix();
	void SetProjectionMatrix(mathf::mat4x4); //? This is a costil, isn't it
	void SetPosition(mathf::vec3);
	void SetRotation(mathf::quat);
	mathf::vec3 GetPosition();
	mathf::quat GetRotation();
private:
	mathf::vec3 _position;
	mathf::quat _rotation;
	mathf::mat4x4 _projectionMatrix;
};