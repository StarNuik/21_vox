#pragma once

#include "Mathf/Mathf.h"

#include "Mathf/Mathf.h"

#include "Types.h"

class Framebuffer {
public:
	Framebuffer();
	~Framebuffer();
	void NewBloom(mathf::ivec2 bufferSize);
	void NewColor(mathf::ivec2 bufferSize);
	void NewShadow(mathf::ivec2 bufferSize);
	void Bind();
	void Use();
	void Unbind();
	void Resize(mathf::ivec2 newSize);
	Texture* GetColorTexture();
	Texture* GetColorTexture(int);
	Texture* GetDepthTexture();
	uint GetId();
private:
	bool _ready;
	uint _fbo;
	Texture* _color0;
	Texture* _color1;
	Texture* _depth;
};