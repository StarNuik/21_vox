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

void GLRenderer::RenderFrame() {
	ResourceLoader* rs = _game->GetResources();
	UIController* ui = _game->GetUI();
	Skybox* skybox = rs->GetSkybox();

	glm::mat4 view = _activeCamera->GetViewMatrix();
	glm::mat4 projection = _activeCamera->GetProjectionMatrix();
	glm::vec3 cameraPos = _activeCamera->GetPosition();

	// glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	ShadowRenderer* shadows = skybox->GetShadowRenderer();

	shadows->Render(_rendered, _game->GetRuntime());
	// glViewport(0, 0, _width, _height);
	// glViewport(0, 0, _width * 2, _height * 2);
	_framebuffer->Bind();
	//* Clear
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//* Skybox
	glDepthMask(GL_FALSE);
	skybox->Use(_activeCamera, _game->GetDayNightVal(), _game->GetRuntime());
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glDepthMask(GL_TRUE);

	//* Set directional lights
	skybox->SetDirLights(_game->GetDayNightVal(), _game->GetRuntime());

	//* Blocks
	std::sort(_rendered.begin(), _rendered.end());
	Shader* oldShader = nullptr;
	Material* oldMaterial = nullptr;
	for (RenderModel* model : _rendered) {
		//* If shader changed
		Shader* modelShader = model->GetShader();
		if (oldShader != modelShader) {
			modelShader->Use();
			modelShader->SetMatrix4("view", view);
			modelShader->SetMatrix4("projection", projection);
			modelShader->SetFloat3("cameraPos", cameraPos);
			skybox->ApplyDirLights(modelShader);
			shadows->ApplySelf(modelShader);
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
	_framebuffer->Unbind();
	
	// glViewport(0, 0, _width, _height);
	// glViewport(0, 0, _width * 2, _height * 2);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDisable(GL_DEPTH_TEST);
	// shadows->Render(_rendered, _game->GetRuntime());
	Shader* postShader = rs->GetShader("Post Base");
	postShader->Use();
	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	Geometry* quad = rs->GetGeometry("Screen Quad");
	quad->Use();
	Texture* color = _framebuffer->GetColorTexture();
	postShader->SetInt("screenTexture", 0);
	glActiveTexture(GL_TEXTURE0);
	color->Use();
	glDrawArrays(GL_TRIANGLES, 0, quad->GetPolygonCount() * 3);
	glEnable(GL_DEPTH_TEST);

	//* UI
	ui->UpdateData();
	ui->Draw();

	// shadows->Render(_rendered, _game->GetRuntime());

	//* Swap
	glfwSwapBuffers(_window);
};
