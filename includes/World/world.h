#pragma once

#include "Types.h"
#include "Render/Objects.h"
#include "Engine/Engine.h"
#include <unordered_map>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtx/hash.hpp>
// #include "Types.h"

class Game;

class Block {
public:
	Block(Texture* texture);
	~Block();
private:
		Texture* _texture;
		bool _isWalkable;
};

class Shard {
public:
	Shard(Game*, glm::ivec3);
	~Shard();
	void SetActive(bool);
	void UpdateGeometry(glm::ivec3);
	BlockType GetBlock(glm::ivec3);
	void SetBlock(glm::ivec3, BlockType);
private:
	bool HasType(BlockType);
	std::vector<float> GenerateBlock(glm::ivec3);
	bool _state;
	Game* _game;
	World* _world;
	//! Maybe change to ammount of blocks
	bool _blockTypePresent[(int)BlockType::Stone + 1] = {false};
	BlockType _blocks[16][16][16] = {BlockType::Air};
	std::vector<RenderModel*> _models;
	RenderModel* _debugModels[16][16][16];
};

class Chunk {
public:
	Chunk(Game*, glm::ivec2);
	~Chunk();
	void SetActive(bool);
	BlockType GetBlock(glm::ivec3);
	void SetBlock(glm::ivec3, BlockType);
	void UpdateGeometry(glm::ivec2);
private:
	bool _state;
	Shard* _shards[16];
};

class World {
public:
	World(Game*);
	~World();
	void ActivateChunk(glm::ivec2);
	void DeactivateChunk(glm::ivec2);
	BlockType GetBlock(glm::ivec3);
	void SetBlock(glm::ivec3, BlockType);
private:
	Game* _game;
	std::unordered_map<glm::ivec2, Chunk*> _chunks;
};