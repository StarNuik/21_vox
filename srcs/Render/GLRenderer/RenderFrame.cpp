#define GLEW_STATIC
#include <GL/glew.h>
#include "Render/GLRenderer.h"
#include "Render/Shader.h"
#include "Render/RenderModel.h"
#include "Render/Skybox.h"
// #include "Render/ShadowRenderer.h"
#include "World/ResourceLoader.h"
#include "Utilities/Log.h"
#include "Engine/Game.h"
#include "UI/UIController.h"
#include "Render/Camera.h"
#include "Render/Material.h"

void GLRenderer::RenderFrame() {
	ResourceLoader* r = _game->GetResources();
	UIController* ui = _game->GetUI();
	Skybox* skybox = r->GetSkybox();

	glm::mat4 view = _activeCamera->GetViewMatrix();
	glm::mat4 projection = _activeCamera->GetProjectionMatrix();
	glm::vec3 cameraPos = _activeCamera->GetPosition();

	// ShadowRenderer* shadows = skybox->GetShadowRenderer();

	// shadows->Render(_rendered, _game->GetRuntime());

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
		Shader* modelShader = model->Use(_activeCamera);
		if (oldShader != modelShader) {
			modelShader->SetMatrix4("view", view);
			modelShader->SetMatrix4("projection", projection);
			modelShader->SetFloat3("cameraPos", cameraPos);
			skybox->ApplyDirLights(modelShader);
			oldShader = modelShader;
		}
		//* If material changed
		Material* modelMaterial = model->GetMaterial();
		if (oldMaterial != modelMaterial) {
			modelMaterial->Use(modelShader);
			oldMaterial = modelMaterial;
		}
		// shadows->ApplySelf(modelShader);
		glDrawArrays(GL_TRIANGLES, 0, model->GetPolygonCount() * 3);
	}

	//* UI
	ui->UpdateData();
	ui->Draw();

	//* Swap
	glfwSwapBuffers(_window);
};
