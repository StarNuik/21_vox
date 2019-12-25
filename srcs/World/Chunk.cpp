#include "Types.h"
#include "World/World.h"
#include "World/Shard.h"
#include "World/Chunk.h"
#include "Engine/Game.h"
#include "Generation/MapGeneration.h"
#include "Generation/BiomeDefine.h"

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
	// for (int y = 0; y < 16; y++) {
		// _shards[y]->Generate();
	// }
	World* w = _game->GetWorld();
	MapGeneration mp = *_game->GetGeneration();
	MapGeneration::StoredMapData block;

	for (int x = 0; x < 16; x++) {
		for (int z = 0; z < 16; z++) {
			
			block = mp.Generation(_position, glm::ivec2(x, z), MapGeneration::Basic); // first layer generation
			int elevation = glm::clamp((int)block.elevation, 0, 255); // max world height == 255
			int firstLayerBorder = 40 + block.elevation;
			for (int y = 1; y < firstLayerBorder; y++){
				w->SetBlock(glm::ivec3(_position.x * 16 + x, y, _position.y * 16 + z), block.firstBlockLayer);
			}
			int cavesDepth = firstLayerBorder - 40;
			int secondCavesDepth = firstLayerBorder - 40;
			int crevicesDepth = firstLayerBorder - 15;


			block = mp.Generation(_position, glm::ivec2(x, z)); //second layer generation
			elevation = glm::clamp((int)block.elevation, 0, 255); // max world height == 255
			int lastLayerBorder = 60 + elevation - 1;
			for (int y = firstLayerBorder; y < lastLayerBorder; y++){
				w->SetBlock(glm::ivec3(_position.x * 16 + x, y, _position.y * 16 + z), block.firstBlockLayer);
			}
			w->SetBlock(glm::ivec3(_position.x * 16 + x, lastLayerBorder, _position.y * 16 + z), block.lastBlockLayer);

			if (block.biom != MapGeneration::Ocean) // caves generation
			{
				float e = mp.Generation(_position, glm::ivec2(x, z), MapGeneration::ShapeCaves).elevation;
				elevation = glm::clamp((int)e, 0, 255);
				if (elevation != 0)
				{
					float height = mp.Generation(_position, glm::ivec2(x, z), MapGeneration::ElevationCaves).elevation;
					int h = glm::clamp((int)height, 1, 255);
					cavesDepth += h;
					cavesDepth = glm::clamp(cavesDepth, 1, 255);
					int cavesHeight = glm::clamp(cavesDepth + elevation + 1, 1, lastLayerBorder);
					for (int y = cavesDepth; y < cavesHeight; y++){
						w->SetBlock(glm::ivec3(_position.x * 16 + x, y, _position.y * 16 + z), BlockType::Air);
					}
				}

				e = mp.Generation(_position, glm::ivec2(x, z), MapGeneration::SecondShapeCaves).elevation;
				elevation = glm::clamp((int)e, 0, 255);
				if (elevation != 0)
				{
					float height = mp.Generation(_position, glm::ivec2(x, z), MapGeneration::SecondElevationCaves).elevation;
					int h = glm::clamp((int)height, 1, 255);
					secondCavesDepth += h;
					int cavesHeight = glm::clamp(secondCavesDepth + elevation + 1, 1, lastLayerBorder);
					for (int y = secondCavesDepth; y < cavesHeight; y++){
						w->SetBlock(glm::ivec3(_position.x * 16 + x, y, _position.y * 16 + z), BlockType::Air);
					}
				}
			}

			elevation = glm::clamp((int)mp.Generation(_position, glm::ivec2(x, z), MapGeneration::Crevices).elevation, 0, 255); // Crevices generation
			if (elevation != 0.f)
			{
				for (int y = cavesDepth; y < 255; y++){
					w->SetBlock(glm::ivec3(_position.x * 16 + x, y, _position.y * 16 + z), BlockType::Air);
				}
			}
			else if (block.biom == MapGeneration::GrassLand && (x > 3 && x < 13 && z > 3 && z < 13) // (x > 3 && x < 13 && z > 3 && z < 13) - a crutch for which trees are not created on the edge of the biome
			&& mp.Generation(_position, glm::ivec2(x, z), MapGeneration::Tree).elevation == 1.f)
			{
				int treeHeight = lastLayerBorder + 5;
				for (int y = lastLayerBorder + 1; y < treeHeight; y++){
					w->SetBlock(glm::ivec3(_position.x * 16 + x, y, _position.y * 16 + z), BlockType::Log);
					int globalTreeHeight = (treeHeight << 1) - y - 1;
					int distanceToLogX = x - 2;
					int distanceToLogZ = z - 2;

					// Leaves for tree generation, sorry for "IF"
					for (int xn = distanceToLogX; xn < distanceToLogX + 5; xn++){
						for (int zn = distanceToLogZ; zn < distanceToLogZ + 5; zn++){
							if ((!((xn == distanceToLogX && zn == distanceToLogZ) || (xn == distanceToLogX + 4 && zn == distanceToLogZ)
							|| (xn == distanceToLogX + 4 && zn == distanceToLogZ + 4) || (xn == distanceToLogX && zn == distanceToLogZ + 4)))
							)
								w->SetBlock(glm::ivec3(_position.x * 16 + xn, globalTreeHeight, _position.y * 16 + zn), BlockType::Leaves);
						}
					}
					for (int xn = distanceToLogX + 1; xn < distanceToLogX + 4; xn++){
						for (int zn = distanceToLogZ + 1; zn < distanceToLogZ + 4; zn++){
							w->SetBlock(glm::ivec3(_position.x * 16 + xn, globalTreeHeight + 1, _position.y * 16 + zn), BlockType::Leaves);
						}
					}
				}
			}

			w->SetBlock(glm::ivec3(_position.x * 16 + x, 0, _position.y * 16 + z), BlockType::Bedrock);
		}
	}
}

void Chunk::SetActive(bool state) {
	_state = state;
	for (int i = 0; i < 16; i++) {
		_shards[i]->SetActive(state);
	}
}

BlockType Chunk::GetBlock(glm::ivec3 pos) {
	if (pos.y < 0)
		return BlockType::Stone; //! Is this okay?
	if (pos.y >= 256)
		return BlockType::Air;
	int y = pos.y / 16;
	return _shards[y]->GetBlock(glm::ivec3(pos.x, pos.y % 16, pos.z));
}

void Chunk::SetBlock(glm::ivec3 pos, BlockType type) {
	if (pos.y < 0 || pos.y >= 256)
		return;
	int y = pos.y / 16;
	_shards[y]->SetBlock(glm::ivec3(pos.x, pos.y % 16, pos.z), type);
}

void Chunk::PlayerSetBlock(glm::ivec3 pos, BlockType type) {
	int y = pos.y / 16;
	_shards[y]->SetBlock(glm::ivec3(pos.x, pos.y % 16, pos.z), type);
	_shards[y]->UpdateGeometry();
}
