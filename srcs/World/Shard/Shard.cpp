// #include "World/World.h"
#include "Types.h"
#include "Utilities/Log.h"
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
		//! THIS IS A FIX FOR A GHOST BUG
		//TODO Fix it
		//? Description:
		//? ShadowRenderer crashed on glDrawArrays
		//? How is this even possible?
		// _game->GetRenderer()->RemoveModel(model);
		delete model->GetGeometry();
		delete model;
	}
};

// void Shard::SetActive(bool newState) {
// 	GLRenderer* r = _game->GetRenderer();
// 	if (_state == false && newState == true) {
// 		UpdateGeometry();
// 	} else if (_state == true && newState == false) {
// 		for (RenderModel* model : _models) {
// 			delete model;
// 		}
// 		_models.clear();
// 	}
// 	_state = newState;
// }

Block Shard::GetBlock(const glm::ivec3 localP) {
	return _blocks[localP.x][localP.y][localP.z];
}

void Shard::SetBlock(glm::ivec3 localP, Block newBlock) {
	Block oldBlock = _blocks[localP.x][localP.y][localP.z];
	if (newBlock == oldBlock) {
		return;
	}
	_blockCount[(int)oldBlock]--;
	_blockCount[(int)newBlock]++;
	_blocks[localP.x][localP.y][localP.z] = newBlock;
}

bool Shard::HasBlock(Block block) {
	if (_blockCount[(int)block] > 0) {
		return true;
	}
	return false;
}

uint Shard::CountBlock(Block block) {
	return _blockCount[(uint)block];
}
