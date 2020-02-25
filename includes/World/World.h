#pragma once

#include <unordered_map>
#include <glm/glm.hpp>
#include <glm/gtx/hash.hpp>

#include "Types.h"

#define WORLD_RADIUS 6
#define RADIUS_SQUARED (WORLD_RADIUS * WORLD_RADIUS)

#define MAX_UPDATES_PER_FRAME 1

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
	void SetBlock(glm::vec3 globalPosition, Block);
	void PlayerSetBlock(glm::vec3 globalPosition, Block);
	// BlockType GetBlock(glm::ivec3 globalPosition);
	Block GetBlock(glm::ivec3 globalPosition);
	void SetBlock(glm::ivec3 globalPosition, Block);
	void PlayerSetBlock(glm::ivec3 globalPosition, Block);
	void AssessChunks(glm::vec3 playerPosition);
private:
	void CreateChunks();
	void DeleteChunks();
	glm::ivec2 Global2Chunk(glm::ivec3 globalP);
	glm::ivec3 Global2Local(glm::ivec3 globalP);
	Game* _game;
	std::unordered_map<glm::ivec2, Chunk*> _chunks;
	WorldCreator* _wc;
};
