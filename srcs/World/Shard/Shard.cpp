// #include "World/World.h"
#include "Types.h"
// #include "Utilities/Locator.hpp"
#include "Utilities/Time.h"
#include "World/Shard.h"
#include "Render/RenderModel.h"
#include "Engine/Game.h"
#include "Render/GLRenderer.h"
#include "Render/Geometry.h"

Shard::Shard(Game* game, glm::ivec3 pos) {
	_state = false;
	_game = game;
	_position = pos;
}

Shard::~Shard() {
	for (RenderModel* model : _models) {
		delete model->GetGeometry();
		delete model;
	}
	SetActive(false);
};

void Shard::Generate() {}

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
