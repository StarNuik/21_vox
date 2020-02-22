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
	MapGeneration::StoredMapData sub;
	MapGeneration::StoredOreData ore;

	for (int x = 0; x < 16; x++)
	{
		for (int z = 0; z < 16; z++)
		{
			int elevation;
			block = mp.Generation(_position, glm::ivec2(x, z), MapGeneration::Basic); // first layer generation
			int firstLayerBorder = MIN_WATER_LEVEL + block.exactElevation;
			for (int y = 1; y < firstLayerBorder; y++)
				SetBlock(glm::ivec3(x, y, z), block.firstBlockLayer);
			int cavesDepth = firstLayerBorder - MIN_WATER_LEVEL;
			int secondCavesDepth = firstLayerBorder - MIN_WATER_LEVEL;
			int crevicesDepth = firstLayerBorder - 30;

			block = mp.Generation(_position, glm::ivec2(x, z)); //second layer generation
			int lastLayerBorder = MAX_WATER_LEVEL + block.exactElevation - 1;
			// std::cout << block.exa
			for (int y = firstLayerBorder; y < lastLayerBorder; y++)
				SetBlock(glm::ivec3(x, y, z), block.firstBlockLayer);

			SetBlock(glm::ivec3(x, lastLayerBorder, z), block.lastBlockLayer);

			if (block.biom == MapGeneration::Swamp && lastLayerBorder < MAX_WATER_LEVEL)
			{
				for (int y = lastLayerBorder + 1; y <= MAX_WATER_LEVEL; y++)
					SetBlock(glm::ivec3(x, y, z), Block::Water);
				SetBlock(glm::ivec3(x, lastLayerBorder, z), Block::Dirt);
			}

			if (block.biom == MapGeneration::River && block.aboveRiverBiome == MapGeneration::HighLand) { // river generation
				sub = mp.Generation(_position, glm::ivec2(x, z), MapGeneration::HighLand);
				int lastSubLayerBorder = MAX_WATER_LEVEL + sub.exactElevation - 1;
				for (int y = lastLayerBorder + (firstLayerBorder % 9) + 1; y < lastSubLayerBorder; y++) {
					SetBlock(glm::ivec3(x, y, z), sub.firstBlockLayer);
				}
				SetBlock(glm::ivec3(x, lastSubLayerBorder, z), sub.lastBlockLayer);
			}

			if (block.biom != MapGeneration::Ocean && block.biom != MapGeneration::River)
			{
				elevation = mp.Generation(_position, glm::ivec2(x, z), MapGeneration::ShapeCaves).exactElevation;// caves generation
				if (elevation != 0)
				{
					int height = mp.Generation(_position, glm::ivec2(x, z), MapGeneration::ElevationCaves).exactElevation;
					cavesDepth += height;
					int cavesHeight = glm::clamp(cavesDepth + elevation + 1, 1, lastLayerBorder);
					for (int y = cavesDepth; y < cavesHeight; y++)
						SetBlock(glm::ivec3(x, y, z), Block::Air);
					__BLOCK_TYPE vegetation = mp.VegetationGeneration(_position, glm::ivec2(x, z), MapGeneration::ElevationCaves);
					if (vegetation != Block::Air)
						SetBlock(glm::ivec3(x, cavesDepth, z), vegetation);
				}

				elevation = mp.Generation(_position, glm::ivec2(x, z), MapGeneration::SecondShapeCaves).exactElevation;// caves generation
				if (elevation != 0)
				{
					int height = mp.Generation(_position, glm::ivec2(x, z), MapGeneration::SecondElevationCaves).exactElevation;
					secondCavesDepth += height;
					int cavesHeight = glm::clamp(secondCavesDepth + elevation + 1, 1, lastLayerBorder);
					for (int y = secondCavesDepth; y < cavesHeight; y++)
						SetBlock(glm::ivec3(x, y, z), Block::Air);
					__BLOCK_TYPE vegetation = mp.VegetationGeneration(_position, glm::ivec2(x, z), MapGeneration::SecondElevationCaves);
					if (vegetation != Block::Air)
						SetBlock(glm::ivec3(x, secondCavesDepth, z), vegetation);
				}

				int crevicesHeight = 0;

				for (int y = crevicesDepth; y < lastLayerBorder + 1; y++) {
						float e = mp.CrevicesGenerations(_position, glm::ivec3(x, y, z)); // Crevices generation
						if (e != -1.f) {
							SetBlock(glm::ivec3(x, y, z), Block::Air);
							crevicesHeight = y;
						}
				}

				if (crevicesHeight < lastLayerBorder && (x > 3 && x < 13 && z > 3 && z < 13) && block.treeType != mp.tree.Nothing) { // (x > 3 && x < 13 && z > 3 && z < 13) - a crutch for which trees are not created on the edge of the biome
					for (int y = 0; y < TREE_HEIGHT; y++)
						for (int xn = 0; xn < TREE_SIZE; xn++)
							for (int zn = 0; zn < TREE_SIZE; zn++)
							{
								if (mp.tree.TreeModels[block.treeType][y][xn][zn] != Block::Air)
									SetBlock(glm::ivec3(x + xn - 3, y + lastLayerBorder + 1, z + zn - 3), mp.tree.TreeModels[block.treeType][y][xn][zn]);
							}
				}
				else if (crevicesHeight < lastLayerBorder) {
					__BLOCK_TYPE vegetation = mp.VegetationGeneration(_position, glm::ivec2(x, z), block.biom);
					if (vegetation != Block::Air)
						SetBlock(glm::ivec3(x, lastLayerBorder + 1, z), vegetation);
				}

				for (int y = 0; y < lastLayerBorder; y++) { // Ore Generation
					__BLOCK_TYPE blockType = GetBlock(glm::ivec3(x, y, z));
					if(blockType == Block::Stone)
					{
						ore = mp.OreGeneration(_position, glm::ivec3(x, y, z), lastLayerBorder);
						if (ore.type != Block::Air)
							SetBlock(glm::ivec3(x, y, z), ore.type);
					}
				}
			}

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
