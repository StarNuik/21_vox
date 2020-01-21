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

void GLRenderer::RenderFrame() {
	ResourceLoader* r = _game->GetResources();
	UIController* ui = _game->GetUI();
	Skybox* skybox = r->GetSkybox();
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
	for (RenderModel* model : _rendered) {
		Shader* modelShader = model->Use(_activeCamera);
		// shadows->ApplySelf(modelShader);
		skybox->ApplyDirLights(modelShader);
		glDrawArrays(GL_TRIANGLES, 0, model->GetPolygonCount() * 3);
	}

	//* UI
	ui->UpdateData();
	ui->Draw();

	//* Swap
	glfwSwapBuffers(_window);
};
