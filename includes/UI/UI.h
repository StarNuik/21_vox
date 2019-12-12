#pragma once

#include "Types.h"
#include <deque>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "Engine/Engine.h"

#define UI_PLOT_FRAMES 100

struct DataRendering {
	DataRendering();
	std::deque<uint> frameTimes;
	float maxFrameTime;
};

struct DataPlayer {};
struct DataGeneration {};
struct DataResources {};

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

	void UpdatePlayer();
	void UpdateRendering();
	void UpdateGeneration();
	void UpdateResources();

	void PlayerUI();
	void RenderingUI();
	void GenerationUI();
	void ResourcesUI();

	Player* _player;
	ResourceLoader* _resources;
	Game* _game;
	bool _uiOn;
	DataRendering _dataRendering;
	DataPlayer _dataPlayer;
	DataGeneration _dataGeneration;
	DataResources _dataResources;
};
