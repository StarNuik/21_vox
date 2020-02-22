#pragma once

#include <glm/glm.hpp>

#include "Types.h"
#include "Generation/MapGeneration.h"

class Chunk {
public:
	Chunk(Game*, glm::ivec2);
	~Chunk();
	void Generate();
	// void UpdateGeometry();
	void SetActive(bool);
	Block GetBlock(glm::ivec3);
	void SetBlock(glm::ivec3, Block);
	void PlayerSetBlock(glm::ivec3, Block);
private:
	bool _state;
	Game* _game;
	glm::ivec2 _position;
	Shard* _shards[16];
	MapGeneration* _mp;

	// 1
	void FillFirstLayer(int& firstLayerBorder, const int x, const int z);
	// 2
	void FillSecondLayer(MapGeneration::StoredMapData& block, int& lastLayerBorder, const int firstLayerBorder, const int x, const int z);
	// 3
	void FillSwampWater(const MapGeneration::GenerationType biome, const int lastLayerBorder, const int x, const int z);
	// 4
	void FillHighLandAboveTheRiver(const MapGeneration::StoredMapData block, const int firstLayerBorder, const int lastLayerBorder, const int x, const int z);
	// 5
	void FillCaves(const MapGeneration::GenerationType biome, int& cavesHeight, const int lastLayerBorder, const int x, const int z);
	// 6
	void FillCrevices(const MapGeneration::GenerationType biome, int& crevicesHeight, const int firstLayerBorder ,const int lastLayerBorder, const int x, const int z);
	// 7
	void FillVegetation(const MapGeneration::GenerationType biome, const int crevicesHeight, const int cavesHeight, const int lastLayerBorder, const int x, const int z);
	// 8
	void FillTrees(const MapGeneration::StoredMapData block, const int crevicesHeight, const int cavesHeight, const int lastLayerBorder, const int x, const int z);
	// 9
	void FillOre(const MapGeneration::GenerationType biome, const int lastLayerBorder, const int x, const int z);
};
