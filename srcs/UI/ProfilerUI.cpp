#include "UI/UIController.h"
#include "Engine/Game.h"
#include <algorithm>
#include "Utilities/Profiler.h"

void PlotLines(std::deque<float>& storage, const char* label);

UIData::DataProfiler::DataProfiler() {
	frameFull.resize(UI_PLOT_FRAMES, 0);
	input.resize(UI_PLOT_FRAMES, 0);
	update.resize(UI_PLOT_FRAMES, 0);
	renderFull.resize(UI_PLOT_FRAMES, 0);
}

void UIController::UpdateProfiler() {
	UIData::DataProfiler& data = _dataProfiler;

	data.frameFull.pop_front();
	data.frameFull.push_back(Profiler::Getf("FrameFull"));
	data.input.pop_front();
	data.input.push_back(Profiler::Getf("Input"));
	data.update.pop_front();
	data.update.push_back(Profiler::Getf("Update"));
	data.renderFull.pop_front();
	data.renderFull.push_back(Profiler::Getf("RenderFull"));
};

void UIController::ProfilerUI() {
	UIData::DataProfiler& data = _dataProfiler;

	PlotLines(data.frameFull, "Full frame");
	PlotLines(data.input, "Input loop");
	PlotLines(data.update, "Update loop");
	PlotLines(data.renderFull, "Render loop");
};

void PlotLines(std::deque<float>& storage, const char* label) {
	float array[UI_PLOT_FRAMES];
	float average = 0.f;
	float maxValue = 0.00000001f;
	for (int i = 0; i < UI_PLOT_FRAMES; i++) {
		array[i] = storage[i];
		average += array[i];
		maxValue = std::max(maxValue, array[i]);
	}
	average /= (float)UI_PLOT_FRAMES;
	std::string overlay = "Average: " + std::to_string(average) + " ms.";
	ImGui::PlotLines(label, array, UI_PLOT_FRAMES, 0, overlay.c_str(), 0.f, (maxValue + maxValue * .2f), ImVec2(0, 100));
}
