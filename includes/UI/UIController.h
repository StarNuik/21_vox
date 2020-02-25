#pragma once

#include <deque>

#include "Types.h"
#include "imgui.h"
#include "examples/imgui_impl_glfw.h"
#include "examples/imgui_impl_opengl3.h"
#include "FastNoise.h"

#define UI_PLOT_FRAMES 100

namespace UIData {
	class DataProfiler {
	public:
		DataProfiler();
		std::deque<float> frameFull;
		std::deque<float> input;
		std::deque<float> update;
		std::deque<float> renderFull;
		std::deque<float> worldGen;
		int generationQueueSize;
		int geometryQueueSize;
	};

	class DataRendering {
	public:
		int selectedPostshader;
		float bloomCutoff;
	};

	class DataPlayer {};

	class DataGeneration {
	public:
		DataGeneration();
		void SwitchBiome(Game*, int);
		int selectedBiome;
		float exp;
		float terrace;
		int seed;
		float frequency;
		FastNoise::Interp interpolation;
		FastNoise::NoiseType noiseType;
		int octaves;
		float lacunarity;
		float gain;
		FastNoise::FractalType fractalType;
		FastNoise::CellularDistanceFunction distFunc;
		FastNoise::CellularReturnType retType;
		int distIndex0;
		int distIndex1;
		float jitter;
	};

	class DataResources {};
}

class UIController {
public:
	UIController(Game*);
	~UIController();
	void Draw();
	void UpdateData();
	void SetState(bool);
	bool GetState();
	void SetPlayer(Player*);
private:
	void MainUI();

	void UpdateProfiler();
	void UpdatePlayer();
	void UpdateRendering();
	void UpdateGeneration();
	void UpdateResources();

	void ProfilerUI();
	void PlayerUI();
	void RenderingUI();
	void GenerationUI();
	void ResourcesUI();


	Player* _player;
	ResourceLoader* _resources;
	Game* _game;
	bool _uiOn;
	UIData::DataRendering _dataRendering;
	UIData::DataPlayer _dataPlayer;
	UIData::DataGeneration _dataGeneration;
	UIData::DataResources _dataResources;
	UIData::DataProfiler _dataProfiler;
	bool _showProfiler;
};


