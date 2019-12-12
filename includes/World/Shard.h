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
	RenderModel* GenerateModelOfType(BlockType);
	std::vector<float> GenerateGeometryFor(BlockType, World*, glm::ivec3);
	std::vector<float> GenerateBlock(World*, glm::ivec3);
	std::vector<float> GenerateMultisideBlock(World*, glm::ivec3);
	std::vector<float> GenerateFlower(World*, glm::ivec3);

	bool _state;
	glm::ivec3 _position;
	Game* _game;
	BlockType _blocks[16][16][16] = {BlockType::Air};
	uint _blockTypePresent[(uint)BlockType::Last] = {0};
	std::vector<RenderModel*> _models;
};