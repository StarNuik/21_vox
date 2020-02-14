#include "Types.h"
#include "World/Shard.h"
#include "World/World.h"
#include "Render/VertexBuffers.h"
#include "Utilities/Profiler.h"

#include "World/Block.h"

namespace {
	inline bool ShouldPlace(const Block b) {
		return b.IsTransparent() or b.IsFlower() or b.IsBlending();
	}
	inline void ResolveUVs(std::vector<float>& vec, const Block b, const Block::Side side) {
		for (int i = 0; i < vec.size(); i += VERTEX_SIZE) {
			vec[i + 6] = b.GetU(side) + BLOCK_SIZE_U * vec[i + 6];
			vec[i + 7] = b.GetV(side) + BLOCK_SIZE_V * vec[i + 7];
		}
	}
	inline void ResolveLocalPos(std::vector<float>& vec, const glm::ivec3 local) {
		for (int i = 0; i < vec.size(); i += VERTEX_SIZE) {
			vec[i + 0] += (float)local.x;
			vec[i + 1] += (float)local.y;
			vec[i + 2] += (float)local.z;
		}
	}
}

void Shard::AddSide(std::vector<float>& res, World* w, const Block b, const glm::ivec3 global, const glm::ivec3 local, const glm::ivec3 offset, const VertexBuffers::BufferType vbSide, const Block::Side uvSide) {
	Block target;
	const glm::ivec3 l = local + offset;
	if (l.x > 15 or l.y > 15 or l.z > 15 or l.x < 0 or l.y < 0 or l.z < 0) {
		target = w->GetBlock(global + offset);
	} else {
		target = _blocks[l.x][l.y][l.z];
	}
	if (ShouldPlace(target)) {
		if (b.IsTransparent()) {
			if (target != b) {
				std::vector<float> buffer = VertexBuffers::GetVectorBuffer(vbSide);
				ResolveUVs(buffer, b, uvSide);
				res.reserve(buffer.size());
				res.insert(res.end(), buffer.begin(), buffer.end());
			}
		} else {
			std::vector<float> buffer = VertexBuffers::GetVectorBuffer(vbSide);
			ResolveUVs(buffer, b, uvSide);
			res.reserve(buffer.size());
			res.insert(res.end(), buffer.begin(), buffer.end());
			res.shrink_to_fit();
		}
	}
}

std::vector<float> Shard::GenerateBlock(World* w, const glm::ivec3 global, const glm::ivec3 local, const Block b) {
	std::vector<float> res = std::vector<float>();

	if (b == Block::Cactus) {
		res = VertexBuffers::GetVectorBuffer(VertexBuffers::Cactus);
		ResolveUVs(res, Block::Cactus, Block::Top);
	} else if (b.IsFlower()) {
		res = VertexBuffers::GetVectorBuffer(VertexBuffers::Flower);
		ResolveUVs(res, b, Block::Top);
	} else {
		AddSide(res, w, b, global, local, glm::ivec3(1, 0, 0), VertexBuffers::Right, Block::Right);
		AddSide(res, w, b, global, local, glm::ivec3(-1, 0, 0), VertexBuffers::Left, Block::Left);
		AddSide(res, w, b, global, local, glm::ivec3(0, 1, 0), VertexBuffers::Top, Block::Top);
		AddSide(res, w, b, global, local, glm::ivec3(0, -1, 0), VertexBuffers::Bottom, Block::Bottom);
		AddSide(res, w, b, global, local, glm::ivec3(0, 0, 1), VertexBuffers::Front, Block::Front);
		AddSide(res, w, b, global, local, glm::ivec3(0, 0, -1), VertexBuffers::Back, Block::Back);
	}
	res.shrink_to_fit();
	ResolveLocalPos(res, local);

	return res;
}

std::vector<float> Shard::GenerateWaterBlock(World* w, const glm::ivec3 global, const glm::ivec3 local) {
	std::vector<float> res = std::vector<float>();

	Block target;
	const glm::ivec3 l = local + glm::ivec3(0, 1, 0);
	if (l.y > 15) {
		target = w->GetBlock(global + l);
	} else {
		target = _blocks[l.x][l.y][l.z];
	}
	if (target == Block::Water) {
		AddSide(res, w, Block::Water, global, local, glm::ivec3(1, 0, 0), VertexBuffers::Right, Block::Right);
		AddSide(res, w, Block::Water, global, local, glm::ivec3(-1, 0, 0), VertexBuffers::Left, Block::Left);
		AddSide(res, w, Block::Water, global, local, glm::ivec3(0, 1, 0), VertexBuffers::Top, Block::Top);
		AddSide(res, w, Block::Water, global, local, glm::ivec3(0, -1, 0), VertexBuffers::Bottom, Block::Bottom);
		AddSide(res, w, Block::Water, global, local, glm::ivec3(0, 0, 1), VertexBuffers::Front, Block::Front);
		AddSide(res, w, Block::Water, global, local, glm::ivec3(0, 0, -1), VertexBuffers::Back, Block::Back);
	} else {
		AddSide(res, w, Block::Water, global, local, glm::ivec3(1, 0, 0), VertexBuffers::WaterRight, Block::Right);
		AddSide(res, w, Block::Water, global, local, glm::ivec3(-1, 0, 0), VertexBuffers::WaterLeft, Block::Left);
		AddSide(res, w, Block::Water, global, local, glm::ivec3(0, 1, 0), VertexBuffers::WaterTop, Block::Top);
		AddSide(res, w, Block::Water, global, local, glm::ivec3(0, -1, 0), VertexBuffers::Bottom, Block::Bottom);
		AddSide(res, w, Block::Water, global, local, glm::ivec3(0, 0, 1), VertexBuffers::WaterFront, Block::Front);
		AddSide(res, w, Block::Water, global, local, glm::ivec3(0, 0, -1), VertexBuffers::WaterBack, Block::Back);
	}
	res.shrink_to_fit();
	ResolveLocalPos(res, local);

	return res;
}

// void Shard::AddSide(std::vector<float>& res, glm::ivec3 local, glm::ivec3 global, glm::ivec3 offset, VertexBuffers::BufferType side, World* w) {
// 	Block block;
// 	local += offset;
// 	if (local.x > 15 or local.y > 15 or local.z > 15 or local.x < 0 or local.y < 0 or local.z < 0)
// 		block = w->GetBlock(global + offset);
// 	else
// 		block = _blocks[local.x][local.y][local.z];
// 	if (block.IsTransparent() or block.IsFlower()) {
// 		float *buffer = VertexBuffers::GetBuffer(side);
// 		res.reserve(48 * sizeof(float));
// 		res.insert(res.end(), buffer, buffer + 48);
// 	}
// };

// void Shard::AddSideTransparent(std::vector<float>& res, glm::ivec3 local, glm::ivec3 global, glm::ivec3 offset, VertexBuffers::BufferType side, World* w, Block b) {
// 	Block block;
// 	local += offset;
// 	if (local.x > 15 or local.y > 15 or local.z > 15 or local.x < 0 or local.y < 0 or local.z < 0)
// 		block = w->GetBlock(global + offset);
// 	else
// 		block = _blocks[local.x][local.y][local.z];
// 	if (block.IsTransparent() and block != b) {
// 		std::vector<float> buffer;
// 		buffer = VertexBuffers::GetVectorBuffer(side);
// 		res.insert(res.end(), buffer.begin(), buffer.end());
// 	}
// };

// void Shard::AddSideWater(std::vector<float>& res, glm::ivec3 local, glm::ivec3 global, glm::ivec3 offset, VertexBuffers::BufferType side, VertexBuffers::BufferType sideLow, World* w) {
// 	Block block;
// 	Block upBlock;

// 	local += offset;
// 	if (local.x > 15 or local.y > 15 or local.z > 15 or local.x < 0 or local.y < 0 or local.z < 0)
// 		block = w->GetBlock(global + offset);
// 	else
// 		block = _blocks[local.x][local.y][local.z];
// 	local += -offset + glm::ivec3(0, 1, 0);
// 	if (local.y > 15)
// 		upBlock = w->GetBlock(global + offset);
// 	else
// 		upBlock = _blocks[local.x][local.y][local.z];
// 	if ((block.IsTransparent() or block.IsFlower()) and block != Block::Water) {
// 		std::vector<float> buffer;
// 		if (upBlock == Block::Water) {
// 			buffer = VertexBuffers::GetVectorBuffer(side);
// 			res.insert(res.end(), buffer.begin(), buffer.end());
// 		} else {
// 			buffer = VertexBuffers::GetVectorBuffer(sideLow);
// 			res.insert(res.end(), buffer.begin(), buffer.end());
// 		}
// 	}
// };

// std::vector<float> Shard::GenerateWater(World* w, const glm::ivec3 globalP, const glm::ivec3 localP) {
// 	std::vector<float> res = std::vector<float>();

// 	AddSideWater(res, localP, globalP, glm::ivec3(1, 0, 0), VertexBuffers::Right, VertexBuffers::WaterRight, w);
// 	AddSideWater(res, localP, globalP, glm::ivec3(-1, 0, 0), VertexBuffers::Left, VertexBuffers::WaterLeft, w);
// 	AddSideWater(res, localP, globalP, glm::ivec3(0, 1, 0), VertexBuffers::Top, VertexBuffers::WaterTop, w);
// 	AddSideWater(res, localP, globalP, glm::ivec3(0, 0, 1), VertexBuffers::Front, VertexBuffers::WaterFront, w);
// 	AddSideWater(res, localP, globalP, glm::ivec3(0, 0, -1), VertexBuffers::Back, VertexBuffers::WaterBack, w);
// 	AddSideTransparent(res, localP, globalP, glm::ivec3(0, -1, 0), VertexBuffers::Bottom, w, Block::Water);

// 	res.shrink_to_fit();
// 	return res;
// }

// std::vector<float> Shard::GenerateTransparent(World* w, const glm::ivec3 globalP, const glm::ivec3 localP, const Block block) {
// 	std::vector<float> res = std::vector<float>();

// 	AddSideTransparent(res, localP, globalP, glm::ivec3(1, 0, 0), VertexBuffers::Right, w, block);
// 	AddSideTransparent(res, localP, globalP, glm::ivec3(-1, 0, 0), VertexBuffers::Left, w, block);
// 	AddSideTransparent(res, localP, globalP, glm::ivec3(0, 1, 0), VertexBuffers::Top, w, block);
// 	AddSideTransparent(res, localP, globalP, glm::ivec3(0, -1, 0), VertexBuffers::Bottom, w, block);
// 	AddSideTransparent(res, localP, globalP, glm::ivec3(0, 0, 1), VertexBuffers::Front, w, block);
// 	AddSideTransparent(res, localP, globalP, glm::ivec3(0, 0, -1), VertexBuffers::Back, w, block);

// 	res.shrink_to_fit();
// 	return res;
// }

// std::vector<float> Shard::GenerateBlock(World* w, const glm::ivec3 globalP, const glm::ivec3 localP) {
// 	std::vector<float> res = std::vector<float>();

// 	AddSide(res, localP, globalP, glm::ivec3(1, 0, 0), VertexBuffers::Right, w);
// 	AddSide(res, localP, globalP, glm::ivec3(-1, 0, 0), VertexBuffers::Left, w);
// 	AddSide(res, localP, globalP, glm::ivec3(0, 1, 0), VertexBuffers::Top, w);
// 	AddSide(res, localP, globalP, glm::ivec3(0, -1, 0), VertexBuffers::Bottom, w);
// 	AddSide(res, localP, globalP, glm::ivec3(0, 0, 1), VertexBuffers::Front, w);
// 	AddSide(res, localP, globalP, glm::ivec3(0, 0, -1), VertexBuffers::Back, w);
// 	return res;
// }

// std::vector<float> Shard::GenerateMultisideBlock(World* w, const glm::ivec3 globalBlockPos, const glm::ivec3 localBlockPos) {
// 	std::vector<float> res = std::vector<float>();

// 	AddSide(res, localBlockPos, globalBlockPos, glm::ivec3(1, 0, 0), VertexBuffers::MultiRight, w);
// 	AddSide(res, localBlockPos, globalBlockPos, glm::ivec3(-1, 0, 0), VertexBuffers::MultiLeft, w);
// 	AddSide(res, localBlockPos, globalBlockPos, glm::ivec3(0, 1, 0), VertexBuffers::MultiTop, w);
// 	AddSide(res, localBlockPos, globalBlockPos, glm::ivec3(0, -1, 0), VertexBuffers::MultiBottom, w);
// 	AddSide(res, localBlockPos, globalBlockPos, glm::ivec3(0, 0, 1), VertexBuffers::MultiFront, w);
// 	AddSide(res, localBlockPos, globalBlockPos, glm::ivec3(0, 0, -1), VertexBuffers::MultiBack, w);
// 	return res;
// }

// std::vector<float> Shard::GenerateFlower() {
// 	// std::vector<float> res = std::vector<float>();
// 	// float *buffer = VertexBuffers::GetBuffer(VertexBuffers::Flower);
// 	// res.reserve(96 * sizeof(float));
// 	// res.insert(res.end(), buffer, buffer + 96);
// 	return VertexBuffers::GetVectorBuffer(VertexBuffers::Flower);
// }

// std::vector<float> Shard::GenerateCactus() {
// 	// std::vector<float> res = std::vector<float>();
// 	// float *buffer = VertexBuffers::GetBuffer(VertexBuffers::Cactus);
// 	// res.reserve(288 * sizeof(float));
// 	// res.insert(res.end(), buffer, buffer + 288);
// 	return VertexBuffers::GetVectorBuffer(VertexBuffers::Cactus);
// }
