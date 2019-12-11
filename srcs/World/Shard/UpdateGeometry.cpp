#include "Types.h"
#include "World/Shard.h"
#include "Render/Objects.h"
#include "Engine/Locator.hpp"

void Shard::UpdateGeometry() {
	GLRenderer* r = _game->GetRenderer();
	//? If shard is active, remove models from the renderer
	if (_state) {
		for (RenderModel* model : _models) {
			r->RemoveModel(model);
		}
	}
	//? Delete old geometry and models
	for (RenderModel* model : _models) {
		delete model->GetGeometry();
		delete model;
	}
	_models.clear();
	//! Otherwise _models.push_back(model) segfaults
	_models = std::vector<RenderModel*>();
	//? For every block type
	for (uint t = (uint)BlockType::First + 1; t <= (uint)BlockType::Last; t++) {
		if (!HasType((BlockType)t))
			continue;
		RenderModel* model = GenerateModelOfType((BlockType)t);
		_models.reserve(sizeof(RenderModel*));
		_models.push_back(model);
	}
	//? If shard is active, add models to the renderer
	if (_state) {
		for (RenderModel* model : _models) {
			r->AddModel(model);
		}
	}
}

RenderModel* Shard::GenerateModelOfType(BlockType type) {
	GLRenderer* r = _game->GetRenderer();
	ResourceLoader* rs = _game->GetResources();
	World* w = _game->GetWorld();
	uint count = CountType(type);
	std::vector<float> vertexBuffer;

	for (int x = 0; x < 16 && count > 0; x++)
		for (int y = 0; y < 16 && count > 0; y++)
			for (int z = 0; z < 16 && count > 0; z++) {
				if (_blocks[x][y][z] != type)
					continue;
				// std::vector<float> block = GenerateBlock(w, _position * 16 + glm::ivec3(x, y, z));
				std::vector<float> block = GenerateGeometryFor(type, w, _position * 16 + glm::ivec3(x, y, z));
				if (block.size() == 0)
					continue;
				for (int i = 0; i < block.size(); i += 8) {
					block[i + 0] += (float)x;
					block[i + 1] += (float)y;
					block[i + 2] += (float)z;
				}
				vertexBuffer.reserve(block.size());
				vertexBuffer.insert(vertexBuffer.end(), block.begin(), block.end());
				count--;
			}
	Geometry* g = new Geometry(vertexBuffer);
	RenderModel* model = new RenderModel(r, rs->GetShader("Base"), rs->GetTexture(type), g);
	model->SetPosition(_position * 16);
	return (model);
};

std::vector<float> Shard::GenerateGeometryFor(BlockType type, World* w, glm::ivec3 p) {
	switch (type) {
		case BlockType::CraftingTable:
		case BlockType::Grass:
		case BlockType::Log:
			return (GenerateMultisideBlock(w, p));
			break;
		case BlockType::Dandelion:
			return (GenerateFlower(w, p));
			break;
		default:
			return (GenerateBlock(w, p));
			break;
	}
}