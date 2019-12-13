#include "Generation/MapGeneration.h"
#include "Generation/BiomeDefine.h"
#include <iostream>
#define LERP MapGeneration::_Lerp
#define VEC3 glm::vec3

// int MapGeneration::BiomeDefinition(float e, float m)
// {
//   if (e < 0.1)
//     return OCEAN;
//   if (e < 0.12) return BEACH;
  
//   if (e > 0.8) {
//     if (m < 0.1) return SCORCHED;
//     if (m < 0.2) return BARE;
//     if (m < 0.5) return TUNDRA;
//     return SNOW;
//   }

//   if (e > 0.6) {
//     if (m < 0.33) return TEMPERATE_DESERT;
//     if (m < 0.66) return SHRUBLAND;
//     return TAIGA;
//   }

//   if (e > 0.3) {
//     if (m < 0.16) return TEMPERATE_DESERT;
//     if (m < 0.50) return GRASSLAND;
//     if (m < 0.83) return TEMPERATE_DECIDUOUS_FOREST;
//     return TEMPERATE_RAIN_FOREST;
//   }

//   if (m < 0.16) return SUBTROPICAL_DESERT;
//   if (m < 0.33) return GRASSLAND;
//   if (m < 0.66) return TROPICAL_SEASONAL_FOREST;
//   return TROPICAL_RAIN_FOREST;
// }


float MapGeneration::Smoothstep(float edge0, float edge1, float x)
{
    // Scale, bias and saturate x to 0..1 range
    x = glm::clamp((x - edge0) / (edge1 - edge0), 0.f, 1.f);
    // Evaluate polynomial
    return x*x*(3 - 2 * x);
}

int MapGeneration::BiomeGeneration(glm::ivec2 pos, glm::ivec2 blockPosition)
{
  FastNoise noise = _noises[BiomeDefinition];
  
  float globalX = pos.x * 16, globalY = pos.y * 16;
  float nx = globalX + blockPosition.x, ny = globalY + blockPosition.y;
  int biome = floorf((noise.GetNoise(nx, ny) * 0.5 + 0.5) * 10);

  if (biome == 1)
    return OCEAN;
  else if (biome == 2)
    return BEACH;
  else if (biome >= 3 && biome <= 5)
    return GRASSLAND;
  else if (biome < 9)
    return (TAIGA);
  else
    return (GRASSLAND);
  // if (biome )
}

float MapGeneration::_Hash(const float n)
{
    float x = sin(n) * 43758.5453f;
    return x - floor(x);
}



float MapGeneration::Noise(const glm::vec3 &x)
{
    glm::vec3 p(floor(x.x), floor(x.y), floor(x.z));
    glm::vec3 f(x.x - p.x, x.y - p.y, x.z - p.z);
    f = f * (f * (glm::vec3(3.f, 3.f, 3.f) - f * 2.f));
    float n = glm::dot(p, glm::vec3(1.f, 57.f, 113.f));
    return LERP(LERP(
                    LERP(_Hash(n +  0.f), _Hash(n +  1.f), f.x),
                    LERP(_Hash(n + 57.f), _Hash(n + 58.f), f.x), f.y),
                LERP(
                    LERP(_Hash(n + 113.f), _Hash(n + 114.f), f.x),
                    LERP(_Hash(n + 170.f), _Hash(n + 171.f), f.x), f.y), f.z);
}

MapGeneration::StoredMapData  MapGeneration::HighLandGenerationColumn(glm::ivec2 pos, glm::ivec2 blockPosition)
{
  FastNoise noise = _noises[HighLand];
  float exp = _exp;
  float terraceValue = _terraceValue;

  float globalX = pos.x * 16, globalY = pos.y * 16;
  float nx = globalX + blockPosition.x, ny = globalY + blockPosition.y;
  float e = 1.f * (noise.GetNoise(nx, ny));
  float e1 = 0.50f * (noise.GetNoise(2.f * nx, 2.f * ny));
  float e2 = 0.25f * (noise.GetNoise(4.f * nx, 4.f * ny));
  // float e3 = 0.13f * (noise.GetNoise(8.f * nx, 8.f * ny));
  // float e4 = 0.06f * (noise.GetNoise(16.f * nx, 16.f * ny));
  // float e5 = 0.03f * (noise.GetNoise(32.f * nx, 32.f * ny));
  // e += e1 + e2 + e3 + e4 + e5;
  e += e1 + e2;
  e = (e * 0.5f + 0.5f) * 10;

  float mountainPeaks = pow(e, exp);
  float terrace = round(mountainPeaks * terraceValue) / terraceValue;
  // std::cout << mountainPeaks << std::endl;
  // std::cout << terrace * 10 << std::endl;
  StoredMapData block;
  block.elevation = terrace;
  return block;
}

MapGeneration::StoredMapData  MapGeneration::LandGenerationColumn(glm::ivec2 pos, glm::ivec2 blockPosition)
{
	FastNoise noise = _noises[Land];
  float terraceValue = _terraceValue;

  float globalX = pos.x * 16, globalY = pos.y * 16;

  float nx = globalX + blockPosition.x, ny = globalY + blockPosition.y;
  float e = 0.5f * (noise.GetNoise(2.f * nx, 2.f * ny));
  e = (e * 0.5f + 0.5f) * 10.f;
  float terrace = round(e * terraceValue) / terraceValue;

  StoredMapData block;
  block.elevation = terrace;
  return block;
}

MapGeneration::StoredMapData MapGeneration::BasicGenerationColumn(glm::ivec2 pos, glm::ivec2 blockPosition)
{
  FastNoise noise = _noises[Basic];
  float terraceValue = _terraceValue;

  float globalX = pos.x * 16, globalY = pos.y * 16;
  float nx = globalX + blockPosition.x, ny = globalY + blockPosition.y;

  float e = 1.f * (noise.GetNoise(1.f * nx, 1.f * ny));
  float e1 = 0.50f * (noise.GetNoise(2.f * nx, 2.f * ny));
  // float e2 = 0.25f * (noise.GetNoise(4.f * nx, 4.f * ny));
  // float e3 = 0.13f * (noise.GetNoise(8.f * nx, 8.f * ny));
  // float e4 = 0.06f * (noise.GetNoise(16.f * nx, 16.f * ny));
  // float e5 = 0.03f * (noise.GetNoise(32.f * nx, 32.f * ny));
  e += e1;
  float m = 1.f * (noise.GetNoise(1.f * nx, 1.f * ny));
  float m1 = 0.75f * (noise.GetNoise(2.f * nx, 2.f * ny));
  float m2 = 0.33f * (noise.GetNoise(4.f * nx, 4.f * ny));
  // float m3 = 0.33f * (noise.GetNoise(8.f * nx, 8.f * ny));
  // float m4 = 0.33f * (noise.GetNoise(16.f * nx, 16.f * ny));
  // float m5 = 0.50f * (noise.GetNoise(32.f * nx, 32.f * ny));
  m += m1 + m2 + m2;//+ m3 + m4 + m5;
  m /= (1.0f + 0.75f + 0.33f + 0.33f + 0.33f + 0.50f);
  e = (e * 0.5f + 0.5f) * 10.f;
  float terrace = round(e * terraceValue) / terraceValue;

  StoredMapData block;
  block.elevation = terrace;
  // block.biom = BiomeDefinition(e, m);
  return block;
  // (umap)[position] = new StoredMapData(pow(e, EXP) - 0.42); // резкие горные пики
  // (umap)[position] = new StoredMapData((round((e * 16) / 16))); // терассы
}

MapGeneration::StoredMapData MapGeneration::Generation(MapGeneration::GenerationType genType, glm::ivec2 globalPos, glm::ivec2 blockPosition)
{
  switch (genType)
  {
    case GenerationType::BiomeDefinition:
    {
      StoredMapData b;
      b.biom = BiomeGeneration(globalPos, blockPosition);
      return b;
    }
      break;
    case GenerationType::Basic:
        return BasicGenerationColumn(globalPos, blockPosition);
      break;
    case GenerationType::Land:
        return LandGenerationColumn(globalPos, blockPosition);
      break;
    case GenerationType::HighLand:
        return HighLandGenerationColumn(globalPos, blockPosition);
      break;
    default:
        return BasicGenerationColumn(globalPos, blockPosition);
      break;
  }
}

MapGeneration::MapGeneration()
{
  _exp = 2.2f;
  _terraceValue = 32.f;
  _noises[BiomeDefinition].SetNoiseType(FastNoise::Cellular);
  _noises[BiomeDefinition].SetSeed(1330);
  _noises[BiomeDefinition].SetFrequency(0.001);
  _noises[BiomeDefinition].SetCellularReturnType(FastNoise::CellValue);
  _noises[BiomeDefinition].SetCellularDistanceFunction(FastNoise::Natural);
  _noises[Basic].SetNoiseType(FastNoise::Perlin);
  _noises[Basic].SetFrequency(0.1);
  _noises[Land].SetNoiseType(FastNoise::Simplex);
  _noises[Land].SetFrequency(0.01);
  _noises[HighLand].SetNoiseType(FastNoise::Perlin);
  _noises[HighLand].SetFrequency(0.01);
  _noiseNames[Basic] = "Basic";
  _noiseNames[Land] = "Land";
  _noiseNames[HighLand] = "HighLand";
  _noiseNames[BiomeDefinition] = "BiomeDefinition";
}

FastNoise& MapGeneration::GetNoise(MapGeneration::GenerationType genType) {return _noises[genType];};

float MapGeneration::GetExpValue() {return _exp;};

float MapGeneration::GetTeracceValue() {return _terraceValue;};

void MapGeneration::SetExpValue(float value) {_exp = value;};

void MapGeneration::SetTerraceValue(float value) {_terraceValue = value;};

std::string MapGeneration::GetNoiseName(GenerationType t) {return _noiseNames[t];};