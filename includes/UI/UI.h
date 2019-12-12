#pragma once

#include "Types.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "Engine/Engine.h"

class Game;

class UIController {
public:
	UIController(Game*);
	~UIController();
	void Draw();
	void SetState(bool);
	bool GetState();
	void SetPlayer(Player*);
private:
	void StartFrame();
	void EndFrame();
	void MainUI();
	void PlayerUI();
	void RenderingUI();
	void GenerationUI();
	void ResourcesUI();
	Player* _player;
	ResourceLoader* _resources;
	Game* _game;
	bool _uiOn;
};
