#pragma once

#include <vector>
#include <glm/glm.hpp>
#include "Mathf/Mathf.h"
#include <GL/glew.h>

#include "Types.h"

#define SHADOWMAP_SIDE (4096) //* 336 is the side in 21 chunks
#define TEXTURE_SLOT (GL_TEXTURE30)

class ShadowRenderer {
public:
	ShadowRenderer();
	~ShadowRenderer();
	void Init(Game*);
	void Render(std::vector<RenderModel*>&);
	void ApplySelf(Shader*);
	void ApplyMap(Shader* shader);
	void ApplyLightSpace(Shader* shader);
	mathf::mat4x4 GetLightSpace();
	void PrepareData(float sunAngle);
private:
	// uint _shadowFBO, _sunMap, _moonMap;
	Game* _game;
	Camera* _camera;
	Shader* _shader;
	mathf::mat4x4 _lightSpace;
	Framebuffer* _shadowFbo;
	Player* _player;
	mathf::mat4x4 _view;
	mathf::mat4x4 _projection;
};
