// #include "World/World.h"
#include "Types.h"
#include "Engine/Locator.hpp"
#include "Utilities/Time.h"
#include "World/Shard.h"

Shard::Shard(Game* game, glm::ivec3 pos) {
	_state = false;
	_game = game;
	_position = pos;
}

Shard::~Shard() {
	SetActive(false);
};

void Shard::Generate() {
	for (int x = 0; x < 16; x++)
		for (int y = 0; y < 16; y++)
			for (int z = 0; z < 16; z++) {
				if (_position.y < 7)
					SetBlock(glm::ivec3(x, y, z), BlockType::Stone);
				// else if (_position.y < 8) {
				// 	int r = rand();
				// 	if (r % 16 == 0)
				// 		SetBlock(glm::ivec3(x, y, z), BlockType::Dirt);
				// }
			}
}

void Shard::SetActive(bool state) {
	GLRenderer* r = _game->GetRenderer();
	if (_state == false && state == true) {
		UpdateGeometry();
		for (RenderModel* model : _models) {
			r->AddModel(model);
		}
	} else if (_state == true && state == false) {
		for (RenderModel* model : _models) {
			r->RemoveModel(model);
		}
	}
	_state = state;
}

BlockType Shard::GetBlock(glm::ivec3 pos) {
	// Locator::getLogger()->LogWarning("[Returning block: " + std::to_string((uint)_blocks[pos.x][pos.y][pos.z]) + " ][");
	return _blocks[pos.x][pos.y][pos.z];
}

void Shard::SetBlock(glm::ivec3 pos, BlockType type) {
	if (type != BlockType::Air) {
		_blockTypePresent[(uint)type]++;
	} else {
		BlockType removed = _blocks[pos.x][pos.y][pos.z];
		_blockTypePresent[(uint)removed]--;
	}
	_blocks[pos.x][pos.y][pos.z] = type;
}

bool Shard::HasType(BlockType type) {
	if (_blockTypePresent[(uint)type] > 0) {
		return true;
	}
	return false;
}

uint Shard::CountType(BlockType type) {
	return _blockTypePresent[(uint)type];
}
