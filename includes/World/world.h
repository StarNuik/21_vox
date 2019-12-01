#pragma once

#include "Render/render.h"

class Block {
	private:
		Geometry geometry;
		Texture texture;
		Shader shader;
	public:
		Block();
		~Block();
		Geometry GetGeometry();
		Texture GetTexture();
		Shader GetShader();
};

class Chunk {
	private:
		glm::vec3 position;
	public:
		Chunk();
		~Chunk();
		GetPosition();
		SetPosition();
};