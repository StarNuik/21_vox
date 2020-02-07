#include "Types.h"
#include "World/World.h"
#include "World/Shard.h"
#include "World/Chunk.h"
#include "Engine/Game.h"
#include "Generation/MapGeneration.h"
// #include "Generation/TreeTypes.h"

Chunk::Chunk(Game* game, glm::ivec2 pos) {
	_state = false;
	_game = game;
	_position = pos;
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
	World* w = _game->GetWorld();
	MapGeneration mp = *_game->GetGeneration();
	MapGeneration::StoredMapData block;
	MapGeneration::StoredOreData ore;

	for (int x = 0; x < 16; x++)
	{
		for (int z = 0; z < 16; z++)
		{
			// int elevation;
			// block = mp.Generation(_position, glm::ivec2(x, z), MapGeneration::Basic); // first layer generation
			// int firstLayerBorder = 40 + block.exactElevation;
			// // for (int y = 1; y < firstLayerBorder; y++)
			// // 	w->SetBlock(glm::ivec3(_position.x * 16 + x, y, _position.y * 16 + z), block.firstBlockLayer);

			// block = mp.Generation(_position, glm::ivec2(x, z), MapGeneration::GrassLand);
			// // std::cout << block.exactElevation << std::endl;
			// for (int y = 0; y < block.exactElevation; y++)
			// 	w->SetBlock(glm::ivec3(_position.x * 16 + x, y, _position.y * 16 + z), block.lastBlockLayer);
			// int cavesDepth = firstLayerBorder - 40;
			// int secondCavesDepth = firstLayerBorder - 40;
			// int crevicesDepth = firstLayerBorder - 15;

			// block = mp.Generation(_position, glm::ivec2(x, z)); //second layer generation
			// int lastLayerBorder = 60 + block.exactElevation - 1;
			// if ((x > 3 && x < 13 && z > 3 && z < 13) && block.treeType != mp.tree.Nothing) { // (x > 3 && x < 13 && z > 3 && z < 13) - a crutch for which trees are not created on the edge of the biome
			// 	for (int y = 0; y < TREE_HEIGHT; y++)
			// 		for (int xn = 0; xn < TREE_SIZE; xn++)
			// 			for (int zn = 0; zn < TREE_SIZE; zn++)
			// 			{
			// 				if (mp.tree.TreeModels[block.treeType][y][xn][zn] != Block::Air)
			// 					w->SetBlock(glm::ivec3(_position.x * 16 + x + xn - 3, y + lastLayerBorder + 1, _position.y * 16 + z + zn - 3), mp.tree.TreeModels[block.treeType][y][xn][zn]);
			// 			}
			// }

			// for (int y = firstLayerBorder; y < lastLayerBorder; y++)
			// 	w->SetBlock(glm::ivec3(_position.x * 16 + x, y, _position.y * 16 + z), block.firstBlockLayer);
			// w->SetBlock(glm::ivec3(_position.x * 16 + x, lastLayerBorder, _position.y * 16 + z), block.lastBlockLayer);

			// if (block.biom != MapGeneration::Ocean)
			// {
			// 	elevation = mp.Generation(_position, glm::ivec2(x, z), MapGeneration::ShapeCaves).exactElevation;// caves generation
			// 	if (elevation != 0)
			// 	{
			// 		int height = mp.Generation(_position, glm::ivec2(x, z), MapGeneration::ElevationCaves).exactElevation;
			// 		cavesDepth += height;
			// 		int cavesHeight = glm::clamp(cavesDepth + elevation + 1, 1, lastLayerBorder);
			// 		for (int y = cavesDepth; y < cavesHeight; y++)
			// 			w->SetBlock(glm::ivec3(_position.x * 16 + x, y, _position.y * 16 + z), BlockType::Air);
			// 	}

			// 	elevation = mp.Generation(_position, glm::ivec2(x, z), MapGeneration::SecondShapeCaves).exactElevation;// caves generation
			// 	if (elevation != 0)
			// 	{
			// 		int height = mp.Generation(_position, glm::ivec2(x, z), MapGeneration::SecondElevationCaves).exactElevation;
			// 		secondCavesDepth += height;
			// 		int cavesHeight = glm::clamp(secondCavesDepth + elevation + 1, 1, lastLayerBorder);
			// 		for (int y = secondCavesDepth; y < cavesHeight; y++)
			// 			w->SetBlock(glm::ivec3(_position.x * 16 + x, y, _position.y * 16 + z), BlockType::Air);
			// 	}
			for (int y = 0; y < 48; y++) {
				ore = mp.OreGeneration(_position, glm::ivec3(x, y, z), y);
				if (ore.type != Block::Air)
					w->SetBlock(glm::ivec3(_position.x * 16 + x, y, _position.y * 16 + z), ore.type);
			}

			// 	elevation = mp.Generation(_position, glm::ivec2(x, z), MapGeneration::Crevices).exactElevation; // Crevices generation
			// 	if (elevation != 0)
			// 	{
			// 		for (int y = cavesDepth; y < 255; y++)
			// 			w->SetBlock(glm::ivec3(_position.x * 16 + x, y, _position.y * 16 + z), BlockType::Air);
			// 	}
			// }

			w->SetBlock(glm::ivec3(_position.x * 16 + x, 0, _position.y * 16 + z), BlockType::Bedrock);
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

void Chunk::SetBlock(glm::ivec3 pos, BlockType type) {
	if (pos.y < 0 || pos.y >= 256)
		return;
	int y = pos.y / 16;
	_shards[y]->SetBlock(glm::ivec3(pos.x, pos.y % 16, pos.z), Block(type)); //! Conversion plug
}

void Chunk::PlayerSetBlock(glm::ivec3 pos, BlockType type) {
	if (pos.y < 0 || pos.y >= 256)
		return;
	int y = pos.y / 16;
	_shards[y]->SetBlock(glm::ivec3(pos.x, pos.y % 16, pos.z), Block(type)); //! Conversion plug
	_shards[y]->UpdateGeometry();
}
