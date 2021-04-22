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
	glm::mat4 GetLightSpace();
	void PrepareData(float sunAngle);
private:
	// uint _shadowFBO, _sunMap, _moonMap;
	Game* _game;
	Camera* _camera;
	Shader* _shader;
	glm::mat4 _lightSpace;
	Framebuffer* _shadowFbo;
	Player* _player;
	glm::mat4 _view;
	glm::mat4 _projection;
};
