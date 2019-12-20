#include <vector>
#include <GL/glew.h>

#include "Types.h"
#include "Render/VertexBuffers.h"
#include "Render/Skybox.h"
#include "Render/Geometry.h"
#include "Render/Shader.h"
#include "Render/Cubemap.h"
#include "Render/Camera.h"

Skybox::Skybox(Shader* shader, CubeMap* day, CubeMap* night) {
	_shader = shader;
	_cubemap_day = day;
	_cubemap_night = night;
	float *buffer = VertexBuffers::GetBuffer(VertexBuffers::Skybox);
	std::vector<float> vertices = std::vector<float>();
	vertices.reserve(sizeof(float) * 48 * 6);
	vertices.insert(vertices.end(), buffer, buffer + 48 * 6);
	_geometry = new Geometry(vertices);
};

Skybox::~Skybox() {
	delete _geometry;
};

Shader* Skybox::Use(Camera* camera, float lerpVal, float runtime) {
	glm::mat4 view = glm::mat4(glm::mat3(camera->GetViewMatrix()));
	float currentTime = std::fmod(runtime, SECONDS_IN_A_DAY);
	float angleRadians = currentTime / SECONDS_IN_A_DAY * 2 * glm::pi<float>();
	glm::quat rotation = glm::quat(glm::vec3(angleRadians, 0.f, 0.f));
	glm::mat4 model = glm::identity<glm::mat4>() * glm::mat4_cast(rotation);

	_shader->Use();
	_geometry->Use();
	glActiveTexture(GL_TEXTURE0);
	_cubemap_day->Use();
	_shader->SetInt("day", 0);
	glActiveTexture(GL_TEXTURE1);
	_cubemap_night->Use();
	_shader->SetInt("night", 1);
	_shader->SetMatrix4("model", model);
	_shader->SetMatrix4("view", view);
	_shader->SetMatrix4("projection", camera->GetProjectionMatrix());
	_shader->SetFloat("lerpVal", lerpVal);
	return _shader;
};