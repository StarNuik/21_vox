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

void GLRenderer::RenderFrame() {
	PrepareData();
	_static.shadows->Render(_static.rendered);
	_static.screenFbo->Use();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	_static.skybox->Render();
	RenderBlocks();
	RenderBloom();
	RenderPostprocess();
	_static.ui->UpdateData();
	_static.ui->Draw();
	glfwSwapBuffers(_static.window);
}

void GLRenderer::RenderBlocks() {
	Shader* lastShader = nullptr;
	Material* lastMaterial = nullptr;

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glEnable(GL_CULL_FACE);
	for (RenderModel* model : _static.rendered) {
		Shader* shader = model->GetShader();
		if (shader != lastShader) {
			shader->Use();
			shader->SetFloat3("cameraPos", _frame.cameraPos);
			_static.skybox->ApplyDirLights(shader);
			_static.shadows->ApplySelf(shader);
			lastShader = shader;
		}
		//* If material changed
		Material* material = model->GetMaterial();
		if (material != lastMaterial) {
			material->Use(shader);
			lastMaterial = material;
		}
		model->GetGeometry()->Use();
		shader->SetMatrix4("mvp", _frame.vp * model->GetModelMatrix());
		shader->SetMatrix4("model", model->GetModelMatrix());
		glDrawArrays(GL_TRIANGLES, 0, model->GetPolygonCount() * 3);
	}
}

void GLRenderer::RenderBloom() {
	Texture* color;
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glDisable(GL_BLEND);
	
	//* Copy bright texture to bloom
	glViewport(0, 0, _static.windowSize.x / 4, _static.windowSize.y / 4);
	_static.bloomFbo->Bind();
	glDrawBuffer(GL_COLOR_ATTACHMENT0);
	_static.postQuad->Use();
	_static.rs->GetShader("Post None")->Use();
	glActiveTexture(GL_TEXTURE0);
	color = _static.screenFbo->GetColorTexture(1);
	color->Use();
	glDrawArrays(GL_TRIANGLES, 0, _static.postQuad->GetPolygonCount() * 3);

	_static.bloomShader->Use();
	for (int i = 0; i < 6; i++) {
		int one = i % 2;
		_static.bloomFbo->Bind();
		glDrawBuffer(GL_COLOR_ATTACHMENT1 - one);
		color = _static.bloomFbo->GetColorTexture(one);
		color->Use();
		glDrawArrays(GL_TRIANGLES, 0, _static.postQuad->GetPolygonCount() * 3);
	}
	glViewport(0, 0, _static.windowSize.x, _static.windowSize.y);
	//* Resulting texture is ATTACHMENT0
}

void GLRenderer::RenderPostprocess() {
	Texture* color;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glDisable(GL_BLEND);
	_static.postShader->Use();
	_static.postShader->SetFloat("runtime", _static.game->GetRuntime());
	_static.postQuad->Use();
	color = _static.screenFbo->GetColorTexture();
	glActiveTexture(GL_TEXTURE0);
	color->Use();
	color = _static.bloomFbo->GetColorTexture();
	glActiveTexture(GL_TEXTURE1);
	color->Use();
	glDrawArrays(GL_TRIANGLES, 0, _static.postQuad->GetPolygonCount() * 3);
}

void GLRenderer::PrepareData() {
	_frame.view = _static.activeCamera->GetViewMatrix();
	_frame.projection = _static.activeCamera->GetProjectionMatrix();
	_frame.vp = _frame.projection * _frame.view;
	_frame.cameraPos = _static.activeCamera->GetPosition();
	_static.shadows->PrepareData(_static.game->GetSunAngle());
	_static.skybox->PrepareData(_static.game->GetSunAngle(), _static.game->GetMoonAngle(), _static.game->GetSunVal(), _static.game->GetMoonVal());
	// _static.skybox->PrepareData(45, 0, 1, 0);
	// _static.skybox->PrepareData(30, 10, 1, 0);
	std::sort(_static.rendered.begin(), _static.rendered.end(), RenderModelLess);
}
