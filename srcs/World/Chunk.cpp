#include "Types.h"
#include "World/World.h"
#include "World/Shard.h"
#include "World/Chunk.h"
#include "Engine/Game.h"

Chunk::Chunk(Game* game, glm::ivec2 pos) {
	_state = false;
	_game = game;
	_position = pos;
	_mp = _game->GetGeneration();
	for (int y = 0; y < 16; y++) {
		_shards[y] = new Shard(game, glm::ivec3(pos.x, y, pos.y));
	}
};

Chunk::~Chunk() {
	for (int y = 0; y < 16; y++) {
		delete _shards[y];
	}
};



void Chunk::Generate() {
	MapGeneration::StoredMapData block;
	
	for (int x = 0; x < 16; x++)
	{
		for (int z = 0; z < 16; z++)
		{
			int firstLayerBorder = 0;
			int lastLayerBorder = 0;
			int cavesHeight = 0;
			int crevicesHeight = 0;
			FillFirstLayer(firstLayerBorder, x, z);
			FillSecondLayer(block, lastLayerBorder, firstLayerBorder, x, z);
			FillSwampWater(block.biom, lastLayerBorder, x, z);
			FillHighLandAboveTheRiver(block, firstLayerBorder, lastLayerBorder, x, z);
			FillCaves(block.biom, cavesHeight, lastLayerBorder, x, z);
			FillCrevices(block.biom, cavesHeight, firstLayerBorder, lastLayerBorder, x, z);
			FillVegetation(block.biom, crevicesHeight, cavesHeight, lastLayerBorder, x, z);
			FillTrees(block, crevicesHeight, cavesHeight, lastLayerBorder, x, z);
			FillOre(block.biom, lastLayerBorder, x, z);
			SetBlock(glm::ivec3(x, 0, z), Block::Bedrock);
		}
	}
}

void Chunk::SetActive(bool state) {
	if (_state == state)
		return;
	_state = state;
	for (int i = 0; i < 16; i++) {
		_shards[i]->SetActive(state);
	}
}

Block Chunk::GetBlock(const glm::ivec3 pos) {
	if (pos.y < 0)
		return Block::NoChunk;
	if (pos.y >= 256)
		return Block::Air;
	int y = pos.y >> 4;
	return _shards[y]->GetBlock(glm::ivec3(pos.x, pos.y % 16, pos.z));
}

void Chunk::SetBlock(glm::ivec3 pos, Block type) {
	if (pos.y < 0 || pos.y >= 256)
		return;
	int y = pos.y / 16;
	_shards[y]->SetBlock(glm::ivec3(pos.x, pos.y % 16, pos.z), type); //! Conversion plug
}

void Chunk::PlayerSetBlock(glm::ivec3 pos, Block type) {
	if (pos.y < 0 || pos.y >= 256)
		return;
	int y = pos.y / 16;
	_shards[y]->SetBlock(glm::ivec3(pos.x, pos.y % 16, pos.z), type); //! Conversion plug
	_shards[y]->UpdateGeometry();
}
