#include "Render/Render.h"
#include "Engine/Locator.hpp"

void GLRenderer::RenderFrame() {
	// egl::gui.frameStart();
	// egl::gui.draw(scene);

	// glClearColor(0.4f, 0.4f, 0.65f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Shader inUseShader;
	// if (gui.wireframe)
	// 	inUseShader = textureShader;
	// else
	// 	inUseShader = phongShader;
	// inUseShader.use();
	// scene.camera.use();
	// Light &currentLight = scene.lightGeometry.front();
	// inUseShader.setFloat3("lightPos", currentLight.position);
	// inUseShader.setFloat3("lightColor", currentLight.color);
	// inUseShader.setFloat3("viewPos", scene.camera.position);
	// for (uint i = 0; i < scene.geometry.size(); i++)
	// {
	// 	Model	current = scene.geometry[i];
	// 	current.testCallback(current, tick);
	// 	current.use(inUseShader, scene.camera);
	// 	glDrawArrays(GL_TRIANGLES, 0, current.polygonCount * 3);
	// }
	// lightSourceShader.use();
	// // (void)lightShader;
	// for (uint i = 0; i < scene.lightGeometry.size(); i++)
	// {
	// 	Light	current = scene.lightGeometry[i];
	// 	current.use(lightSourceShader, scene.camera);
	// 	glDrawArrays(GL_TRIANGLES, 0, current.polygonCount * 3);
	// }

	// egl::gui.frameEnd();
	// eventRouter(this);
	glfwSwapBuffers(_window);
	// tick++;

};
