#include "UI/UI.h"

UIController::UIController(Game* game) {
	_game = game;
	_uiOn = false;
};
UIController::~UIController() {};

void UIController::SetState(bool state) {
	_uiOn = state;
};

void UIController::StartFrame() {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
};

void UIController::EndFrame() {
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
};

void UIController::Draw() {
	StartFrame();
	if (_uiOn) {
		Main();
	}
	EndFrame();
}