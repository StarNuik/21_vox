#include <glm/glm.hpp>
#include "UI/UIController.h"
#include "Engine/Game.h"
// #include <math.h>

// UIData::DataRendering::DataRendering() {}

void UIController::UpdateRendering() {
	UIData::DataRendering& data = _dataRendering;

};

void UIController::RenderingUI() {
	UIData::DataRendering& data = _dataRendering;

	ImGui::Text("Rendering UI");
};
