#pragma once

#include <glm/glm.hpp>
#include "Types.h"

class Chunk {
public:
	Chunk(Game*, glm::ivec2);
	~Chunk();
	void Generate();
	// void UpdateGeometry();
	void SetActive(bool);
	BlockType GetBlock(glm::ivec3);
	void SetBlock(glm::ivec3, BlockType);
	void PlayerSetBlock(glm::ivec3, BlockType);
private:
	bool _state;
	Shard* _shards[16];
};
