#pragma once

#include "Render/Objects.h"
#include "Engine/Engine.h"
#include <unordered_map>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtx/hash.hpp>
// #include "Types.h"

class Game;

enum BLOCK_TYPE {
	AIR = 0,
	BEDROCK,
	COBBLESTONE,
	DIRT,
	PLANKS,
	SAND,
	STONE
};

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
	void UpdateGeometry();
	Block* GetBlock(glm::ivec3);
	void SetBlock(glm::ivec3, BLOCK_TYPE);
private:
	bool _state;
	Game* _game;
	BLOCK_TYPE _blocks[16][16][16];
	std::vector<RenderModel*> _models;
	RenderModel* _debugModels[16][16][16];
};

class Chunk {
public:
	Chunk(Game*, glm::ivec2);
	~Chunk();
	void SetActive(bool);
	Block* GetBlock(glm::ivec3);
	void SetBlock(glm::ivec3, BLOCK_TYPE);
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
	Block* GetBlock(glm::ivec3);
	void SetBlock(glm::ivec3, BLOCK_TYPE);
private:
	Game* _game;
	std::unordered_map<glm::ivec2, Chunk*> _chunks;
};