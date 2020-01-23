#include <GL/glew.h>

#include "Types.h"
#include "Render/ShadowRenderer.h"
#include "Render/RenderModel.h"
#include "Render/Camera.h"
#include "Render/GLRenderer.h"
#include "Render/Shader.h"
#include "Engine/Game.h"
#include "World/ResourceLoader.h"
#include "Player/Player.h"

ShadowRenderer::ShadowRenderer(Game* game) {
	_game = game;
	_shader = new Shader("./resources/Shaders/shadowRenderer.vert", "./resources/Shaders/shadowRenderer.frag");
	_camera = new Camera(nullptr, 0.f, 0.f, 0.f);
	_camera->SetProjectionMatrix(glm::ortho(-200.f, 200.f, -200.f, 200.f, -200.f, 200.f));

	glGenFramebuffers(1, &_shadowFBO);
	
	// uint tex[2];
	// glGenTextures(2, tex);
	// for (int i = 0; i < 2; i++) {
		glGenTextures(1, &_sunMap);
		glBindTexture(GL_TEXTURE_2D, _sunMap);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOWMAP_SIDE, SHADOWMAP_SIDE, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); 
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	// }
	// _sunMap = tex[0];
	// _moonMap = tex[1];

	glBindFramebuffer(GL_FRAMEBUFFER, _shadowFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, _sunMap, 0);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, _sunMap, 0);
	glDrawBuffer(GL_NONE);
	// glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
};

ShadowRenderer::~ShadowRenderer() {
	glDeleteTextures(1, &_sunMap);
	glDeleteTextures(1, &_moonMap);
	glDeleteFramebuffers(1, &_shadowFBO);
}

void ShadowRenderer::Render(std::vector<RenderModel*>& rendered, float runtime) {
	//* Prepare gl
	#ifdef __APPLE__ //? Retina
		glViewport(0, 0, SHADOWMAP_SIDE * 2, SHADOWMAP_SIDE * 2);
	#else
		glViewport(0, 0, SHADOWMAP_SIDE, SHADOWMAP_SIDE);
	#endif
	glBindFramebuffer(GL_FRAMEBUFFER, _shadowFBO);
	glClear(GL_DEPTH_BUFFER_BIT);

	glm::vec3 pPos = _game->GetPlayer()->GetPosition();
	_camera->SetPosition(glm::vec3(pPos.x, 256.f, pPos.z));
	float currentTime = std::fmod(runtime, SECONDS_IN_A_DAY);
	float angleRadians = currentTime / SECONDS_IN_A_DAY * 2 * glm::pi<float>();
	glm::quat rotation = glm::quat(glm::vec3(angleRadians, 0.f, 0.f));
	_camera->SetRotation(rotation);
	_lightSpace = _camera->GetProjectionMatrix() * _camera->GetViewMatrix();

	for (RenderModel* model : rendered) {
		_shader->SetMatrix4("lightSpace", _lightSpace);
		model->ApplySelf(_camera, _shader);
		glDrawArrays(GL_TRIANGLES, 0, model->GetPolygonCount() * 3);
	}

	//* Put everything back
	glm::ivec2 winSize = _game->GetRenderer()->GetWindowSize();
	#ifdef __APPLE__ //? Retina fix
		glViewport(0, 0, winSize.x * 2, winSize.y * 2);
	#else
		glViewport(0, 0, winSize.x, winSize.y);
	#endif
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
};

void ShadowRenderer::ApplySelf(Shader* shader) {
	glActiveTexture(TEXTURE_SLOT);
	glBindTexture(GL_TEXTURE_2D, _sunMap);
	shader->SetInt("shadowMap", TEXTURE_SLOT - GL_TEXTURE0);
	shader->SetMatrix4("lightSpace", _lightSpace);

};

glm::mat4 ShadowRenderer::GetLightSpace() {return _lightSpace;};
