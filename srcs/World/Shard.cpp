#include "World/World.h"
#include "Engine/Locator.hpp"

Shard::Shard(Game* game, glm::ivec3 globalPos) {
	_state = true;
	_game = game;
	// Generate self here
	//! THIS IS DEBUG CODE
	// for (int x = 0; x < 16; x++)
	// 	for (int y = 0; y < 16; y++)
	// 		for (int z = 0; z < 16; z++) {
	// 			_blocks[x][y][z] = BLOCK_TYPE::STONE;
	// 		}
	ResourceLoader* r = _game->GetResources();
	for (int x = 0; x < 16; x++)
		for (int y = 0; y < 16; y++)
			for (int z = 0; z < 16; z++) {
				_blocks[x][y][z] = BlockType::Planks;
				_debugModels[x][y][z] = new RenderModel(_game->GetRenderer(), r->GetShader("Base"), r->GetTexture("Planks"), r->GetGeometry("Box"));
				_debugModels[x][y][z]->SetPosition(glm::vec3(globalPos.x * 16 + x, globalPos.y * 16 + y, globalPos.z * 16 + z));
			}
}

Shard::~Shard() {
	// if (_state) {
		// Remove model from renderer
	// }
	//! THIS IS DEBUG CODE
	for (int x = 0; x < 16; x++)
		for (int y = 0; y < 16; y++)
			for (int z = 0; z < 16; z++) {
				_game->GetRenderer()->RemoveModel(_debugModels[x][y][z]);
				delete _debugModels[x][y][z];
			}
};

void Shard::SetActive(bool state) {
	_state = state;
	if (_state) {
		// Add model to renderer
	} else {
		// Remove model from renderer
	}
}

void Shard::UpdateGeometry() {
	//? Remove models from renderer
	if (_models.size()) {
		for (RenderModel* model : _models) {
			_game->GetRenderer()->RemoveModel(model);
			delete model;
		}
		_models.clear();
	}
	//? Recalculate the model

	// if (_state) {
		// Add model to renderer
	// }
}

Block* Shard::GetBlock(glm::ivec3 pos) {
	return _game->GetResources()->GetBlock(_blocks[pos.x][pos.y][pos.z]);
}

void Shard::SetBlock(glm::ivec3 pos, BlockType type) {
	_blocks[pos.x][pos.y][pos.z] = type;
	//! Uncomment this later for optimization
	// if (type != BlockType::Air && !HasType(type)) {
	// 	_blockTypePresent[(int)type] = true;
	// }
}

bool Shard::HasType(BlockType type) {
	for (int i = (int)BlockType::First + 1; i <= (int)BlockType::Last; i++) {
		if (_blockTypePresent[i] == (int)type - 1) {
			return true;
		}
	}
	return false;
}