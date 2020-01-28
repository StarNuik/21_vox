#define GLEW_STATIC
#include <GL/glew.h>
#include "Render/GLRenderer.h"
#include "Render/Shader.h"
#include "Render/RenderModel.h"
#include "Render/Skybox.h"
#include "Render/ShadowRenderer.h"
#include "World/ResourceLoader.h"
#include "Utilities/Log.h"
#include "Engine/Game.h"
#include "UI/UIController.h"
#include "Render/Camera.h"
#include "Render/Material.h"
#include "Render/Geometry.h"
#include "Render/Texture.h"
#include "Render/Framebuffer.h"
#include <algorithm>

// void GLRenderer::RenderFrame() {
// 	PrepareData();
// 	RenderSkybox();
// 	RenderShadows();
// 	RenderBlocks();
// 	RenderPostprocess();
// 	RenderUI();
// }

void GLRenderer::PrepareData() {
	_frame.view = _static.activeCamera->GetViewMatrix();
	_frame.projection = _static.activeCamera->GetProjectionMatrix();
	_frame.cameraPos = _static.activeCamera->GetPosition();
	// RenderModel& a = new RenderModel();
}

void GLRenderer::RenderFrame() {
	ResourceLoader* rs = _static.game->GetResources();
	UIController* ui = _static.game->GetUI();
	// Skybox* skybox = rs->GetSkybox();

	glm::mat4 view = _static.activeCamera->GetViewMatrix();
	glm::mat4 projection = _static.activeCamera->GetProjectionMatrix();
	glm::vec3 cameraPos = _static.activeCamera->GetPosition();
	float currentTime = std::fmod(_static.game->GetRuntime(), SECONDS_IN_A_DAY);
	float angle = -currentTime / SECONDS_IN_A_DAY * 360.f;

	// glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	// ShadowRenderer* shadows = _static.skybox->GetShadowRenderer();

	_static.shadows->Render(_static.rendered, angle);
	// glViewport(0, 0, _width, _height);
	// glViewport(0, 0, _width * 2, _height * 2);
	_static.framebuffer->Bind();
	//* Clear
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//* Skybox
	// glDepthMask(GL_FALSE);
	_static.skybox->PrepareData(angle, 30.f, _static.game->GetDayNightVal(), 0.5f);
	_static.skybox->Render();
	// skybox->Use(_static.activeCamera, _static.game->GetDayNightVal(), _static.game->GetRuntime());
	// glDrawArrays(GL_TRIANGLES, 0, 36);
	// glDepthMask(GL_TRUE);

	//* Set directional lights
	// skybox->SetDirLights(_static.game->GetDayNightVal(), _static.game->GetRuntime());

	//* Blocks
	std::sort(_static.rendered.begin(), _static.rendered.end());
	Shader* oldShader = nullptr;
	Material* oldMaterial = nullptr;
	for (RenderModel* model : _static.rendered) {
		//* If shader changed
		Shader* modelShader = model->GetShader();
		if (oldShader != modelShader) {
			modelShader->Use();
			modelShader->SetMatrix4("view", view);
			modelShader->SetMatrix4("projection", projection);
			modelShader->SetFloat3("cameraPos", cameraPos);
			_static.skybox->ApplyDirLights(modelShader);
			_static.shadows->ApplySelf(modelShader);
			oldShader = modelShader;
		}
		//* If material changed
		Material* modelMaterial = model->GetMaterial();
		if (oldMaterial != modelMaterial) {
			modelMaterial->Use(modelShader);
			oldMaterial = modelMaterial;
		}
		model->ApplySelf(modelShader);
		glDrawArrays(GL_TRIANGLES, 0, model->GetPolygonCount() * 3);
	}
	_static.framebuffer->Unbind();
	
	// glViewport(0, 0, _static.windowSize.x, _static.windowSize.y);
	// glViewport(0, 0, _width * 2, _height * 2);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDisable(GL_DEPTH_TEST);
	// shadows->Render(_static.rendered, _static.game->GetRuntime());
	Shader* postShader = rs->GetShader("Post Base");
	postShader->Use();
	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	Geometry* quad = rs->GetGeometry("Screen Quad");
	quad->Use();
	Texture* color = _static.framebuffer->GetColorTexture();
	postShader->SetInt("screenTexture", 0);
	glActiveTexture(GL_TEXTURE0);
	color->Use();
	glDrawArrays(GL_TRIANGLES, 0, quad->GetPolygonCount() * 3);
	glEnable(GL_DEPTH_TEST);

	//* UI
	ui->UpdateData();
	ui->Draw();

	// shadows->Render(_static.rendered, _static.game->GetRuntime());

	//* Swap
	glfwSwapBuffers(_static.window);
};
