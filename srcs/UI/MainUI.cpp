#include "UI/UIController.h"

void UIController::MainUI() {
	ImGui::Begin("Toolbar");
	if (ImGui::CollapsingHeader("Player")) {
		PlayerUI();
	}
	if (ImGui::CollapsingHeader("Rendering")) {
		RenderingUI();
	}
	if (ImGui::CollapsingHeader("Generation")) {
		GenerationUI();
	}
	if (ImGui::CollapsingHeader("Resources")) {
		ResourcesUI();
	}
	ImGui::End();
	ImGui::ShowDemoWindow();
}




