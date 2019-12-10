#include "World/World.h"
#include "Engine/Locator.hpp"
#include "Utilities/Time.h"
#include "Types.h"

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
//	/ ResourceLoader* r = _game->GetResources();
//	// for (int x = 0; x < 16; x++)
//	// 	for (int y = 0; y < 16; y++)
//	// 		for (int z = 0; z < 16; z++) {
//	// 			int r = rand();
//	// 			if (r % 3 == 0)
//	// 				SetBlock(glm::ivec3(x, y, z), BlockType::Stone);
//	// 			if (r % 7 == 0)
					// SetBlock(glm::ivec3(1, 1, 1), BlockType::Planks);
//				// if (rand() % 2)
//					// _blocks[x][y][z] = BlockType::Planks;
//				// _debugModels[x][y][z] = new RenderModel(_game->GetRenderer(), r->GetShader("Base"), r->GetTexture("Planks"), r->GetGeometry("Box"));
//				// _debugModels[x][y][z]->SetPosition(glm::vec3(globalPos.x * 16 + x, globalPos.y * 16 + y, globalPos.z * 16 + z));
//			// }
	ResourceLoader* r = _game->GetResources();
	MapGeneration* mp = _game->GetGeneration();
	mp->Generation(globalPos.x + 1, globalPos.y + 1);
	for (int x = 0; x < 16; x++)
		for (int y = 0; y < 16; y++)
			for (int z = 0; z < 16; z++) {
				// if (y < 10)
					// SetBlock(glm::ivec3(x, y, z), BlockType::Stone);
				// else -- для биомов
				// {

				// }
				SetBlock((glm::ivec3(x, (int)floorf((*mp->umap)[glm::ivec2(globalPos.x, globalPos.y)]->elevation * 10),
										z)), BlockType::Stone);
			}
	UpdateGeometry(globalPos);
}

Shard::~Shard() {
	// if (_state) {
		// Remove model from renderer
	// }
	//! THIS IS DEBUG CODE
	// for (int x = 0; x < 16; x++)
	// 	for (int y = 0; y < 16; y++)
	// 		for (int z = 0; z < 16; z++) {
	// 			_game->GetRenderer()->RemoveModel(_debugModels[x][y][z]);
	// 			delete _debugModels[x][y][z];
	// 		}
};

void Shard::SetActive(bool state) {
	_state = state;
	if (_state) {
		// Add model to renderer
	} else {
		// Remove model from renderer
	}
}

void Shard::UpdateGeometry(glm::ivec3 globalPos) {
	//? Remove models from renderer
	for (RenderModel* model : _models) {
		_game->GetRenderer()->RemoveModel(model);
		delete model;
	}
	_models.clear();
	//? Recalculate the model
	std::vector<float> cube = Geometry::ReadGeometry("./resources/Models/Box.obj");
	std::vector<float> arr;
	std::vector<float> temp;
	for (int t = (int)BlockType::First + 1; t <= (int)BlockType::Last; t++) {
		if (!_blockTypePresent[t]) {
			continue;
		}
		for (int x = 0; x < 16; x++)
			for (int y = 0; y < 16; y++)
				for (int z = 0; z < 16; z++) {
					if ((int)_blocks[x][y][z] == t) {
						temp = cube;

						for (int i = 0; i < 288; i += 8) {
							temp[i + 0] += x;
							temp[i + 1] += y;
							temp[i + 2] += z;
						}
						arr.reserve(temp.size());
						arr.insert(arr.end(), temp.begin(), temp.end());
						temp.clear();
					}
				}
		Geometry* g = new Geometry(arr);
		ResourceLoader* r = _game->GetResources();
		RenderModel* model = new RenderModel(_game->GetRenderer(), r->GetShader("Base"), r->GetTexture((BlockType)t), g);
		model->SetPosition(globalPos * 16);
		_models.push_back(model);
	}
	// if (_state) {
	// 	// Add model to renderer
	// }
}

// std::vector<float> GenerateBlock(int x, int y, int z) {

// }

Block* Shard::GetBlock(glm::ivec3 pos) {
	return _game->GetResources()->GetBlock(_blocks[pos.x][pos.y][pos.z]);
}

void Shard::SetBlock(glm::ivec3 pos, BlockType type) {
	_blocks[pos.x][pos.y][pos.z] = type;
	//! Uncomment this later for optimization
	if (type != BlockType::Air && !HasType(type)) {
		_blockTypePresent[(int)type] = true;
	}
}

bool Shard::HasType(BlockType type) {
	for (int i = (int)BlockType::First + 1; i <= (int)BlockType::Last; i++) {
		if (_blockTypePresent[i] == (int)type - 1) {
			return true;
		}
	}
	return false;
}