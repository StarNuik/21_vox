#include <string>
#include "Mathf/Mathf.h"
#include "UI/UIController.h"
#include "Engine/Game.h"
#include "Render/GLRenderer.h"
// #include <math.h>

// UIData::DataRendering::DataRendering() {}

void UIController::UpdateRendering() {
	UIData::DataRendering& data = _dataRendering;

	data.bloomCutoff = _game->GetRenderer()->GetBloomCutoff();
};

void UIController::RenderingUI() {
	UIData::DataRendering& data = _dataRendering;

	ImGui::Text("Post-process Shader");
	const int postSize = 21;
	const char* postShaders[postSize] = {
		"Main: Bloom & HDR Aces approximate & Gamma",
		"None",
		"HDR Reinhard only",
		"Gamma correction",
		"HDR Reinhard & Gamma",
		"HDR Filmic & Gamma",
		"HDR Uncharted & Gamma",
		"HDR Aces approximate & Gamma",
		"Acesa & Gamma & Purple",
		"Acesa & Gamma & Inverse",
		"Acesa & Gamma & Grayscale",
		"Acesa & Gamma & Gaussian Blur 3x3",
		"Acesa & Gamma & Gaussian Blur 5x5",
		"Acesa & Gamma & Sharpen",
		"Acesa & Gamma & Outline",
		"Acesa & Gamma & Emboss",
		"Pre Bloom",
		"Gaussian Blur 5x5",
		"Main & Outline",
		"Main & Hueshift",
		"Main & Outline Moving",
	};
	ImGui::Combo("Post-process Shader", &_dataRendering.selectedPostshader, postShaders, postSize);
	if (ImGui::Button("Set post shader")) {
		_game->GetRenderer()->SetPostShader("Post " + std::string(postShaders[_dataRendering.selectedPostshader]));
	}
	ImGui::Separator();
	ImGui::Spacing();
	ImGui::Spacing();

	float f0 = data.bloomCutoff;
	ImGui::InputFloat("Bloom cutoff", &f0, 0.01f, 0.1f, "%.3f");

	if (data.bloomCutoff != f0)
		_game->GetRenderer()->SetBloomCutoff(f0);
};
