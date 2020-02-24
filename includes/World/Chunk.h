#pragma once

#include <glm/glm.hpp>
#include <atomic>

#include "Types.h"

class Chunk {
public:
	enum State {
		Empty,
		Completed,
		AwaitingGeneration,
		AwaitingGeometry
	};
	Chunk(Game*, glm::ivec2);
	~Chunk();
	void Generate();
	// void UpdateGeometry();
	void SetActive(bool);
	Block GetBlock(glm::ivec3);
	void SetBlock(glm::ivec3, Block);
	void PlayerSetBlock(glm::ivec3, Block);
	//* New API
	State GetState();
	std::atomic<State> state;
	void GenerateBlocks();
	void GenerateVertices();
	void GenerateModels();
private:
	bool _state; //! This will be no longer needed
	Game* _game;
	glm::ivec2 _position;
	Shard* _shards[16];
};
