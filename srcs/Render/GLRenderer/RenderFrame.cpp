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
#include "Utilities/Profiler.h"
#include <algorithm>

void GLRenderer::RenderFrame() {
	Profiler::Start("RenerFull");
	PrepareData();

	_static.shadows->Render(_static.rendered);

	_static.screenFbo->Use();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	_static.skybox->Render();

	_static.gbufferFbo->Use();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	RenderGeometry();

	_static.screenFbo->Use();
	// glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	RenderLighting();
	// RenderBlocks();
	RenderBloom();
	RenderPostprocess();
	Profiler::Add("RenderFull");
	_static.ui->UpdateData();
	_static.ui->Draw();

	glfwSwapBuffers(_static.window);
}

bool GLRenderer::ModelInView(RenderModel* model) {
	glm::vec3 bounding[8];
	bounding[0] = model->GetPosition();
	bounding[1] = bounding[0] + glm::vec3(16.f, 0.f, 0.f);
	bounding[2] = bounding[0] + glm::vec3(0.f, 16.f, 0.f);
	bounding[3] = bounding[0] + glm::vec3(0.f, 0.f, 16.f);
	bounding[4] = bounding[0] + glm::vec3(16.f, 16.f, 0.f);
	bounding[5] = bounding[0] + glm::vec3(16.f, 0.f, 16.f);
	bounding[6] = bounding[0] + glm::vec3(16.f, 16.f, 0.f);
	bounding[7] = bounding[0] + glm::vec3(16.f, 16.f, 16.f);
	for (int i = 0; i < 8; i++) {
		if (_static.activeCamera->IsInView(_frame.vp, bounding[i])) {
			return true;
		}
	}
	return false;
}

void GLRenderer::RenderGeometry() {
	Shader* shader = _static.geometryPassShader;
	Material* lastMaterial = nullptr;

	glEnable(GL_DEPTH_TEST);
	shader->Use();
	uint gg = 0;
	for (RenderModel* model : _static.rendered) {
		//! ACHTUNG IT ONLY WORKS FOR BLOCK MODELS
		if (!ModelInView(model)) {
			gg++;
			continue;
		}
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
	// Log::Basic("Geometries skipped: " + std::to_string(gg));
	glDisable(GL_DEPTH_TEST);
}

void GLRenderer::RenderBlocks() {
	Shader* lastShader = nullptr;
	Material* lastMaterial = nullptr;

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	// glEnable(GL_CULL_FACE);
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
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
}

void GLRenderer::RenderBloom() {
	Texture* color;
	
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

void GLRenderer::RenderLighting() {
	Shader* shader = _static.lightingPassShader;
	Texture* color;
	
	shader->Use();
	_static.postQuad->Use();

	glActiveTexture(GL_TEXTURE0);
	shader->SetInt("positionMap", 0);
	_static.gbufferFbo->GetColorTexture(0)->Use();

	glActiveTexture(GL_TEXTURE1);
	shader->SetInt("normalMap", 1);
	_static.gbufferFbo->GetColorTexture(1)->Use();

	glActiveTexture(GL_TEXTURE2);
	shader->SetInt("albedoSpecMap", 2);
	_static.gbufferFbo->GetColorTexture(2)->Use();

	shader->SetFloat3("cameraPos", _frame.cameraPos);
	_static.skybox->ApplyDirLights(shader);
	_static.shadows->ApplySelf(shader);

	glDrawArrays(GL_TRIANGLES, 0, _static.postQuad->GetPolygonCount() * 3);
	
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void GLRenderer::RenderPostprocess() {
	Texture* color;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	// glDisable(GL_CULL_FACE);
	_static.postShader->Use();
	_static.postShader->SetFloat("runtime", _static.game->GetRuntime());
	_static.postQuad->Use();
	//! Debug
	// color = _static.gbufferFbo->GetColorTexture(1);
	color = _static.screenFbo->GetColorTexture();
	// color = _static.shadows->_shadowFbo->GetDepthTexture();
	glActiveTexture(GL_TEXTURE0);
	color->Use();
	color = _static.bloomFbo->GetColorTexture();
	glActiveTexture(GL_TEXTURE1);
	//! Debug
	color->Use();
	glDrawArrays(GL_TRIANGLES, 0, _static.postQuad->GetPolygonCount() * 3);
}

void GLRenderer::PrepareData() {
	_frame.view = _static.activeCamera->GetViewMatrix();
	_frame.projection = _static.activeCamera->GetProjectionMatrix();
	_frame.vp = _frame.projection * _frame.view;
	_frame.cameraPos = _static.activeCamera->GetPosition();
	// _static.shadows->PrepareData(_static.game->GetSunAngle());
	_static.shadows->PrepareData(30);
	// _static.skybox->PrepareData(_static.game->GetSunAngle(), _static.game->GetMoonAngle(), _static.game->GetSunVal(), _static.game->GetMoonVal());
	// _static.skybox->PrepareData(45, 0, 1, 0);
	_static.skybox->PrepareData(30, 10, 1, 0);
	std::sort(_static.rendered.begin(), _static.rendered.end(), RenderModelLess);
}
