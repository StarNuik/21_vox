#include "Render/Render.h"
#include "Engine/Locator.hpp"

void GLRenderer::RenderFrame() {
	glClearColor(0.4f, 0.4f, 0.65f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	for (int i = 0; i < _rendered.size(); i++) {
		RenderModel* model = _rendered[i];
		Shader* modelShader = model->Use(_activeCamera);
		glDrawArrays(GL_TRIANGLES, 0, model->GetPolygonCount() * 3);
	}
	_game->GetUI()->Draw();
	glfwSwapBuffers(_window);
};
