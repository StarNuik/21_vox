#include "UI/UI.h"
// #include <math.h>

DataRendering::DataRendering() {
	frameTimes.resize(UI_PLOT_FRAMES, 0);
	maxFrameTime = 1;
}

void UIController::UpdateRendering() {
	DataRendering& data = _dataRendering;

	uint frameTime = _game->GetLastFrameTime();
	data.frameTimes.pop_front();
	data.frameTimes.push_back(frameTime);
};

void UIController::RenderingUI() {
	DataRendering& data = _dataRendering;

	float array[UI_PLOT_FRAMES];
	float average;
	for (int i = 0; i < UI_PLOT_FRAMES; i++) {
		array[i] = data.frameTimes[i];
		average += array[i];
		data.maxFrameTime = glm::max(data.maxFrameTime, array[i]);
	}
	average /= UI_PLOT_FRAMES;
	std::string overlay = "Average: " + std::to_string(average) + " ms.";
	ImGui::PlotLines("Frame time", array, UI_PLOT_FRAMES, 0, overlay.c_str(), 0.0f, data.maxFrameTime, ImVec2(0, 100));
	// ImGui::Text("Current frame time: %.3f s.", (float)_game->GetLastFrameTime() / 1000.f);
};
