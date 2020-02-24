#include "Types.h"
#include "World/World.h"
#include "World/Shard.h"
#include "World/Chunk.h"
#include "Engine/Game.h"
// #include "Generation/MapGeneration.h"
// #include "Generation/TreeTypes.h"

Chunk::Chunk(Game* game, glm::ivec2 pos) {
	_state = false;
	_game = game;
	_position = pos;
	_mp = _game->GetGeneration();
	state = AwaitingGeneration;
	for (int y = 0; y < 16; y++) {
		_shards[y] = new Shard(game, glm::ivec3(pos.x, y, pos.y));
	}
}

Chunk::~Chunk() {
	for (int y = 0; y < 16; y++) {
		delete _shards[y];
	}
}

void Chunk::FillFirstLayer(int& firstLayerBorder, const int x, const int z)
{
	MapGeneration::StoredMapData block = _mp->Generation(_position, glm::ivec2(x, z), MapGeneration::Basic);
	firstLayerBorder = MIN_WATER_LEVEL + block.exactElevation;
	for (int y = 1; y < firstLayerBorder; y++)
		SetBlock(glm::ivec3(x, y, z), block.firstBlockLayer);
}

void Chunk::FillSecondLayer(MapGeneration::StoredMapData& block, int& lastLayerBorder, const int firstLayerBorder, const int x, const int z)
{
	block = _mp->Generation(_position, glm::ivec2(x, z));
	lastLayerBorder = MAX_WATER_LEVEL + block.exactElevation - 1;
	for (int y = firstLayerBorder; y < lastLayerBorder; y++)
		SetBlock(glm::ivec3(x, y, z), block.firstBlockLayer);
	SetBlock(glm::ivec3(x, lastLayerBorder, z), block.lastBlockLayer);
}

void Chunk::FillHighLandAboveTheRiver(const MapGeneration::StoredMapData block, const int firstLayerBorder, const int lastLayerBorder, const int x, const int z)
{
	if (block.biom != MapGeneration::River || block.aboveRiverBiome != MapGeneration::HighLand)
		return;

	MapGeneration::StoredMapData sub = _mp->Generation(_position, glm::ivec2(x, z), MapGeneration::HighLand);
	int lastSubLayerBorder = MAX_WATER_LEVEL + sub.exactElevation - 1;
	for (int y = lastLayerBorder + (firstLayerBorder % 9) + 1; y < lastSubLayerBorder; y++) {
		SetBlock(glm::ivec3(x, y, z), sub.firstBlockLayer);
	}
	SetBlock(glm::ivec3(x, lastSubLayerBorder, z), sub.lastBlockLayer);
}

void Chunk::FillSwampWater(const MapGeneration::GenerationType biome, const int lastLayerBorder, const int x, const int z)
{
	if (biome != MapGeneration::Swamp || lastLayerBorder >= MAX_WATER_LEVEL)
		return;
	for (int y = lastLayerBorder + 1; y <= MAX_WATER_LEVEL; y++)
		SetBlock(glm::ivec3(x, y, z), Block::Water);
	SetBlock(glm::ivec3(x, lastLayerBorder, z), Block::Dirt);
}



void Chunk::FillCaves(const MapGeneration::GenerationType biome, int& cavesHeight, const int lastLayerBorder, const int x, const int z)
{
	if (biome == MapGeneration::Ocean || biome == MapGeneration::River)
		return;

	bool cavesStart = false;
	int cavesDepth = 0;
	for (int y = 1; y < lastLayerBorder + (lastLayerBorder % 2); y++) { // caves generation
		if (_mp->CavesGeneration(_position, glm::ivec3(x, y, z))) {
			SetBlock(glm::ivec3(x, y, z), Block::Air);
			cavesHeight = y;
			cavesStart = true;
		}
		if (cavesStart == true && cavesDepth == 0) { // vegetation for caves
			cavesDepth = y;
			__BLOCK_TYPE vegetation = _mp->VegetationGeneration(_position, glm::ivec2(x, z), MapGeneration::Caves);
			if (vegetation != Block::Air)
				SetBlock(glm::ivec3(x, cavesDepth, z), vegetation);
		}
		
	}

}

void Chunk::FillCrevices(const MapGeneration::GenerationType biome, int& crevicesHeight, const int firstLayerBorder ,const int lastLayerBorder, const int x, const int z)
{
	if (biome == MapGeneration::Ocean || biome == MapGeneration::River)
		return;

	int crevicesDepth = firstLayerBorder - 30;
	for (int y = crevicesDepth; y < lastLayerBorder + 1; y++) {
		if (_mp->CrevicesGeneration(_position, glm::ivec3(x, y, z))) {
			SetBlock(glm::ivec3(x, y, z), Block::Air);
			crevicesHeight = y;
		}
	}
}

void Chunk::FillTrees(const MapGeneration::StoredMapData block, const int crevicesHeight, const int cavesHeight, const int lastLayerBorder, const int x, const int z)
{
	if (block.biom == MapGeneration::Ocean || block.biom == MapGeneration::River
		|| cavesHeight >= lastLayerBorder || crevicesHeight >= lastLayerBorder
			|| block.treeType == Trees::Nothing || (x <= 3 || x >= 13 || z <= 3 || z >= 13)) // (x > 3 && x < 13 && z > 3 && z < 13) - a crutch for which trees are not created on the edge of the biome
		return;

	for (int y = 0; y < TREE_HEIGHT; y++)
		for (int xn = 0; xn < TREE_SIZE; xn++)
			for (int zn = 0; zn < TREE_SIZE; zn++)
			{
				if (_mp->tree.TreeModels[block.treeType][y][xn][zn] != Block::Air)
					SetBlock(glm::ivec3(x + xn - 3, y + lastLayerBorder + 1, z + zn - 3), _mp->tree.TreeModels[block.treeType][y][xn][zn]);
			}
}

void Chunk::FillVegetation(const MapGeneration::GenerationType biome, const int crevicesHeight, const int cavesHeight, const int lastLayerBorder, const int x, const int z)
{
	if (biome == MapGeneration::Ocean || biome == MapGeneration::River
		|| cavesHeight >= lastLayerBorder || crevicesHeight >= lastLayerBorder)
		return;

	__BLOCK_TYPE vegetation = _mp->VegetationGeneration(_position, glm::ivec2(x, z), biome);
	__BLOCK_TYPE blockType = GetBlock(glm::ivec3(x, lastLayerBorder + 1, z));
	if (vegetation != Block::Air && blockType != Block::Water)
			SetBlock(glm::ivec3(x, lastLayerBorder + 1, z), vegetation);
}

void Chunk::FillOre(const MapGeneration::GenerationType biome, const int lastLayerBorder, const int x, const int z)
{
	if (biome == MapGeneration::Ocean || biome == MapGeneration::River)
		return;
	
	MapGeneration::StoredOreData ore;
	for (int y = 0; y < lastLayerBorder; y++) { // Ore Generation
		__BLOCK_TYPE blockType = GetBlock(glm::ivec3(x, y, z));
		if(blockType == Block::Stone)
		{
			ore = _mp->OreGeneration(_position, glm::ivec3(x, y, z), lastLayerBorder);
			if (ore.type != Block::Air)
				SetBlock(glm::ivec3(x, y, z), ore.type);
		}
	}
}

void Chunk::Generate() {GenerateBlocks();}

void Chunk::GenerateBlocks() {
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

void Chunk::GenerateVertices() {
	for (int y = 0; y < 16; y++) {
		_shards[y]->GenerateVertices();
	}
}

void Chunk::GenerateModels() {
	for (int y = 0; y < 16; y++) {
		_shards[y]->GenerateModels();
	}
}

// void Chunk::SetActive(bool state) {
// 	if (_state == state)
// 		return;
// 	_state = state;
// 	for (int i = 0; i < 16; i++) {
// 		_shards[i]->SetActive(state);
// 	}
// }

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
