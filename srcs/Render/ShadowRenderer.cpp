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
#include "World/ResourceLoader.h"
#include "Render/Framebuffer.h"
#include "Render/Texture.h"

ShadowRenderer::ShadowRenderer() {
	_shadowFbo = new Framebuffer();
};

void ShadowRenderer::Init(Game* game) {
	_game = game;
	// _shader = new Shader("./resources/Shaders/shadowRenderer.vert", "./resources/Shaders/shadowRenderer.frag");
	_shader = game->GetResources()->GetShader("Shadow Renderer");
	
	_shadowFbo->NewShadow(glm::ivec2(SHADOWMAP_SIDE, SHADOWMAP_SIDE));	
};

ShadowRenderer::~ShadowRenderer() {
	delete _shadowFbo;
}

void ShadowRenderer::Render(std::vector<RenderModel*>& rendered, float sunAngle) {
	//* Prepare gl
	// #ifdef __APPLE__ //? Retina
	// 	glViewport(0, 0, SHADOWMAP_SIDE * 2, SHADOWMAP_SIDE * 2);
	// #else
		glViewport(0, 0, SHADOWMAP_SIDE, SHADOWMAP_SIDE);
	// #endif
	_shadowFbo->Use();
	// glBindFramebuffer(GL_FRAMEBUFFER, _shadowFBO);
	// glClearColor(0.5, 0, 0, 1.0);
	glClear(GL_DEPTH_BUFFER_BIT);
	Player* player = _game->GetPlayer();

	glm::quat rotation = glm::quat(glm::vec3(glm::radians(-sunAngle), 0.f, 0.f));

	glm::vec3 sunDir = glm::vec3(0.f, 0.f, 1.f) * rotation;
	glm::vec3 playerPos = player->GetPosition();

	glm::mat4 view = glm::lookAt(playerPos + sunDir, playerPos, glm::vec3(0.f, 1.f, 0.f));
	glm::mat4 projection = glm::ortho(-128.f, 128.f, -128.f, 128.f, -128.f, 128.f);
	_lightSpace = projection * view;

	_shader->Use();
	_shader->SetMatrix4("lightSpace", _lightSpace);
	for (RenderModel* model : rendered) {
		model->ApplySelf(_shader);
		glDrawArrays(GL_TRIANGLES, 0, model->GetPolygonCount() * 3);
	}
	// glCullFace(GL_BACK);

	// _lightSpace = glm::scale(glm::mat4(1.f), glm::vec3(336.f) / (float)SHADOWMAP_SIDE) * _lightSpace;

	//* Put everything back
	glm::ivec2 winSize = _game->GetRenderer()->GetWindowSize();
	#ifdef __APPLE__ //? Retina fix
		glViewport(0, 0, winSize.x, winSize.y);
	#else
		glViewport(0, 0, winSize.x, winSize.y);
	#endif
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
};

void ShadowRenderer::ApplyMap(Shader* shader) {
	glActiveTexture(TEXTURE_SLOT);
	_shadowFbo->GetDepthTexture()->Use();
	shader->SetInt("shadowMap", TEXTURE_SLOT - GL_TEXTURE0);
};

void ShadowRenderer::ApplyLightSpace(Shader* shader) {
	shader->SetMatrix4("lightSpace", _lightSpace);
};

void ShadowRenderer::ApplySelf(Shader* shader) {
	glActiveTexture(TEXTURE_SLOT);
	_shadowFbo->GetDepthTexture()->Use();
	shader->SetInt("shadowMap", TEXTURE_SLOT - GL_TEXTURE0);
	shader->SetMatrix4("lightSpace", _lightSpace);

};

glm::mat4 ShadowRenderer::GetLightSpace() {return _lightSpace;};
