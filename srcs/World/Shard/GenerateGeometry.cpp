#include "Types.h"
#include "World/Shard.h"
#include "World/World.h"
#include "Render/VertexBuffers.h"
#include "Utilities/Profiler.h"

#include "World/Block.h"

void Shard::AddSide(std::vector<float>& res, glm::ivec3 local, glm::ivec3 global, glm::ivec3 offset, VertexBuffers::BufferType side, World* w) {
	Block block;
	local += offset;
	if (local.x > 15 or local.y > 15 or local.z > 15 or local.x < 0 or local.y < 0 or local.z < 0)
		block = w->GetBlock(global + offset);
	else
		block = _blocks[local.x][local.y][local.z];
	if (!block.IsTransparent())
		return;
	float *buffer = VertexBuffers::GetBuffer(side);
	res.reserve(48 * sizeof(float));
	res.insert(res.end(), buffer, buffer + 48);
};

void Shard::AddSideTransparent(std::vector<float>& res, glm::ivec3 local, glm::ivec3 global, glm::ivec3 offset, VertexBuffers::BufferType side, World* w, Block b) {
	Block block;
	local += offset;
	if (local.x > 15 or local.y > 15 or local.z > 15 or local.x < 0 or local.y < 0 or local.z < 0)
		block = w->GetBlock(global + offset);
	else
		block = _blocks[local.x][local.y][local.z];
	if (!block.IsTransparent() or block == b)
		return;
	float *buffer = VertexBuffers::GetBuffer(side);
	res.reserve(48 * sizeof(float));
	res.insert(res.end(), buffer, buffer + 48);
};

std::vector<float> Shard::GenerateTransparent(World* w, const glm::ivec3 globalP, const glm::ivec3 localP, const Block block) {
	std::vector<float> res = std::vector<float>();

	AddSideTransparent(res, localP, globalP, glm::ivec3(1, 0, 0), VertexBuffers::Right, w, block);
	AddSideTransparent(res, localP, globalP, glm::ivec3(-1, 0, 0), VertexBuffers::Left, w, block);
	AddSideTransparent(res, localP, globalP, glm::ivec3(0, 1, 0), VertexBuffers::Top, w, block);
	AddSideTransparent(res, localP, globalP, glm::ivec3(0, -1, 0), VertexBuffers::Bottom, w, block);
	AddSideTransparent(res, localP, globalP, glm::ivec3(0, 0, 1), VertexBuffers::Front, w, block);
	AddSideTransparent(res, localP, globalP, glm::ivec3(0, 0, -1), VertexBuffers::Back, w, block);

	return res;
}

std::vector<float> Shard::GenerateBlock(World* w, const glm::ivec3 globalP, const glm::ivec3 localP) {
	std::vector<float> res = std::vector<float>();

	AddSide(res, localP, globalP, glm::ivec3(1, 0, 0), VertexBuffers::Right, w);
	AddSide(res, localP, globalP, glm::ivec3(-1, 0, 0), VertexBuffers::Left, w);
	AddSide(res, localP, globalP, glm::ivec3(0, 1, 0), VertexBuffers::Top, w);
	AddSide(res, localP, globalP, glm::ivec3(0, -1, 0), VertexBuffers::Bottom, w);
	AddSide(res, localP, globalP, glm::ivec3(0, 0, 1), VertexBuffers::Front, w);
	AddSide(res, localP, globalP, glm::ivec3(0, 0, -1), VertexBuffers::Back, w);
	// //? Right face
	// if ((localP.x != 15 ? _blocks[localP.x + 1][localP.y][localP.z] : w->GetBlock(globalP + glm::ivec3(1, 0, 0))).IsTransparent()) {
	// 	float *buffer = VertexBuffers::GetBuffer(VertexBuffers::Right);
	// 	res.reserve(48 * sizeof(float));
	// 	res.insert(res.end(), buffer, buffer + 48);
	// }
	// //? Left face
	// if ((localP.x != 0 ? _blocks[localP.x - 1][localP.y][localP.z] : w->GetBlock(globalP + glm::ivec3(-1, 0, 0))).IsTransparent()) {
	// 	float *buffer = VertexBuffers::GetBuffer(VertexBuffers::Left);
	// 	res.reserve(48 * sizeof(float));
	// 	res.insert(res.end(), buffer, buffer + 48);
	// }
	// //? Top face
	// if ((localP.y != 15 ? _blocks[localP.x][localP.y + 1][localP.z] : w->GetBlock(globalP + glm::ivec3(0, 1, 0))).IsTransparent()) {
	// 	float *buffer = VertexBuffers::GetBuffer(VertexBuffers::Top);
	// 	res.reserve(48 * sizeof(float));
	// 	res.insert(res.end(), buffer, buffer + 48);
	// }
	// //? Bottom face
	// if ((localP.y != 0 ? _blocks[localP.x][localP.y - 1][localP.z] : w->GetBlock(globalP + glm::ivec3(0, -1, 0))).IsTransparent()) {
	// 	float *buffer = VertexBuffers::GetBuffer(VertexBuffers::Bottom);
	// 	res.reserve(48 * sizeof(float));
	// 	res.insert(res.end(), buffer, buffer + 48);
	// }
	// //? Front face
	// if ((localP.z != 15 ? _blocks[localP.x][localP.y][localP.z + 1] : w->GetBlock(globalP + glm::ivec3(0, 0, 1))).IsTransparent()) {
	// 	float *buffer = VertexBuffers::GetBuffer(VertexBuffers::Front);
	// 	res.reserve(48 * sizeof(float));
	// 	res.insert(res.end(), buffer, buffer + 48);
	// }
	// //? Back face
	// if ((localP.z != 0 ? _blocks[localP.x][localP.y][localP.z - 1] : w->GetBlock(globalP + glm::ivec3(0, 0, -1))).IsTransparent()) {
	// 	float *buffer = VertexBuffers::GetBuffer(VertexBuffers::Back);
	// 	res.reserve(48 * sizeof(float));
	// 	res.insert(res.end(), buffer, buffer + 48);
	// }
	return res;
}

std::vector<float> Shard::GenerateMultisideBlock(World* w, const glm::ivec3 globalBlockPos, const glm::ivec3 localBlockPos) {
	std::vector<float> res = std::vector<float>();

	AddSide(res, localBlockPos, globalBlockPos, glm::ivec3(1, 0, 0), VertexBuffers::MultiRight, w);
	AddSide(res, localBlockPos, globalBlockPos, glm::ivec3(-1, 0, 0), VertexBuffers::MultiLeft, w);
	AddSide(res, localBlockPos, globalBlockPos, glm::ivec3(0, 1, 0), VertexBuffers::MultiTop, w);
	AddSide(res, localBlockPos, globalBlockPos, glm::ivec3(0, -1, 0), VertexBuffers::MultiBottom, w);
	AddSide(res, localBlockPos, globalBlockPos, glm::ivec3(0, 0, 1), VertexBuffers::MultiFront, w);
	AddSide(res, localBlockPos, globalBlockPos, glm::ivec3(0, 0, -1), VertexBuffers::MultiBack, w);
	// //? Right face
	// if (w->GetBlock(globalBlockPos + glm::ivec3(1, 0, 0)).IsTransparent()) {
	// 	float *buffer = VertexBuffers::GetBuffer(VertexBuffers::MultiRight);
	// 	res.reserve(48 * sizeof(float));
	// 	res.insert(res.end(), buffer, buffer + 48);
	// }
	// //? Left face
	// if (w->GetBlock(globalBlockPos + glm::ivec3(-1, 0, 0)).IsTransparent()) {
	// 	float *buffer = VertexBuffers::GetBuffer(VertexBuffers::MultiLeft);
	// 	res.reserve(48 * sizeof(float));
	// 	res.insert(res.end(), buffer, buffer + 48);
	// }
	// //? Top face
	// if (w->GetBlock(globalBlockPos + glm::ivec3(0, 1, 0)).IsTransparent()) {
	// 	float *buffer = VertexBuffers::GetBuffer(VertexBuffers::MultiTop);
	// 	res.reserve(48 * sizeof(float));
	// 	res.insert(res.end(), buffer, buffer + 48);
	// }
	// //? Bottom face
	// if (w->GetBlock(globalBlockPos + glm::ivec3(0, -1, 0)).IsTransparent()) {
	// 	float *buffer = VertexBuffers::GetBuffer(VertexBuffers::MultiBottom);
	// 	res.reserve(48 * sizeof(float));
	// 	res.insert(res.end(), buffer, buffer + 48);
	// }
	// //? Front face
	// if (w->GetBlock(globalBlockPos + glm::ivec3(0, 0, 1)).IsTransparent()) {
	// 	float *buffer = VertexBuffers::GetBuffer(VertexBuffers::MultiFront);
	// 	res.reserve(48 * sizeof(float));
	// 	res.insert(res.end(), buffer, buffer + 48);
	// }
	// //? Back face
	// if (w->GetBlock(globalBlockPos + glm::ivec3(0, 0, -1)).IsTransparent()) {
	// 	float *buffer = VertexBuffers::GetBuffer(VertexBuffers::MultiBack);
	// 	res.reserve(48 * sizeof(float));
	// 	res.insert(res.end(), buffer, buffer + 48);
	// }
	return res;
}

std::vector<float> Shard::GenerateFlower(World* w, glm::ivec3 globalBlockPos, const glm::ivec3 localBlockPos) {
	std::vector<float> res = std::vector<float>();
	float *buffer = VertexBuffers::GetBuffer(VertexBuffers::Flower);
	res.reserve(96 * sizeof(float));
	res.insert(res.end(), buffer, buffer + 48);
	return res;
}
