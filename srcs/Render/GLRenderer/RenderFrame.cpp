#include "Render/GLRenderer.h"
#include "Render/Shader.h"
#include "Render/RenderModel.h"
#include "Render/Skybox.h"
#include "World/ResourceLoader.h"
#include "Utilities/Locator.hpp"
#include "Engine/Game.h"
#include "UI/UIController.h"

void GLRenderer::RenderFrame() {
	ResourceLoader* r = _game->GetResources();
	UIController* ui = _game->GetUI();

	glClearColor(0.4f, 0.4f, 0.65f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	Skybox* skybox = r->GetSkybox();
	glDepthMask(GL_FALSE);
	skybox->Use(_activeCamera, _game->GetDayNightVal());
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glDepthMask(GL_TRUE);

	for (int i = 0; i < _rendered.size(); i++) {
		RenderModel* model = _rendered[i];
		Shader* modelShader = model->Use(_activeCamera);
		modelShader->SetInt("tick", _tick);
		glDrawArrays(GL_TRIANGLES, 0, model->GetPolygonCount() * 3);
	}
	ui->UpdateData();
	ui->Draw();
	glfwSwapBuffers(_window);
	_tick++;
};
