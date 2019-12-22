#include "UI/UIController.h"
#include "Engine/Game.h"
#include <algorithm>
#include "Utilities/Profiler.h"
#include "Player/Player.h"
#include <glm/glm.hpp>

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
	data.frameFull.push_back(Profiler::GetMs("FrameFull"));
	data.input.pop_front();
	data.input.push_back(Profiler::GetMs("Input"));
	data.update.pop_front();
	data.update.push_back(Profiler::GetMs("Update"));
	data.renderFull.pop_front();
	data.renderFull.push_back(Profiler::GetMs("RenderFull"));
};

void UIController::ProfilerUI() {
	const float margin = 20.f;
	UIData::DataProfiler& data = _dataProfiler;
	ImGuiIO& io = ImGui::GetIO();

	ImGui::SetNextWindowPos(ImVec2(margin, io.DisplaySize.y - margin), ImGuiCond_Always, ImVec2(0.f, 1.f));
	ImGui::SetNextWindowBgAlpha(0.9f);
	int windowFlags = ImGuiWindowFlags_NoMove |
	ImGuiWindowFlags_NoDecoration |
	ImGuiWindowFlags_AlwaysAutoResize |
	ImGuiWindowFlags_NoSavedSettings |
	ImGuiWindowFlags_NoFocusOnAppearing |
	ImGuiWindowFlags_NoNav;
	ImGui::Begin("Profiler", nullptr, windowFlags);
	glm::vec3 pos = _player->GetPosition();
	glm::vec3 dir = _player->GetDirection();
	ImGui::Text("Pos: [x: %6.1f, y: %6.1f, z: %6.1f]", pos.x, pos.y, pos.z);
	ImGui::Text("Dir: [x: %6.1f, y: %6.1f, z: %6.1f]", dir.x, dir.y, dir.z);
	ImGui::Separator();
	PlotLines(data.frameFull, "Full frame");
	ImGui::Separator();
	PlotLines(data.input, "Input loop");
	ImGui::Separator();
	PlotLines(data.update, "Update loop");
	ImGui::Separator();
	PlotLines(data.renderFull, "Render loop");
	ImGui::End();
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
	ImGui::PlotLines(label, array, UI_PLOT_FRAMES, 0, overlay.c_str(), 0.f, (maxValue + maxValue * .2f), ImVec2(0, 50));
}
