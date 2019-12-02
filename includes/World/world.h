#pragma once

#include "Render/Render.h"

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
		glm::vec3 GetPosition();
		glm::vec3 SetPosition();
};