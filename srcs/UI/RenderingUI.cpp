#include "UI/UI.h"
// #include <math.h>

DataRendering::DataRendering() {
	frameTimes.resize(UI_PLOT_FRAMES, 0);
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
	float average = 0.f;
	float maxFrameTime = 1.f;
	for (int i = 0; i < UI_PLOT_FRAMES; i++) {
		array[i] = data.frameTimes[i];
		average += array[i];
		maxFrameTime = glm::max(maxFrameTime, array[i]);
	}
	average /= (float)UI_PLOT_FRAMES;
	std::string overlay = "Average: " + std::to_string(average) + " ms.";
	ImGui::PlotLines("Frame time", array, UI_PLOT_FRAMES, 0, overlay.c_str(), 0.f, (maxFrameTime + maxFrameTime * .5f), ImVec2(0, 100));
};
