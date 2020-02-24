#include "Types.h"
#include "World/Chunk.h"

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
	int height = lastLayerBorder + (lastLayerBorder % 2);
	if (biome == MapGeneration::ChineseMountains)
		height = MAX_WATER_LEVEL;
	for (int y = 1; y < height; y++) { // caves generation
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
