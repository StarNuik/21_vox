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
#include "World/Block.h"

void Shard::UpdateGeometry() {
	GLRenderer* r = _game->GetRenderer();

	//* Delete old geometry and models
	for (RenderModel* model : _models) {
		delete model;
	}
	_models.clear();
	// ! Otherwise _models.push_back(model) segfaults
	// _models = std::vector<RenderModel*>();

	//* For every block type
	for (Block b = Block::First; b <= Block::Last; b++) {
		if (!HasBlock(b))
			continue;
		RenderModel* model = GenerateModelOfBlock(b);
		if (!model)
			continue;
		_models.reserve(sizeof(RenderModel*));
		_models.push_back(model);
	}
}

RenderModel* Shard::GenerateModelOfBlock(const Block b) {
	GLRenderer* r = _game->GetRenderer();
	ResourceLoader* rs = _game->GetResources();
	World* w = _game->GetWorld();
	uint count = CountBlock(b);
	std::vector<float> vertexBuffer;

	for (int x = 0; x < 16 && count > 0; x++)
		for (int y = 0; y < 16 && count > 0; y++)
			for (int z = 0; z < 16 && count > 0; z++) {
				if (_blocks[x][y][z] != b)
					continue;
				std::vector<float> block = GenerateGeometryFor(b, w, _position * 16 + glm::ivec3(x, y, z), glm::ivec3(x, y, z));
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
	RenderModel* model = new RenderModel(r, rs->GetShader(USED_SHADER), rs->GetMaterial(b), g);
	model->SetPosition(_position * 16);
	return model;
};

std::vector<float> Shard::GenerateGeometryFor(const Block block, World* w, const glm::ivec3 p, const glm::ivec3 l) {
	if (block == Block::Cactus) {
		return GenerateCactus();
	}
	if (block.IsMultisided()) {
		return GenerateMultisideBlock(w, p, l);
	}
	if (block.IsFlower()) {
		return GenerateFlower(w, p, l);
	}
	if (block.IsTransparent()) {
		return GenerateTransparent(w, p, l, block);
	}
	return GenerateBlock(w, p, l);
}