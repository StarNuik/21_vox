#pragma once

#include <vector>
#include <glm/glm.hpp>
#include "Types.h"

class Shard {
public:
	Shard(Game*, glm::ivec3);
	~Shard();
	void Generate();
	void UpdateGeometry();
	void SetActive(bool);
	BlockType GetBlock(glm::ivec3);
	void SetBlock(glm::ivec3, BlockType);
private:
	bool HasType(BlockType);
	uint CountType(BlockType);
	std::vector<float> GenerateBlock(World*, glm::ivec3);
	RenderModel* GenerateModelOfType(BlockType);

	bool _state;
	glm::ivec3 _position;
	Game* _game;
	BlockType _blocks[16][16][16] = {BlockType::Air};
	uint _blockTypePresent[(uint)BlockType::Stone + 1] = {0};
	std::vector<RenderModel*> _models;
};