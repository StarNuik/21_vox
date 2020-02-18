#include <vector>
#include <GL/glew.h>

#include "Types.h"
#include "Engine/Game.h"
#include "World/ResourceLoader.h"
#include "Render/VertexBuffers.h"
#include "Render/Skybox.h"
#include "Render/Geometry.h"
#include "Render/Shader.h"
#include "Render/Cubemap.h"
#include "Render/ShadowRenderer.h"
#include "Render/Camera.h"
#include "Render/DirLight.h"
#include "Render/RenderModel.h"
#include "Render/Material.h"
#include "World/Block.h"

//! Old constructor
// Skybox::Skybox(Game* game, Shader* shader, CubeMap* day, CubeMap* night) {
// 	_shader = shader;
// 	_cubemap_day = day;
// 	_cubemap_night = night;
// 	float *buffer = VertexBuffers::GetBuffer(VertexBuffers::Skybox);
// 	std::vector<float> vertices = std::vector<float>();
// 	vertices.reserve(sizeof(float) * 48 * 6);
// 	vertices.insert(vertices.end(), buffer, buffer + 48 * 6);
// 	_geometry = new Geometry(vertices);

// 	_sunLight = new DirLight();
// 	_moonLight = new DirLight();
// 	_shadows = new ShadowRenderer(game);
// 	// _lastSunVal = 0.f;
// 	// _lastMoonVal = 0.f;
// 	_moonVal = 0.f;
// 	_sunVal = 0.f;
// 	_easyMoonApply = false;
// 	_easySunApply = false;
// 	// _sunColor = glm::vec3(SUN_COLOR);
// 	// _moonColor = glm::vec3(MOON_COLOR);
// };

Skybox::Skybox() {
	// _shadows = new ShadowRenderer();
	_sunLight = new DirLight();
	_moonLight = new DirLight();
	// _geometry = nullptr;
	_shader = nullptr;
	_dayCubemap = nullptr;
	_nightCubemap = nullptr;
	_skyGeometry = nullptr;
}

Skybox::~Skybox() {
	// delete _shadows;
	delete _skyGeometry;
	delete _sunLight;
	delete _moonLight;
};

void Skybox::Init(Game* game) {
	_game = game;
	ResourceLoader* rs = _game->GetResources();

	float *buffer = VertexBuffers::GetBuffer(VertexBuffers::Skybox);
	std::vector<float> vertices = std::vector<float>();
	vertices.reserve(sizeof(float) * 66 * 6);
	vertices.insert(vertices.end(), buffer, buffer + 66 * 6);
	_skyGeometry = new Geometry(vertices);

	_shader = rs->GetShader("Skybox");
	_dayCubemap = rs->GetCubeMap("Day");
	_nightCubemap = rs->GetCubeMap("Night");
	_sunModel = new RenderModel(nullptr, rs->GetShader("Skybox Sun Bloom"), rs->GetMaterial(Block::Sun), rs->GetGeometry("Sun"));
	_moonModel = new RenderModel(nullptr, rs->GetShader("Skybox Sun Bloom"), rs->GetMaterial(Block::Moon), rs->GetGeometry("Sun"));

	//* Pre-set static shader values
	_shader->Use();
	glActiveTexture(GL_TEXTURE0);
	_dayCubemap->Use();
	_shader->SetInt("day", 0);
	glActiveTexture(GL_TEXTURE1);
	_nightCubemap->Use();
	_shader->SetInt("night", 1);

	// _shadows->Init(game);
}

void Skybox::SetActiveCamera(Camera* camera) {
	_activeCamera = camera;
	_projection = camera->GetProjectionMatrix();
	// _shader->Use();
	// _shader->SetMatrix4("projection", camera->GetProjectionMatrix());
};

void Skybox::PrepareData(float sunAngle, float moonAngle, float sunVal, float moonVal) {
	_easySunApply = _sunVal == sunVal ? true : false;
	_easyMoonApply = _moonVal == moonVal ? true : false;
	_sunVal = sunVal;
	_moonVal = moonVal;
	glm::mat4 view = glm::mat4(glm::mat3(_activeCamera->GetViewMatrix()));
	glm::quat sunRotation = glm::quat(glm::vec3(glm::radians(-sunAngle), 0.f, 0.f));
	glm::quat moonRotation = glm::quat(glm::vec3(glm::radians(-moonAngle), 27.498f, 0.f));
	glm::mat4 skyModel = glm::mat4(1.f) * glm::mat4_cast(sunRotation);
	glm::mat4 moonModel = glm::mat4(1.f) * glm::mat4_cast(moonRotation);
	_mvpSky = _projection * view * skyModel;
	_mvpMoon = _projection * view * moonModel;

	_sunLight->SetDiffuse(glm::vec3(SUN_DIFFUSE) * sunVal);
	_sunLight->SetAmbient(glm::vec3(SUN_AMBIENT) * sunVal);
	glm::vec3 sunDir = glm::vec3(0.f, 0.f, 1.f) * glm::quat(glm::vec3(glm::radians(sunAngle), 0.f, 0.f));
	_sunLight->SetDirection(glm::normalize(sunDir));

	_moonLight->SetDiffuse(glm::vec3(MOON_DIFFUSE) * moonVal);
	_moonLight->SetAmbient(glm::vec3(MOON_AMBIENT) * moonVal);
	glm::vec3 moonDir = moonModel * glm::vec4(0.f, 0.f, 1.f, 0.f);
	_moonLight->SetDirection(glm::normalize(moonDir));
}

void Skybox::Render() {
	_shader->Use();
	_skyGeometry->Use();
	_shader->SetMatrix4("mvp", _mvpSky);
	_shader->SetFloat("sunVal", _sunVal);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	//! Draw sun and moon here later
	Shader* shader = _sunModel->GetShader();
	shader->Use();
	_sunModel->GetMaterial()->Use(shader);
	_sunModel->GetGeometry()->Use();

	shader->SetMatrix4("mvp", _mvpSky);
	glDrawArrays(GL_TRIANGLES, 0, _sunModel->GetPolygonCount() * 3);
	_moonModel->GetMaterial()->Use(shader);
	shader->SetMatrix4("mvp", _mvpMoon);
	glDrawArrays(GL_TRIANGLES, 0, _sunModel->GetPolygonCount() * 3);
}

// Shader* Skybox::Use(Camera* camera, float lerpVal, float runtime) {
// 	// glm::mat4 view = glm::mat4(glm::mat3(camera->GetViewMatrix()));
// 	// float currentTime = std::fmod(runtime, SECONDS_IN_A_DAY);
// 	// float angle = currentTime / SECONDS_IN_A_DAY * 360.f;
// 	// glm::quat rotation = glm::quat(glm::vec3(glm::radians(angle), 0.f, 0.f));
// 	// glm::mat4 model = glm::identity<glm::mat4>() * glm::mat4_cast(rotation);

// 	_shader->Use();
// 	_geometry->Use();
// 	_shader->SetMatrix4("model", model);
// 	_shader->SetMatrix4("view", view);
// 	//? Projection is set on camera change
// 	_shader->SetFloat("lerpVal", lerpVal);
// 	return _shader;
// };

void Skybox::ApplyDirLights(Shader* shader) {
	_sunLight->ApplySelf(shader, 0);
	_moonLight->ApplySelf(shader, 1);
};

// void Skybox::SetDirLights(float lerpVal, float runtime) {
// 	const glm::vec3 forwardLightDir = glm::vec3(0.f, 0.f, 1.f);
// 	const float currentTime = std::fmod(runtime, SECONDS_IN_A_DAY);
// 	float sunAngleRad = currentTime / SECONDS_IN_A_DAY * 2 * glm::pi<float>();
// 	// float moonAngleRad = currentTime / (SECONDS_IN_A_DAY * 30.f) * 2 * glm::pi<float>();

// 	_sunLight->SetDiffuse(glm::vec3(SUN_DIFFUSE) * lerpVal);
// 	_sunLight->SetAmbient(glm::vec3(SUN_AMBIENT) * lerpVal);
// 	glm::quat sunRotation = glm::quat(glm::vec3(sunAngleRad, 0.f, 0.f));
// 	glm::vec3 sunDir = forwardLightDir * sunRotation;
// 	_sunLight->SetDirection(glm::normalize(sunDir));

// 	_moonLight->SetDiffuse(glm::vec3(MOON_DIFFUSE) * (1.f - lerpVal));
// 	_moonLight->SetAmbient(glm::vec3(MOON_AMBIENT) * (1.f - lerpVal));
// 	_moonLight->SetDirection(glm::normalize(glm::vec3(0.f, 1.f, 0.f)));
// };

// ShadowRenderer* Skybox::GetShadowRenderer() {return _shadows;};
