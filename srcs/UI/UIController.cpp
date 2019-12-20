#include "UI/UIController.h"

UIController::UIController(Game* game) {
	_game = game;
	_uiOn = false;
	_showProfiler = true;
};

UIController::~UIController() {};

void UIController::SetState(bool state) {_uiOn = state;};
bool UIController::GetState() {return _uiOn;};
void UIController::SetPlayer(Player* player) {_player = player;};

void UIController::Draw() {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	MainUI();
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void UIController::UpdateData() {
	UpdateProfiler();
	UpdatePlayer();
	UpdateRendering();
	UpdateGeneration();
	UpdateResources();
}