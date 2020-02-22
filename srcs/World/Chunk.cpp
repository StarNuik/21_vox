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
	MapGeneration& mp = *_game->GetGeneration();
	MapGeneration::StoredMapData block;
	MapGeneration::StoredMapData sub;
	MapGeneration::StoredOreData ore;
	
	//! Move to methods
	for (int x = 0; x < 16; x++)
	{
		for (int z = 0; z < 16; z++)
		{
			//? First layer (Stone)
			block = mp.Generation(_position, glm::ivec2(x, z), MapGeneration::Basic); // first layer generation
			// const int firstLayerBorder = MIN_WATER_LEVEL + block.exactElevation;
			int firstLayerBorder = MIN_WATER_LEVEL + block.exactElevation;
			for (int y = 1; y < firstLayerBorder; y++)
				w->SetBlock(glm::ivec3(_position.x * 16 + x, y, _position.y * 16 + z), block.firstBlockLayer);
			
			//? Second layer (Biome dependent)
			block = mp.Generation(_position, glm::ivec2(x, z)); //second layer generation
			int lastLayerBorder = MAX_WATER_LEVEL + block.exactElevation - 1;
			for (int y = firstLayerBorder; y < lastLayerBorder; y++)
				w->SetBlock(glm::ivec3(_position.x * 16 + x, y, _position.y * 16 + z), block.firstBlockLayer);
			
			//? Surface
			w->SetBlock(glm::ivec3(_position.x * 16 + x, lastLayerBorder, _position.y * 16 + z), block.lastBlockLayer);

			//? Set swamp water
			if (block.biom == MapGeneration::Swamp && lastLayerBorder < MAX_WATER_LEVEL)
			{
				for (int y = lastLayerBorder + 1; y <= MAX_WATER_LEVEL; y++)
					w->SetBlock(glm::ivec3(_position.x * 16 + x, y, _position.y * 16 + z), Block::Water);
				w->SetBlock(glm::ivec3(_position.x * 16 + x, lastLayerBorder, _position.y * 16 + z), Block::Dirt);
			}

			//? Rivers
			if (block.biom == MapGeneration::River && block.aboveRiverBiome == MapGeneration::HighLand) { // river generation
				sub = mp.Generation(_position, glm::ivec2(x, z), MapGeneration::HighLand);
				int lastSubLayerBorder = MAX_WATER_LEVEL + sub.exactElevation - 1;
				for (int y = lastLayerBorder + (firstLayerBorder % 9) + 1; y < lastSubLayerBorder; y++) {
					w->SetBlock(glm::ivec3(_position.x * 16 + x, y, _position.y * 16 + z), sub.firstBlockLayer);
				}
				w->SetBlock(glm::ivec3(_position.x * 16 + x, lastSubLayerBorder, _position.y * 16 + z), sub.lastBlockLayer);
			}

			//? Big IF
			int cavesHeight = 0;
			if (block.biom != MapGeneration::Ocean && block.biom != MapGeneration::River)
			{
				//? Caves
				bool cavesStart = false;
				int cavesDepth = 0;
				for (int y = 1; y < lastLayerBorder + (lastLayerBorder % 2); y++) { // caves generation
					if (mp.CavesGeneration(_position, glm::ivec3(x, y, z))) {
						w->SetBlock(glm::ivec3(_position.x * 16 + x, y, _position.y * 16 + z), Block::Air);
						cavesHeight = y;
						cavesStart = true;
					}
					if (cavesStart == true && cavesDepth == 0) { // vegetation for caves
						cavesDepth = y;
						__BLOCK_TYPE vegetation = mp.VegetationGeneration(_position, glm::ivec2(x, z), MapGeneration::Caves);
						if (vegetation != Block::Air)
							w->SetBlock(glm::ivec3(_position.x * 16 + x, cavesDepth, _position.y * 16 + z), vegetation);
					}
					
				}

				//? Ravines
				int crevicesHeight = 0;
				int crevicesDepth = firstLayerBorder - 30;
				for (int y = crevicesDepth; y < lastLayerBorder + 1; y++) {  // Crevices generation
						if (mp.CrevicesGeneration(_position, glm::ivec3(x, y, z))) {
							w->SetBlock(glm::ivec3(_position.x * 16 + x, y, _position.y * 16 + z), Block::Air);
							crevicesHeight = y;
						}
				}

				//? Trees
				if (cavesHeight < lastLayerBorder && crevicesHeight < lastLayerBorder && (x > 3 && x < 13 && z > 3 && z < 13) && block.treeType != mp.tree.Nothing) { // (x > 3 && x < 13 && z > 3 && z < 13) - a crutch for which trees are not created on the edge of the biome
					for (int y = 0; y < TREE_HEIGHT; y++)
						for (int xn = 0; xn < TREE_SIZE; xn++)
							for (int zn = 0; zn < TREE_SIZE; zn++)
							{
								if (mp.tree.TreeModels[block.treeType][y][xn][zn] != Block::Air)
									w->SetBlock(glm::ivec3(_position.x * 16 + x + xn - 3, y + lastLayerBorder + 1, _position.y * 16 + z + zn - 3), mp.tree.TreeModels[block.treeType][y][xn][zn]);
							}
				}
				//? Vegetation
				else if (cavesHeight < lastLayerBorder && crevicesHeight < lastLayerBorder) { // Vegetation generation
					__BLOCK_TYPE vegetation = mp.VegetationGeneration(_position, glm::ivec2(x, z), block.biom);
					__BLOCK_TYPE blockType = w->GetBlock(glm::ivec3(_position.x * 16 + x, lastLayerBorder + 1, _position.y * 16 + z));
					if (vegetation != Block::Air && blockType != Block::Water)
						w->SetBlock(glm::ivec3(_position.x * 16 + x, lastLayerBorder + 1, _position.y * 16 + z), vegetation);
				}

				//? Ore
				for (int y = 0; y < lastLayerBorder; y++) { // Ore Generation
					__BLOCK_TYPE blockType = w->GetBlock(glm::ivec3(_position.x * 16 + x, y, _position.y * 16 + z));
					if(blockType == Block::Stone)
					{
						ore = mp.OreGeneration(_position, glm::ivec3(x, y, z), lastLayerBorder);
						if (ore.type != Block::Air)
							w->SetBlock(glm::ivec3(_position.x * 16 + x, y, _position.y * 16 + z), ore.type);
					}
				}

			}

			w->SetBlock(glm::ivec3(_position.x * 16 + x, 0, _position.y * 16 + z), Block::Bedrock);
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
