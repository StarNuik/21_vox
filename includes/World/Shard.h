#pragma once

#include <vector>
#include <glm/glm.hpp>

#include "Types.h"
#include "World/Block.h"
#include "Render/VertexBuffers.h"

#define USED_SHADER "Phong"

class Shard {
public:
	Shard(Game*, glm::ivec3);
	~Shard();
	void UpdateGeometry();
	void SetActive(bool);
	Block GetBlock(glm::ivec3 localP);
	void SetBlock(glm::ivec3 localP, Block);
private:
	bool HasBlock(Block);
	uint CountBlock(Block);
	RenderModel* GenerateModelOfBlock(Block);
	std::vector<float> GenerateGeometryFor(Block, World*, glm::ivec3 global, glm::ivec3 local);
	std::vector<float> GenerateBlock(World*, glm::ivec3 global, glm::ivec3 local);
	std::vector<float> GenerateTransparent(World*, glm::ivec3 global, glm::ivec3 local, Block);
	std::vector<float> GenerateMultisideBlock(World*, glm::ivec3 global, glm::ivec3 local);
	std::vector<float> GenerateFlower(World*, glm::ivec3 global, glm::ivec3 local);
	void AddSide(std::vector<float>&, glm::ivec3 l, glm::ivec3 g, glm::ivec3 o, VertexBuffers::BufferType, World*);
	void AddSideTransparent(std::vector<float>&, glm::ivec3 l, glm::ivec3 g, glm::ivec3 o, VertexBuffers::BufferType, World*, Block);

	bool _state;
	glm::ivec3 _position;
	Game* _game;
	Block _blocks[16][16][16] = {Block::Air};
	uint16 _blockCount[(int)Block::Size] = {4096, 0};
	std::vector<RenderModel*> _models;
};