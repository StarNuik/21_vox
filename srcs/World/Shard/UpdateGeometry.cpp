#include "Types.h"
#include "World/Shard.h"
#include "Render/RenderModel.h"
#include "Render/GLRenderer.h"
#include "Render/Geometry.h"
#include "World/World.h"
#include "Engine/Game.h"
#include "World/ResourceLoader.h"
#include "Utilities/Log.h"
#include "Utilities/Profiler.h"

void Shard::UpdateGeometry() {
	GLRenderer* r = _game->GetRenderer();
	//* Delete old geometry and models
	for (RenderModel* model : _models) {
		delete model;
	}
	_models.clear();
	//! Otherwise _models.push_back(model) segfaults
	_models = std::vector<RenderModel*>();
	//* For every block type
	for (uint t = (uint)BlockType::First + 1; t <= (uint)BlockType::Last; t++) {
		if (!HasType((BlockType)t))
			continue;
		// Profiler::Start("Model|Gen");
		RenderModel* model = GenerateModelOfType((BlockType)t);
		// Profiler::Add("Model|Gen");
		if (!model)
			continue;
		_models.reserve(sizeof(RenderModel*));
		_models.push_back(model);
	}
	//* If shard is active, add models to the renderer
	if (_state) {
		for (RenderModel* model : _models) {
			r->AddModel(model);
		}
	}
}

RenderModel* Shard::GenerateModelOfType(const BlockType type) {
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
				// Profiler::Start("Model|Block");
				std::vector<float> block = GenerateGeometryFor(type, w, _position * 16 + glm::ivec3(x, y, z), glm::ivec3(x, y, z));
				// Profiler::Add("Model|Block");
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
	if (vertexBuffer.size() == 0)
		return nullptr;
	Geometry* g = new Geometry(vertexBuffer);
	RenderModel* model = new RenderModel(r, rs->GetShader(USED_SHADER), rs->GetMaterial(type), g);
	model->SetPosition(_position * 16);
	return (model);
};

std::vector<float> Shard::GenerateGeometryFor(const BlockType type, World* w, const glm::ivec3 p, const glm::ivec3 l) {
	switch (type) {
		case BlockType::CraftingTable:
		case BlockType::Grass:
		case BlockType::Log:
			return (GenerateMultisideBlock(w, p, l));
			break;
		case BlockType::Dandelion:
			return (GenerateFlower(w, p, l));
			break;
		default:
			return (GenerateBlock(w, p, l));
			break;
	}
}