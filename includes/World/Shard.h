#pragma once

#include <vector>
#include <glm/glm.hpp>

#include "Types.h"
#include "World/Block.h"
#include "Render/VertexBuffers.h"

#define USED_SHADER "Flower"
#define BLOCKS_SHADER "Flower"
#define BLOCKS_MATERIAL Block::Air
#define WATER_SHADER "Flower"
#define WATER_MATERIAL Block::Water

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

	//! Old model generation
	// RenderModel* GenerateModelOfBlock(Block);
	// std::vector<float> GenerateGeometryFor(Block, World*, glm::ivec3 global, glm::ivec3 local);
	// std::vector<float> GenerateBlock(World*, glm::ivec3 global, glm::ivec3 local);
	// std::vector<float> GenerateTransparent(World*, glm::ivec3 global, glm::ivec3 local, Block);
	// std::vector<float> GenerateMultisideBlock(World*, glm::ivec3 global, glm::ivec3 local);
	// std::vector<float> GenerateFlower();
	// std::vector<float> GenerateCactus();
	// std::vector<float> GenerateWater(World*, glm::ivec3 global, glm::ivec3 local);
	// void AddSide(std::vector<float>&, glm::ivec3 l, glm::ivec3 g, glm::ivec3 o, VertexBuffers::BufferType, World*);
	// void AddSideWater(std::vector<float>&, glm::ivec3 l, glm::ivec3 g, glm::ivec3 o, VertexBuffers::BufferType, VertexBuffers::BufferType, World*);
	// void AddSideTransparent(std::vector<float>&, glm::ivec3 l, glm::ivec3 g, glm::ivec3 o, VertexBuffers::BufferType, World*, Block);
	//! --- ----- ----------

	//? New model generation
	RenderModel* GenerateBlocks();
	RenderModel* GenerateWater();
	std::vector<float> GenerateBlock(World*, glm::ivec3 global, glm::ivec3 local, Block);
	std::vector<float> GenerateWaterBlock(World*, glm::ivec3 global, glm::ivec3 local);
	void AddSide(std::vector<float>&, World*, Block, glm::ivec3 global, glm::ivec3 local, glm::ivec3 offset, VertexBuffers::BufferType, Block::Side);
	//? --- ----- ----------

	bool _state;
	glm::ivec3 _position;
	Game* _game;
	Block _blocks[16][16][16] = {Block::Air};
	uint16 _blockCount[(int)Block::Size] = {4096, 0};
	std::vector<RenderModel*> _models;
};