#include "Generation/MapGeneration.h"
#include "World/Block.h"
#include <iostream>
#include "Utilities/Rand.h"
#define LERP MapGeneration::Lerp

float MapGeneration::Lerp(float v0, float v1, float t)
{
	t = glm::clamp(t, 0.f, 1.f);
	return (1.f - t) * v0 + t * v1;
}

/* - look pretty good
float MapGeneration::SwampGenerationColumn(glm::ivec2 pos)
{
  FastNoise& noise = _noises[Swamp];
  float terraceValue = _terraceValue;

  float e = 1.f * (noise.GetNoise(1.f * pos.x, 1.f * pos.y));
  float e1 = 0.50f * (noise.GetNoise(2.f * pos.x, 2.f * pos.y));

  e += e1;
  e = (e * 0.5f + 0.5f) * 10.f;

  float terrace = round(e * terraceValue) / terraceValue;
  int elevation = (int)floorf(terrace);
  return elevation;
}
*/

//! Should be bool
float MapGeneration::CrevicesGeneration(glm::ivec2 globalPos, glm::ivec3 blockPosition)
{
	FastNoise& noise = _noises[Crevices];

	float globalX = globalPos.x * 16, globalZ = globalPos.y * 16; //! Vector math
	glm::ivec3 pos = glm::ivec3(blockPosition.x + globalX, blockPosition.y, blockPosition.z + globalZ);

	//! May be thread dangerous
	float terraceValue = _terraceValue;
	float e = noise.GetNoise(pos.x * 0.55f, pos.y, pos.z * 3.5f);
	e = (e * 0.5f + 0.5f);
	if (e < 0.815f)
		return -1.f;

	float terrace = round(e * terraceValue) / terraceValue;
	return terrace;
}

//! Should be bool
float MapGeneration::CavesGenerations(glm::ivec2 globalPos, glm::ivec3 blockPosition)
{
	FastNoise& noise = _noises[Caves];

	float globalX = globalPos.x * 16, globalZ = globalPos.y * 16; //!
	glm::ivec3 pos = glm::ivec3(blockPosition.x + globalX, blockPosition.y, blockPosition.z + globalZ);

	float e = noise.GetNoise(pos.x, pos.y, pos.z);
	e = (e * 0.5f) + 0.5f;

	if (e < 0.69751f)
		return -1.f;
	return e;
}

//! Move code out of here
//! Call other overload with generated biome
MapGeneration::StoredMapData MapGeneration::Generation(glm::ivec2 globalPos, glm::ivec2 blockPosition)
{
	StoredMapData column;
	float globalX = globalPos.x * 16, globalY = globalPos.y * 16;
	glm::vec2 pos = glm::ivec2(globalX + blockPosition.x, globalY + blockPosition.y);

	column.biom = BiomeGeneration(pos);
	column.treeType = Trees::Nothing;
	switch (column.biom)
	{
	case GenerationType::Ocean:
		GetOceanData(column, pos);
		break;
	case GenerationType::River:
		GetRiverData(column, pos);
		break;
	case GenerationType::GrassLand:
		GetGrassLandData(column, pos);
		break;
	case GenerationType::Swamp:
		GetSwampData(column, pos);
		break;
	case GenerationType::Desert:
		GetDesertData(column, pos);
		break;
	case GenerationType::HighLand:
		GetHighLandData(column, pos);
		break;
	case GenerationType::Snow:
		GetSnowLandData(column, pos);
		break;
	default:
		column.firstBlockLayer = Block::Dirt;
		column.lastBlockLayer = Block::Grass;
		column.approximateElevation = LandGenerationColumn(pos);
		column.approximateElevation = (int)floorf(column.approximateElevation * 10.f);
		break;
	}
	column.exactElevation = glm::clamp((int)column.approximateElevation, -10, 255);
	// column.exactElevation = glm::clamp((int)column.approximateElevation, 0, 255);
	return column;
}

MapGeneration::StoredMapData MapGeneration::Generation(glm::ivec2 chunkPos, glm::ivec2 columnPos, MapGeneration::GenerationType genType)
{
	StoredMapData column;

	glm::ivec2 globalChunkPos = chunkPos * 16;
	glm::vec2 pos = globalChunkPos + columnPos;
	column.firstBlockLayer = Block::Air;
	column.lastBlockLayer = Block::Air;

	switch (genType)
	{
		case GenerationType::Basic:
			GetBasicData(column, pos);
			break;
		case GenerationType::HighLand:
			GetHighLandData(column, pos);
			break;
		case GenerationType::Tree:
			column.approximateElevation = TreeGeneration(pos);
			column.firstBlockLayer = Block::Leaves;
			column.lastBlockLayer = Block::Log;
			break;
		case GenerationType::River:
			GetRiverData(column, pos);
			break;
		default:
			GetBasicData(column, pos);
			break;
	}
	column.biom = Nothing;
	column.treeType = tree.Nothing;
	column.exactElevation = glm::clamp((int)column.approximateElevation, -10, 255);
	// column.exactElevation = glm::clamp((int)column.approximateElevation, 0, 255);
	return column;
}


MapGeneration::MapGeneration()
{
	_exp = 2.2f;
	_terraceValue = 32.f;

	_noises[Biomes].SetNoiseType(FastNoise::Cellular);
	_noises[Biomes].SetSeed(1337);
	_noises[Biomes].SetFrequency(0.05);
	_noises[Biomes].SetCellularReturnType(FastNoise::CellValue);
	_noises[Biomes].SetCellularDistanceFunction(FastNoise::Natural);

	_noises[BeachBordered].SetNoiseType(FastNoise::SimplexFractal);
	_noises[BeachBordered].SetSeed(1337);
	_noises[BeachBordered].SetFrequency(0.005);

	_noises[PerlinX].SetNoiseType(FastNoise::Perlin);
	_noises[PerlinX].SetSeed(1532);
	_noises[PerlinX].SetFrequency(0.05);

	_noises[PerlinY].SetNoiseType(FastNoise::Perlin);
	_noises[PerlinY].SetSeed(863);
	_noises[PerlinY].SetFrequency(0.05);

	_noises[River].SetNoiseType(FastNoise::Cellular);
	_noises[River].SetSeed(1337);
	_noises[River].SetFrequency(0.003);
	_noises[River].SetCellularReturnType(FastNoise::Distance2Sub);
	_noises[River].SetCellularDistanceFunction(FastNoise::Natural);

	_noises[Tree].SetNoiseType(FastNoise::WhiteNoise);
	_noises[Tree].SetSeed(200);
	_noises[Tree].SetFrequency(0.01);

	_noises[Vegetation].SetNoiseType(FastNoise::WhiteNoise);
	_noises[Vegetation].SetSeed(1300);
	_noises[Vegetation].SetFrequency(100.0);

	_noises[Caves].SetNoiseType(FastNoise::Perlin);
	_noises[Caves].SetFrequency(0.1);
	_noises[Caves].SetSeed(167998);

	_noises[Crevices].SetNoiseType(FastNoise::Perlin);
	_noises[Crevices].SetSeed(1333);
	_noises[Crevices].SetFrequency(0.016);

	_noises[Basic].SetNoiseType(FastNoise::Perlin);
	_noises[Basic].SetFrequency(0.1);

	_noises[GrassLand].SetNoiseType(FastNoise::Simplex);
	_noises[GrassLand].SetFrequency(0.020214);

	_noises[Swamp].SetNoiseType(FastNoise::Simplex);
	_noises[Swamp].SetFrequency(0.021);

	_noises[Desert].SetNoiseType(FastNoise::Simplex);
	_noises[Desert].SetFrequency(0.01);

	_noises[Snow].SetNoiseType(FastNoise::Perlin);
	_noises[Snow].SetFrequency(0.01);

	_noises[HighLand].SetNoiseType(FastNoise::Perlin);
	_noises[HighLand].SetFrequency(0.01);

	_noises[Ore].SetNoiseType(FastNoise::Cellular);
	_noises[Ore].SetFrequency(0.35);
	_noises[Ore].SetCellularJitter(0.75);

	_noises[OreDimond].SetNoiseType(FastNoise::Cellular);
	_noises[OreDimond].SetSeed(1330);
	_noises[OreDimond].SetFrequency(0.35);
	_noises[OreDimond].SetCellularJitter(0.85);

	_noiseNames[Basic] = "Basic";
	_noiseNames[GrassLand] = "GrassLand";
	_noiseNames[Swamp] = "Swamp";
	_noiseNames[Desert] = "Desert";
	_noiseNames[Snow] = "Snow";
	_noiseNames[HighLand] = "HighLand";
	_noiseNames[Biomes] = "BiomeDefinition";
	_noiseNames[BeachBordered] = "BeachBordered";
	_noiseNames[River] = "River";
	_noiseNames[Tree] = "Tree";
	_noiseNames[Vegetation] = "Vegetation";
	_noiseNames[Caves] = "Caves";
	_noiseNames[Crevices] = "Crevices";
	_noiseNames[PerlinX] = "PerlinX";
	_noiseNames[PerlinY] = "PerlinY";
	_noiseNames[Ore] = "Ore";
	_noiseNames[OreDimond] = "OreDimond";
}

FastNoise& MapGeneration::GetNoise(MapGeneration::GenerationType genType) {return _noises[genType];};

float MapGeneration::GetExpValue() {return _exp;};

float MapGeneration::GetTeracceValue() {return _terraceValue;};

void MapGeneration::SetExpValue(float value) {_exp = value;};

void MapGeneration::SetTerraceValue(float value) {_terraceValue = value;};

std::string MapGeneration::GetNoiseName(GenerationType t) {return _noiseNames[t];};