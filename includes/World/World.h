#pragma once

#include <unordered_map>
#include <glm/glm.hpp>
#include <glm/gtx/hash.hpp>

#include "Types.h"

class World {
public:
	World(Game*);
	~World();
	void GenerateChunk(glm::ivec2);
	void DestroyChunk(glm::ivec2);
	void ActivateChunk(glm::ivec2);
	void SetChunkState(glm::ivec2, bool);
	void DeactivateChunk(glm::ivec2);
	// BlockType GetBlock(glm::vec3 globalPosition);
	Block GetBlock(glm::vec3 globalPosition);
	void SetBlock(glm::vec3 globalPosition, BlockType);
	void PlayerSetBlock(glm::vec3 globalPosition, BlockType);
	// BlockType GetBlock(glm::ivec3 globalPosition);
	Block GetBlock(glm::ivec3 globalPosition);
	void SetBlock(glm::ivec3 globalPosition, BlockType);
	void PlayerSetBlock(glm::ivec3 globalPosition, BlockType);
private:
	glm::ivec2 Global2Chunk(glm::ivec3 globalP);
	glm::ivec3 Global2Local(glm::ivec3 globalP);
	Game* _game;
	std::unordered_map<glm::ivec2, Chunk*> _chunks;
};
