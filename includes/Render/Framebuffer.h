#pragma once

#include <glm/glm.hpp>

#include "Types.h"

class Framebuffer {
public:
	Framebuffer();
	~Framebuffer();
	void NewColor(glm::ivec2 windowSize);
	void NewShadow(glm::ivec2 windowSize);
	void Bind();
	void Use();
	void Unbind();
	void Resize(glm::ivec2 newSize);
	Texture* GetColorTexture();
	Texture* GetDepthTexture();
private:
	bool _ready;
	uint _fbo;
	Texture* _color;
	Texture* _depth;
};