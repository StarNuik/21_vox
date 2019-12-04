#pragma once

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
private:
	void StartFrame();
	void EndFrame();
	Game* _game;
	bool _uiOn;
};