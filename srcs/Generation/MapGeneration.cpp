#include "Generation/MapGeneration.h"
#include "World/Block.h"


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

bool MapGeneration::CrevicesGeneration(glm::ivec2 chunkPos, glm::ivec3 columnPos)
{
	FastNoise& noise = _noises[Crevices];

	glm::ivec2 globalChunkPos = chunkPos * 16;
	glm::ivec3 pos = glm::ivec3(globalChunkPos.x + columnPos.x, columnPos.y, globalChunkPos.y + columnPos.z);

	//! May be thread dangerous
	float terraceValue = _terraceValue;
	float e = noise.GetNoise(pos.x * 0.55f, pos.y, pos.z * 3.5f);
	e = (e * 0.5f + 0.5f);
	if (e < 0.815f)
		return false;
	return true;
}

bool MapGeneration::CavesGeneration(glm::ivec2 chunkPos, glm::ivec3 columnPos)
{
	FastNoise& noise = _noises[Caves];

	glm::ivec2 globalChunkPos = chunkPos * 16;
	glm::ivec3 pos = glm::ivec3(globalChunkPos.x + columnPos.x, columnPos.y, globalChunkPos.y + columnPos.z);

	float e = noise.GetNoise(pos.x, pos.y, pos.z);
	e = (e * 0.5f) + 0.5f;

	if (e < 0.69751f)
		return false;
	return true;
}

MapGeneration::StoredMapData MapGeneration::Generation(glm::ivec2 chunkPos, glm::ivec2 columnPos)
{
	glm::ivec2 globalChunkPos = chunkPos * 16;
	glm::ivec2 pos = globalChunkPos + columnPos;

	GenerationType biom = GenerationBiome(pos);
	return Generation(chunkPos, columnPos, biom);
}

MapGeneration::StoredMapData MapGeneration::Generation(glm::ivec2 chunkPos, glm::ivec2 columnPos, MapGeneration::GenerationType genType)
{
	StoredMapData column;

	glm::ivec2 globalChunkPos = chunkPos * 16;
	glm::ivec2 pos = globalChunkPos + columnPos;

	column.firstBlockLayer = Block::Air;
	column.lastBlockLayer = Block::Air;
	column.treeType = Trees::Nothing;
	column.biom = genType;

	switch (genType)
	{
		case GenerationType::Basic:
			GetBasicData(column, pos);
			break;
		case GenerationType::HighLand:
			GetHighLandData(column, pos);
			break;
		case GenerationType::ChineseMountains:
			GetChineseMountainsData(column, pos);
			break;
		case GenerationType::GrassLand:
			GetGrassLandData(column, pos);
			break;
		case GenerationType::Glade:
			GetGladeData(column, pos);
			break;
		case GenerationType::Swamp:
			GetSwampData(column, pos);
			break;
		case GenerationType::Desert:
			GetDesertData(column, pos);
			break;
		case GenerationType::Snow:
			GetSnowLandData(column, pos);
			break;
		case GenerationType::GrassLandHills:
			GetGrassLandHillsData(column, pos);
			break;
		case GenerationType::DesertHills:
			GetDesertHillsData(column, pos);
			break;
		case GenerationType::SnowHills:
			GetSnowHillsData(column, pos);
			break;
		case GenerationType::Tree:
			column.approximateElevation = TreeGeneration(pos);
			column.firstBlockLayer = Block::Leaves;
			column.lastBlockLayer = Block::Log;
			break;
		case GenerationType::River:
			GetRiverData(column, pos);
			break;
		case GenerationType::Ocean:
			GetOceanData(column, pos);
			break;
		default:
			GetBasicData(column, pos);
			break;
	}
	column.exactElevation = glm::clamp((int)column.approximateElevation, -10, 255);
	// column.exactElevation = glm::clamp((int)column.approximateElevation, 0, 255);
	return column;
}


MapGeneration::MapGeneration()
{
	_exp = 1.2f;
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

	_noises[GladeVegetation].SetNoiseType(FastNoise::Cellular);
	_noises[GladeVegetation].SetSeed(1300);
	_noises[GladeVegetation].SetFrequency(0.23f);
	_noises[GladeVegetation].SetCellularDistanceFunction(FastNoise::Natural);

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

	_noises[Hills].SetNoiseType(FastNoise::Simplex);
	_noises[Hills].SetSeed(6128);
	_noises[Hills].SetFrequency(0.0110615);

	_noises[Glade].SetNoiseType(FastNoise::PerlinFractal);
	_noises[Glade].SetFrequency(0.00214);

	_noises[Swamp].SetNoiseType(FastNoise::Simplex);
	_noises[Swamp].SetFrequency(0.021);

	_noises[Desert].SetNoiseType(FastNoise::Simplex);
	_noises[Desert].SetFrequency(0.01);

	_noises[Snow].SetNoiseType(FastNoise::Perlin);
	_noises[Snow].SetFrequency(0.01);

	_noises[HighLand].SetNoiseType(FastNoise::Perlin);
	_noises[HighLand].SetFrequency(0.01);

	_noises[ChineseMountains].SetNoiseType(FastNoise::Cellular);
	_noises[ChineseMountains].SetCellularDistanceFunction(FastNoise::Natural);
	_noises[ChineseMountains].SetFrequency(0.05);
	_noises[ChineseMountains].SetCellularJitter(0.85f);

	_noises[Ore].SetNoiseType(FastNoise::Cellular);
	_noises[Ore].SetFrequency(0.35);
	_noises[Ore].SetCellularJitter(0.75);

	_noises[OreDimond].SetNoiseType(FastNoise::Cellular);
	_noises[OreDimond].SetSeed(1330);
	_noises[OreDimond].SetFrequency(0.35);
	_noises[OreDimond].SetCellularJitter(0.85);

	_noiseNames[Basic] = "Basic";
	_noiseNames[GrassLand] = "GrassLand";
	_noiseNames[Hills] = "Hills";
	_noiseNames[Glade] = "Glade";
	_noiseNames[Swamp] = "Swamp";
	_noiseNames[Desert] = "Desert";
	_noiseNames[Snow] = "Snow";
	_noiseNames[HighLand] = "HighLand";
	_noiseNames[ChineseMountains] = "ChineseMountains";
	_noiseNames[Biomes] = "BiomeDefinition";
	_noiseNames[BeachBordered] = "BeachBordered";
	_noiseNames[River] = "River";
	_noiseNames[Tree] = "Tree";
	_noiseNames[Vegetation] = "Vegetation";
	_noiseNames[GladeVegetation] = "GladeVegetation";
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