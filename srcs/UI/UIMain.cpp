#include "UI/UI.h"

void UIController::Render() {};
void UIController::Engine() {};
void UIController::Player() {};
void UIController::World() {};
void UIController::Resources() {};

void UIController::Main() {
	ImGui::Begin("Toolbar");
	char s[30];
	sprintf(s, "Current frame time: %.3f s.", (float)_game->GetLastFrameTime() / 1000.f);
	ImGui::Text(s);
	ImGui::End();
	// ImGui::ShowDemoWindow();
}
