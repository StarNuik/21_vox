#include "UI/UIController.h"

void UIController::MainUI() {
	if (_uiOn) {
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
		if (ImGui::Button("Toggle profiler")) {
			_showProfiler = !_showProfiler;
		}
		ImGui::End();
		ImGui::ShowDemoWindow();
	}
	if (_showProfiler) {
		ProfilerUI();
	}
}




