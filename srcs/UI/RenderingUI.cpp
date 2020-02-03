#include <string>
#include <glm/glm.hpp>
#include "UI/UIController.h"
#include "Engine/Game.h"
#include "Render/GLRenderer.h"
// #include <math.h>

// UIData::DataRendering::DataRendering() {}

void UIController::UpdateRendering() {
	UIData::DataRendering& data = _dataRendering;

};

void UIController::RenderingUI() {
	UIData::DataRendering& data = _dataRendering;

	ImGui::Text("Post-process Shader");
	const char* postShaders[13] = {
		"Main: HDR Aces approximate & Gamma",
		"None",
		"HDR Reinhard only",
		"Gamma correction",
		"HDR Reinhard & Gamma",
		"HDR Filmic & Gamma",
		"HDR Uncharted & Gamma",
		"Main & Purple",
		"Main & Inverse",
		"Main & Grayscale",
		"Main & Gaussian Blur 3x3",
		"Main & Sharpen",
		"Main & Edge detection"
	};
	ImGui::Combo("Post-process Shader", &_dataRendering.selectedPostshader, postShaders, 13);
	if (ImGui::Button("Set post shader")) {
		_game->GetRenderer()->SetPostShader("Post " + std::string(postShaders[_dataRendering.selectedPostshader]));
	}
};
