#include "Types.h"
#include "World/Shard.h"
#include "Render/Objects.h"

std::vector<float> Shard::GenerateBlock(World* w, glm::ivec3 globalBlockPos) {
	std::vector<float> res = std::vector<float>();

	//? Right face
	if (w->GetBlock(globalBlockPos + glm::ivec3(1, 0, 0)) == BlockType::Air) {
		float *buffer = VertexBuffers::GetBuffer(VertexBuffers::Right);
		res.reserve(48 * sizeof(float));
		res.insert(res.end(), buffer, buffer + 48);
	}
	//? Left face
	if (w->GetBlock(globalBlockPos + glm::ivec3(-1, 0, 0)) == BlockType::Air) {
		float *buffer = VertexBuffers::GetBuffer(VertexBuffers::Left);
		res.reserve(48 * sizeof(float));
		res.insert(res.end(), buffer, buffer + 48);
	}
	//? Top face
	if (w->GetBlock(globalBlockPos + glm::ivec3(0, 1, 0)) == BlockType::Air) {
		float *buffer = VertexBuffers::GetBuffer(VertexBuffers::Top);
		res.reserve(48 * sizeof(float));
		res.insert(res.end(), buffer, buffer + 48);
	}
	//? Bottom face
	if (w->GetBlock(globalBlockPos + glm::ivec3(0, -1, 0)) == BlockType::Air) {
		float *buffer = VertexBuffers::GetBuffer(VertexBuffers::Bottom);
		res.reserve(48 * sizeof(float));
		res.insert(res.end(), buffer, buffer + 48);
	}
	//? Front face
	if (w->GetBlock(globalBlockPos + glm::ivec3(0, 0, 1)) == BlockType::Air) {
		float *buffer = VertexBuffers::GetBuffer(VertexBuffers::Front);
		res.reserve(48 * sizeof(float));
		res.insert(res.end(), buffer, buffer + 48);
	}
	//? Back face
	if (w->GetBlock(globalBlockPos + glm::ivec3(0, 0, -1)) == BlockType::Air) {
		float *buffer = VertexBuffers::GetBuffer(VertexBuffers::Back);
		res.reserve(48 * sizeof(float));
		res.insert(res.end(), buffer, buffer + 48);
	}
	return res;
}

std::vector<float> Shard::GenerateMultisideBlock(World* w, glm::ivec3 globalBlockPos) {
	std::vector<float> res = std::vector<float>();

	//? Right face
	if (w->GetBlock(globalBlockPos + glm::ivec3(1, 0, 0)) == BlockType::Air) {
		float *buffer = VertexBuffers::GetBuffer(VertexBuffers::MultiRight);
		res.reserve(48 * sizeof(float));
		res.insert(res.end(), buffer, buffer + 48);
	}
	//? Left face
	if (w->GetBlock(globalBlockPos + glm::ivec3(-1, 0, 0)) == BlockType::Air) {
		float *buffer = VertexBuffers::GetBuffer(VertexBuffers::MultiLeft);
		res.reserve(48 * sizeof(float));
		res.insert(res.end(), buffer, buffer + 48);
	}
	//? Top face
	if (w->GetBlock(globalBlockPos + glm::ivec3(0, 1, 0)) == BlockType::Air) {
		float *buffer = VertexBuffers::GetBuffer(VertexBuffers::MultiTop);
		res.reserve(48 * sizeof(float));
		res.insert(res.end(), buffer, buffer + 48);
	}
	//? Bottom face
	if (w->GetBlock(globalBlockPos + glm::ivec3(0, -1, 0)) == BlockType::Air) {
		float *buffer = VertexBuffers::GetBuffer(VertexBuffers::MultiBottom);
		res.reserve(48 * sizeof(float));
		res.insert(res.end(), buffer, buffer + 48);
	}
	//? Front face
	if (w->GetBlock(globalBlockPos + glm::ivec3(0, 0, 1)) == BlockType::Air) {
		float *buffer = VertexBuffers::GetBuffer(VertexBuffers::MultiFront);
		res.reserve(48 * sizeof(float));
		res.insert(res.end(), buffer, buffer + 48);
	}
	//? Back face
	if (w->GetBlock(globalBlockPos + glm::ivec3(0, 0, -1)) == BlockType::Air) {
		float *buffer = VertexBuffers::GetBuffer(VertexBuffers::MultiBack);
		res.reserve(48 * sizeof(float));
		res.insert(res.end(), buffer, buffer + 48);
	}
	return res;
}

std::vector<float> Shard::GenerateFlower(World* w, glm::ivec3 globalBlockPos) {
	std::vector<float> res = std::vector<float>();
	float *buffer = VertexBuffers::GetBuffer(VertexBuffers::Flower);
	res.reserve(96 * sizeof(float));
	res.insert(res.end(), buffer, buffer + 48);
	return res;
}
