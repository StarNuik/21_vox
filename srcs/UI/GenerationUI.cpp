#include "UI/UI.h"

DataGeneration::DataGeneration() {
	selectedNoise = 0;
}

void DataGeneration::SwitchNoise(Game* game, int newNoise) {
	selectedNoise = newNoise;
	FastNoise& noise = game->GetGeneration()->GetNoise((MapGeneration::GenerationType)selectedNoise);
	seed = noise.GetSeed();
	frequency = noise.GetFrequency();
	interpolation = noise.GetInterp();
	noiseType = noise.GetNoiseType();
}

void UIController::UpdateGeneration() {
	DataGeneration& data = _dataGeneration;
	MapGeneration* g = _game->GetGeneration();

	data.exp = g->GetExpValue();
	data.terrace = g->GetTeracceValue();
	data.SwitchNoise(_game, data.selectedNoise);
}

void UIController::GenerationUI() {
	const float zero = 0.f;
	const float fstep = 0.1f;
	const int istep = 1;
	DataGeneration& data = _dataGeneration;
	MapGeneration* map = _game->GetGeneration();

	float exp = data.exp;
	float terrace = data.terrace;
	ImGui::InputScalar("Exp", ImGuiDataType_Float, &exp, &fstep, "%.3f");
	ImGui::InputScalar("Terrace", ImGuiDataType_Float, &terrace, &fstep, "%.2f");
	if (exp != data.exp)
		_game->GetGeneration()->SetExpValue(exp);
	if (terrace != data.terrace)
		_game->GetGeneration()->SetTerraceValue(terrace);

	ImGui::Separator();
	for (int i = MapGeneration::First; i < MapGeneration::Size; i++)
	{
		char buf[80];
		sprintf(buf, "%s", map->GetNoiseName((MapGeneration::GenerationType)i).c_str());
		if (ImGui::Selectable(buf, data.selectedNoise == i)) {
			data.SwitchNoise(_game, i);
		}
	}
	ImGui::Separator();

	int seed = data.seed;
	float freq = data.frequency;
	int interp = data.interpolation;
	int noiseType = data.noiseType;
	ImGui::InputScalar("Seed", ImGuiDataType_S32, &seed, &istep, "%d");
	ImGui::InputScalar("Frequency", ImGuiDataType_Float, &freq, &fstep, "%.3f");
	
	const char* interpNames[3] = {"Linear", "Hermite", "Quintic"};
	ImGui::PushID(15);
	ImGui::Combo("Interpolation", &interp, interpNames, 3);
	ImGui::PopID();

	const char* noiseNames[10] = {"Value", "ValueFractal", "Perlin", "PerlinFractal", "Simplex", "SimplexFractal", "Cellular", "WhiteNoise", "Cubic", "CubicFractal"};
	ImGui::PushID(16);
	ImGui::Combo("Interpolation", &noiseType, noiseNames, 10);
	ImGui::PopID();

	FastNoise& noise = _game->GetGeneration()->GetNoise((MapGeneration::GenerationType)data.selectedNoise);
	if (seed != data.seed)
		noise.SetSeed(seed);
	if (freq != data.frequency)
		noise.SetFrequency(freq);
	if (interp != data.interpolation)
		noise.SetInterp((FastNoise::Interp)interp);
	if (noiseType != data.noiseType)
		noise.SetNoiseType((FastNoise::NoiseType)noiseType);

	ImGui::Separator();
	if (ImGui::Button("Regenerate world")) {
		_game->DestroyWorld();
		_game->InitWorld();
	}
};