#include <vector>
#include <GL/glew.h>

#include "Types.h"
#include "Render/VertexBuffers.h"
#include "Render/Skybox.h"
#include "Render/Geometry.h"
#include "Render/Shader.h"
#include "Render/Cubemap.h"
#include "Render/Camera.h"
#include "Render/DirLight.h"

Skybox::Skybox(Shader* shader, CubeMap* day, CubeMap* night) {
	_shader = shader;
	_cubemap_day = day;
	_cubemap_night = night;
	float *buffer = VertexBuffers::GetBuffer(VertexBuffers::Skybox);
	std::vector<float> vertices = std::vector<float>();
	vertices.reserve(sizeof(float) * 48 * 6);
	vertices.insert(vertices.end(), buffer, buffer + 48 * 6);
	_geometry = new Geometry(vertices);

	_sunLight = new DirLight();
	_moonLight = new DirLight();
	// _sunColor = glm::vec3(SUN_COLOR);
	// _moonColor = glm::vec3(MOON_COLOR);
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

void Skybox::ApplyDirLights(Shader* shader) {
	_sunLight->ApplySelf(shader, 0);
	_moonLight->ApplySelf(shader, 1);
};

void Skybox::SetDirLights(float lerpVal, float runtime) {
	const glm::vec3 forwardLightDir = glm::vec3(0.f, 0.f, 1.f);
	const float currentTime = std::fmod(runtime, SECONDS_IN_A_DAY);
	float sunAngleRad = currentTime / SECONDS_IN_A_DAY * 2 * glm::pi<float>();
	// float moonAngleRad = currentTime / (SECONDS_IN_A_DAY * 30.f) * 2 * glm::pi<float>();

	_sunLight->SetDiffuse(glm::vec3(SUN_DIFFUSE) * lerpVal);
	_sunLight->SetAmbient(glm::vec3(SUN_AMBIENT) * lerpVal);
	glm::quat sunRotation = glm::quat(glm::vec3(sunAngleRad, 0.f, 0.f));
	glm::vec3 sunDir = forwardLightDir * sunRotation;
	_sunLight->SetDirection(glm::normalize(sunDir));

	_moonLight->SetDiffuse(glm::vec3(MOON_DIFFUSE) * (1.f - lerpVal));
	_moonLight->SetAmbient(glm::vec3(MOON_AMBIENT) * (1.f - lerpVal));
	_moonLight->SetDirection(glm::normalize(glm::vec3(0.f, 1.f, 0.f)));
};