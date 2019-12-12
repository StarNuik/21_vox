#include "UI/UI.h"

void UIController::RenderingUI() {
	ImGui::Text("Current frame time: %.3f s.", (float)_game->GetLastFrameTime() / 1000.f);
};
