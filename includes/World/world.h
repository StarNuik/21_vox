#pragma once

#include "Render/Objects.h"

class Block {
	private:
		RenderModel _model;
	public:
		Block();
		~Block();
};

class Chunk {
	private:
		glm::vec3 _position;
	public:
		Chunk();
		~Chunk();
		glm::vec3 GetPosition();
		glm::vec3 SetPosition();
};