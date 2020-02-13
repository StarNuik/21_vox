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
	RenderModel* model;
	model = GenerateBlocks();
	if (model) {
		_models.reserve(sizeof(RenderModel*));
		_models.push_back(model);
	}
	model = GenerateWater();
	if (model) {
		_models.reserve(sizeof(RenderModel*));
		_models.push_back(model);
	}
	// for (Block b = Block::First; b <= Block::Last; b++) {
	// 	if (!HasBlock(b))
	// 		continue;
	// 	RenderModel* model = GenerateModelOfBlock(b);
	// 	if (!model)
	// 		continue;
	// 	_models.reserve(sizeof(RenderModel*));
	// 	_models.push_back(model);
	// }
}

RenderModel* Shard::GenerateBlocks() {
	GLRenderer* r = _game->GetRenderer();
	ResourceLoader* rs = _game->GetResources();
	World* w = _game->GetWorld();
	std::vector<float> vertexBuffer = std::vector<float>();

	for (int x = 0; x < 16; x++)
		for (int y = 0; y < 16; y++)
			for (int z = 0; z < 16; z++) {
				Block b = _blocks[x][y][z];
				if (b == Block::Air or b == Block::Water or b.IsBlending())
					continue;
				std::vector<float> blockBuffer = GenerateBlock(w, _position * 16 + glm::ivec3(x, y, z), glm::ivec3(x, y, z), b);
				if (blockBuffer.size() == 0)
					continue;
				vertexBuffer.reserve(blockBuffer.size());
				vertexBuffer.insert(vertexBuffer.end(), blockBuffer.begin(), blockBuffer.end());
			}
	vertexBuffer.shrink_to_fit();
	Geometry* g = new Geometry(vertexBuffer);
	RenderModel* model = new RenderModel(r, rs->GetShader(BLOCKS_SHADER), rs->GetMaterial(BLOCKS_MATERIAL), g);
	model->SetPosition(_position * 16);
	return model;
}

RenderModel* Shard::GenerateWater() {
	GLRenderer* r = _game->GetRenderer();
	ResourceLoader* rs = _game->GetResources();
	World* w = _game->GetWorld();
	std::vector<float> vertexBuffer = std::vector<float>();

	uint count = CountBlock(Block::Water);
	for (int x = 0; x < 16 and count > 0; x++)
		for (int y = 0; y < 16 and count > 0; y++)
			for (int z = 0; z < 16 and count > 0; z++) {
				Block b = _blocks[x][y][z];
				if (b == Block::Air or b != Block::Water)
					continue;
				std::vector<float> blockBuffer = GenerateWaterBlock(w, _position * 16 + glm::ivec3(x, y, z), glm::ivec3(x, y, z));
				if (blockBuffer.size() == 0)
					continue;
				vertexBuffer.reserve(blockBuffer.size());
				vertexBuffer.insert(vertexBuffer.end(), blockBuffer.begin(), blockBuffer.end());
				count--;
			}
	vertexBuffer.shrink_to_fit();
	Geometry* g = new Geometry(vertexBuffer);
	RenderModel* model = new RenderModel(r, rs->GetShader(WATER_SHADER), rs->GetMaterial(WATER_MATERIAL), g);
	model->SetPosition(_position * 16);
	return model;
}

// RenderModel* Shard::GenerateModelOfBlock(const Block b) {
// 	GLRenderer* r = _game->GetRenderer();
// 	ResourceLoader* rs = _game->GetResources();
// 	World* w = _game->GetWorld();
// 	uint count = CountBlock(b);
// 	std::vector<float> vertexBuffer;

// 	for (int x = 0; x < 16 && count > 0; x++)
// 		for (int y = 0; y < 16 && count > 0; y++)
// 			for (int z = 0; z < 16 && count > 0; z++) {
// 				if (_blocks[x][y][z] != b)
// 					continue;
// 				std::vector<float> block = GenerateGeometryFor(b, w, _position * 16 + glm::ivec3(x, y, z), glm::ivec3(x, y, z));
// 				if (block.size() == 0)
// 					continue;
// 				for (int i = 0; i < block.size(); i += 8) {
// 					block[i + 0] += (float)x;
// 					block[i + 1] += (float)y;
// 					block[i + 2] += (float)z;
// 				}
// 				vertexBuffer.reserve(block.size());
// 				vertexBuffer.insert(vertexBuffer.end(), block.begin(), block.end());
// 				count--;
// 			}
// 	if (vertexBuffer.size() == 0)
// 		return nullptr;
// 	Geometry* g = new Geometry(vertexBuffer);
// 	RenderModel* model = new RenderModel(r, rs->GetShader(USED_SHADER), rs->GetMaterial(b), g);
// 	model->SetPosition(_position * 16);
// 	return model;
// };

// std::vector<float> Shard::GenerateGeometryFor(const Block block, World* w, const glm::ivec3 p, const glm::ivec3 l) {
// 	if (block == Block::Cactus) {
// 		return GenerateCactus();
// 	}
// 	if (block == Block::Water) {
// 		return GenerateWater(w, p, l);
// 	}
// 	if (block.IsMultisided()) {
// 		return GenerateMultisideBlock(w, p, l);
// 	}
// 	if (block.IsFlower()) {
// 		return GenerateFlower();
// 	}
// 	if (block.IsTransparent()) {
// 		return GenerateTransparent(w, p, l, block);
// 	}
// 	return GenerateBlock(w, p, l);
// }