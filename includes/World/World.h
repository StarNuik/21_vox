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
	BlockType GetBlock(glm::ivec3 globalPosition);
	void SetBlock(glm::ivec3 globalPosition, BlockType);
	void PlayerSetBlock(glm::ivec3 globalPosition, BlockType);
private:
	Game* _game;
	std::unordered_map<glm::ivec2, Chunk*> _chunks;
};
