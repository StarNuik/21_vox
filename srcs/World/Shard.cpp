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
	MapGeneration* mp = _game->GetGeneration();
	mp->Generation(globalPos.x + 1, globalPos.y + 1, mp->umap);
	// for (auto pair : (*mp->umap))
	// {
	// 	std::cout << "| x: " << pair.first.x << "| y: " << pair.first.y << "| z: " << pair.second->elevation << std::endl;
	// }
	// std::cout << (*mp->umap)[glm::ivec2(globalPos.x, globalPos.y)]->elevation << std::endl;
	// std::cout << mp->umap->at(glm::ivec2(globalPos.x, globalPos.y))->elevation << std::endl;
	// mp->Generation(globalPos.x, globalPos.y, mp->umap);
	// std::cout << mp->umap << std::endl;
	// for (auto pair : *(mp->umap))
	 	// std::cout << "| x: " << pair.first.x << "| y: " << pair.first.y << "| z: " << pair.second->elevation << std::endl;
	for (int x = 0; x < 16; x++)
		for (int y = 0; y < 16; y++)
			for (int z = 0; z < 16; z++) {
				_blocks[x][y][z] = BLOCK_TYPE::PLANKS;
				_debugModels[x][y][z] = new RenderModel(_game->GetRenderer(), r->GetShader("Base"), r->GetTexture("Planks"), r->GetGeometry("Box"));
				_debugModels[x][y][z]->SetPosition(glm::vec3(globalPos.x * 16 + x, (*mp->umap)[glm::ivec2(globalPos.x, globalPos.y)]->elevation * 16.f + y, globalPos.z * 16 + z));
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
	// Remove model from renderer
	// Recalculate the model
	// if (_state) {
		// Add model to renderer
	// }
}

Block* Shard::GetBlock(glm::ivec3 pos) {
	return _game->GetResources()->GetBlock(_blocks[pos.x][pos.y][pos.z]);
}

void Shard::SetBlock(glm::ivec3 pos, BLOCK_TYPE type) {
	_blocks[pos.x][pos.y][pos.z] = type;
}