#include "UI/UIController.h"
#include "Generation/MapGeneration.h"
#include "Engine/Game.h"

UIData::DataGeneration::DataGeneration() {
	selectedBiome = 0;
}

void UIData::DataGeneration::SwitchBiome(Game* game, int newNoise) {
	selectedBiome = newNoise;
	FastNoise& noise = game->GetGeneration()->GetNoise((MapGeneration::GenerationType)selectedBiome);
	seed = noise.GetSeed();
	frequency = noise.GetFrequency();
	interpolation = noise.GetInterp();
	noiseType = noise.GetNoiseType();
	octaves = noise.GetFractalOctaves();
	lacunarity = noise.GetFractalLacunarity();
	gain = noise.GetFractalGain();
	fractalType = noise.GetFractalType();
	distFunc = noise.GetCellularDistanceFunction();
	retType = noise.GetCellularReturnType();
	noise.GetCellularDistance2Indices(distIndex0, distIndex1);
	jitter = noise.GetCellularJitter();
}

void UIController::UpdateGeneration() {
	UIData::DataGeneration& data = _dataGeneration;
	MapGeneration* g = _game->GetGeneration();

	data.exp = g->GetExpValue();
	data.terrace = g->GetTeracceValue();
	data.SwitchBiome(_game, data.selectedBiome);
}

void UIController::GenerationUI() {
	UIData::DataGeneration& data = _dataGeneration;
	MapGeneration* map = _game->GetGeneration();

	//? MapGeneration settings
	ImGui::Text("MapGeneration settings");
	const float zero = 0.f;
	const float fstep = 0.1f;
	const int istep = 1;

	float exp = data.exp;
	float terrace = data.terrace;
	ImGui::InputScalar("Exp", ImGuiDataType_Float, &exp, &fstep, "%.3f");
	ImGui::InputScalar("Terrace", ImGuiDataType_Float, &terrace, &fstep, "%.3f");
	ImGui::Separator();
	ImGui::Spacing();
	ImGui::Spacing();

	//? Biome choice
	ImGui::Text("Biome");
	for (int i = MapGeneration::First; i < MapGeneration::Size; i++)
	{
		char buf[80];
		sprintf(buf, "%s", map->GetNoiseName((MapGeneration::GenerationType)i).c_str());
		if (ImGui::Selectable(buf, data.selectedBiome == i)) {
			data.SwitchBiome(_game, i);
		}
	}
	ImGui::Separator();
	ImGui::Spacing();
	ImGui::Spacing();

	//? Noise type choice
	ImGui::Text("Noise type");	
	int noiseType = data.noiseType;
	const char* noiseNames[10] = {"Value", "ValueFractal", "Perlin", "PerlinFractal", "Simplex", "SimplexFractal", "Cellular", "WhiteNoise", "Cubic", "CubicFractal"};
	ImGui::Combo("Noise Type##1", &noiseType, noiseNames, 10);
	ImGui::Separator();
	ImGui::Spacing();
	ImGui::Spacing();

	//? General settings
	ImGui::Text("General settings");
	int seed = data.seed;
	float freq = data.frequency;
	int interp = data.interpolation;
	ImGui::InputScalar("Seed", ImGuiDataType_S32, &seed, &istep, "%d");
	ImGui::InputScalar("Frequency", ImGuiDataType_Float, &freq, &fstep, "%.3f");
	// const char* interpNames[3] = {"Linear", "Hermite", "Quintic"};
	ImGui::Combo("Interpolation##2", &interp, "Linear\0Hermite\0Quintic\0\0");
	ImGui::Separator();
	ImGui::Spacing();
	ImGui::Spacing();

	//? Fractal settings
	int octaves = data.octaves;
	float lacunarity = data.lacunarity;
	float gain = data.gain;
	int fractalType = data.fractalType;
	switch (noiseType) {
	case FastNoise::ValueFractal:
	case FastNoise::PerlinFractal:
	case FastNoise::SimplexFractal:
	case FastNoise::CubicFractal:
		ImGui::Text("Fractal settings");
		ImGui::InputScalar("Octaves", ImGuiDataType_S32, &octaves, &istep, "%d");
		ImGui::InputScalar("Lacunarity", ImGuiDataType_Float, &lacunarity, &fstep, "%.3f");
		ImGui::InputScalar("Gain", ImGuiDataType_Float, &gain, &fstep, "%.3f");
		ImGui::Combo("Interpolation##3", &fractalType, "FBM\0Billow\0RigidMulti\0\0");
		ImGui::Separator();
		ImGui::Spacing();
		ImGui::Spacing();
	default:
		break;
	}


	int distFunc = data.distFunc;
	int retType = data.retType;
	int index0 = data.distIndex0;
	int index1 = data.distIndex1;
	float jitter = data.jitter;
	//? Cellular settings
	if (noiseType == FastNoise::Cellular) {
		ImGui::Text("Cellular settings");
		ImGui::Combo("Distance function##4", &distFunc, "Euclidian\0Manhattan\0Natural\0\0");
		ImGui::Combo("Return type##5", &retType, "Cell Value\0 \0Distance\0Distance2\0Distance2Add\0Distance2Sub\0Distance2Mul\0Distance2Div\0\0");
		if (retType == 1)
			retType = 0;
		ImGui::InputScalar("Jitter", ImGuiDataType_Float, &jitter, &fstep, "%.3f");
		if (retType >= 3) {
			ImGui::InputScalar("Distance index 0", ImGuiDataType_S32, &index0, &istep, "%d");
			index0 = glm::clamp(index0, 0, 4);
			ImGui::InputScalar("Distance index 1", ImGuiDataType_S32, &index1, &istep, "%d");
			if (index1 < 0)
				index1 = 0;
		}
		ImGui::Separator();
		ImGui::Spacing();
		ImGui::Spacing();
	}

	//? Data handling
	FastNoise& noise = _game->GetGeneration()->GetNoise((MapGeneration::GenerationType)data.selectedBiome);
	if (exp != data.exp)
		_game->GetGeneration()->SetExpValue(exp);
	if (terrace != data.terrace)
		_game->GetGeneration()->SetTerraceValue(terrace);
	if (seed != data.seed)
		noise.SetSeed(seed);
	if (freq != data.frequency)
		noise.SetFrequency(freq);
	if (interp != data.interpolation)
		noise.SetInterp((FastNoise::Interp)interp);
	if (noiseType != data.noiseType)
		noise.SetNoiseType((FastNoise::NoiseType)noiseType);
	if (octaves != data.octaves)
		noise.SetFractalOctaves(octaves);
	if (lacunarity != data.lacunarity)
		noise.SetFractalLacunarity(lacunarity);
	if (gain != data.gain)
		noise.SetFractalGain(gain);
	if (fractalType != data.fractalType)
		noise.SetFractalType((FastNoise::FractalType)fractalType);
	if (distFunc != data.distFunc)
		noise.SetCellularDistanceFunction((FastNoise::CellularDistanceFunction)distFunc);
	if (retType != data.retType)
		noise.SetCellularReturnType((FastNoise::CellularReturnType)retType);
	if (     index0 != data.distIndex0 && index1 == data.distIndex1)
		noise.SetCellularDistance2Indices(index0, data.distIndex1);
	else if (index0 == data.distIndex0 && index1 != data.distIndex1)
		noise.SetCellularDistance2Indices(data.distIndex0, index1);
	else if (index0 != data.distIndex0 && index1 != data.distIndex1)
		noise.SetCellularDistance2Indices(index0, index1);
	if (jitter != data.jitter)
		noise.SetCellularJitter(jitter);

	if (ImGui::Button("Regenerate world")) {
		_game->DestroyWorld();
		_game->InitWorld();
	}
};