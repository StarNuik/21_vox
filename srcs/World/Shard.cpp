#include "World/World.h"
#include "Engine/Locator.hpp"
#include "Utilities/Time.h"


Shard::Shard(Game* game, glm::ivec3 globalPos) {
	_state = true;
	_game = game;
	_world = _game->GetWorld();
	//!!!!!! NULLLLLLLLLLLLLLLLLLLLLLLLLLLL
	std::cout << _world << std::endl;
	// Generate self here
	//! THIS IS DEBUG CODE
	for (int x = 0; x < 16; x++)
		for (int y = 0; y < 16; y++)
			for (int z = 0; z < 16; z++) {
				int r = rand();
				if (r % 3 == 0)
					SetBlock(glm::ivec3(x, y, z), BlockType::Stone);
				if (r % 7 == 0)
					SetBlock(glm::ivec3(x, y, z), BlockType::Planks);
			}
	//! Gen first, update later
	// UpdateGeometry(globalPos);
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
	std::vector<float> vertexBuffer;
	for (int t = (int)BlockType::First + 1; t <= (int)BlockType::Last; t++) {
		if (!_blockTypePresent[t]) {
			continue;
		}
		for (int x = 0; x < 16; x++)
			for (int y = 0; y < 16; y++)
				for (int z = 0; z < 16; z++) {
					if ((int)_blocks[x][y][z] == t) {
						std::vector<float> cube = GenerateBlock(globalPos + glm::ivec3(x, y, z));
						for (int i = 0; i < cube.size(); i += 8) {
							cube[i + 0] += x;
							cube[i + 1] += y;
							cube[i + 2] += z;
						}
						vertexBuffer.reserve(cube.size());
						vertexBuffer.insert(vertexBuffer.end(), cube.begin(), cube.end());
					}
				}
		Geometry* g = new Geometry(vertexBuffer);
		ResourceLoader* r = _game->GetResources();
		RenderModel* model = new RenderModel(_game->GetRenderer(), r->GetShader("Base"), r->GetTexture((BlockType)t), g);
		model->SetPosition(globalPos * 16);
		_models.push_back(model);
	}
	// if (_state) {
	// 	// Add model to renderer
	// }
}

std::vector<float> Shard::GenerateBlock(glm::ivec3 globalBlockPos) {
	std::vector<float> res;

	//? Right face
	if (_world->GetBlock(globalBlockPos + glm::ivec3(1, 0, 0)) == BlockType::Air) {
		float *buffer = Geometry::FaceRight();
		res.insert(res.end(), buffer, buffer + 48 * sizeof(float));
	}
	//? Left face
	if (_world->GetBlock(globalBlockPos + glm::ivec3(-1, 0, 0)) == BlockType::Air) {
		float *buffer = Geometry::FaceLeft();
		res.insert(res.end(), buffer, buffer + 48 * sizeof(float));
	}
	//? Top face
	if (_world->GetBlock(globalBlockPos + glm::ivec3(0, -1, 0)) == BlockType::Air) {
		float *buffer = Geometry::FaceTop();
		res.insert(res.end(), buffer, buffer + 48 * sizeof(float));
	}
	//? Bottom face
	if (_world->GetBlock(globalBlockPos + glm::ivec3(0, 1, 0)) == BlockType::Air) {
		float *buffer = Geometry::FaceBottom();
		res.insert(res.end(), buffer, buffer + 48 * sizeof(float));
	}
	//? Front face
	if (_world->GetBlock(globalBlockPos + glm::ivec3(0, 0, 1)) == BlockType::Air) {
		float *buffer = Geometry::FaceFront();
		res.insert(res.end(), buffer, buffer + 48 * sizeof(float));
	}
	//? Back face
	if (_world->GetBlock(globalBlockPos + glm::ivec3(0, 0, -1)) == BlockType::Air) {
		float *buffer = Geometry::FaceBack();
		res.insert(res.end(), buffer, buffer + 48 * sizeof(float));
	}
	return res;
}

BlockType Shard::GetBlock(glm::ivec3 pos) {
	return _blocks[pos.x][pos.y][pos.z];
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