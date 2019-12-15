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

glm::vec2 MapGeneration::random2(glm::vec2 p) {
    return glm::fract(glm::sin(glm::vec2(dot(p, glm::vec2(127.1f ,311.7f)), glm::dot(p, glm::vec2(269.5f, 183.3f)))) * 43758.5453f);
}

float MapGeneration::Smoothstep(float edge0, float edge1, float x)
{
    x = glm::clamp((x - edge0) / (edge1 - edge0), 0.f, 1.f);
    return x * x * (3 - 2 * x);
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

float  MapGeneration::HighLandGenerationColumn(glm::ivec2 pos, int biome)
{
  FastNoise noise = _noises[HighLand];
  float exp = _exp;
  float terraceValue = _terraceValue;

  float e = 1.f * (noise.GetNoise(pos.x, pos.y));
  float e1 = 0.50f * (noise.GetNoise(2.f * pos.x, 2.f * pos.y));
  float e2 = 0.25f * (noise.GetNoise(4.f * pos.x, 4.f * pos.y));
  // float e3 = 0.13f * (noise.GetNoise(8.f * nx, 8.f * ny));
  // float e4 = 0.06f * (noise.GetNoise(16.f * nx, 16.f * ny));
  // float e5 = 0.03f * (noise.GetNoise(32.f * nx, 32.f * ny));
  // e += e1 + e2 + e3 + e4 + e5;
  e += e1 + e2;
  e = (e * 0.5f + 0.5f) * 10; // range 0..~103

  float mountainPeaks = pow(e, exp);
  float terrace = round(mountainPeaks * terraceValue) / terraceValue;

  int elevation = (int)floorf(terrace);
  return elevation;
}

float MapGeneration::BeachGenerationColumn(glm::ivec2 pos, int biome)
{
	FastNoise noise = _noises[BeachLand];
  float terraceValue = _terraceValue;

  float e = 1.f * (noise.GetNoise(1.f * pos.x, 1.f * pos.y));
  e = (e * 0.5f + 0.5f) * 10.f; // range 0..10;
  
  // if (CheckingTheBiomeInTheNextColumn(glm::ivec3(pos, biome)))
    // std::cout << "border: " << pos.x << " " << pos.y << std::endl;

  float terrace = round(e * terraceValue) / terraceValue;  
  int elevation = (int)floorf(terrace);
  return elevation;
}

float MapGeneration::LandGenerationColumn(glm::ivec2 pos, int biome)
{
	FastNoise noise = _noises[Land];
  float terraceValue = _terraceValue;

  float e = 0.5f * (noise.GetNoise(2.f * pos.x, 2.f * pos.y));
  e = (e * 0.5f + 0.5f) * 10.f; // range 0..10;
  
  // if (CheckingTheBiomeInTheNextColumn(glm::ivec3(pos, biome)))
    // std::cout << "border: " << pos.x << " " << pos.y << std::endl;

  float terrace = round(e * terraceValue) / terraceValue;  
  int elevation = (int)floorf(terrace);
  // std::cout << elevation << std::endl;
  return elevation;
}

float MapGeneration::BasicGenerationColumn(glm::ivec2 pos)
{
  FastNoise noise = _noises[Basic];
  float terraceValue = _terraceValue;

  float e = 1.f * (noise.GetNoise(1.f * pos.x, 1.f * pos.y));
  float e1 = 0.50f * (noise.GetNoise(2.f * pos.x, 2.f * pos.y));

  e += e1;
  e = (e * 0.5f + 0.5f) * 10.f;

  float terrace = round(e * terraceValue) / terraceValue;  
  int elevation = (int)floorf(terrace);
  return elevation;
}


bool MapGeneration::CheckingTheBiomeInTheNextColumn(glm::ivec3 middle) // x,y - coord. z - bioms
{
  FastNoise noise = _noises[BiomeDefinition];

  glm::ivec3 top = glm::ivec3(middle.x, middle.y + 1, 0);
  glm::ivec3 right = glm::ivec3(middle.x + 1, middle.y, 0);
  glm::ivec3 left = glm::ivec3(middle.x - 1, middle.y, 0);
  glm::ivec3 buttom = glm::ivec3(middle.x, middle.y - 1, 0);

  top.z = floorf((noise.GetNoise(top.x, top.y) * 0.5 + 0.5) * 10);
  if (top.z != middle.z)
    return true;

  right.z = floorf((noise.GetNoise(right.x, right.y) * 0.5 + 0.5) * 10);
  if (right.z != middle.z)
    return true;
 
  left.z = floorf((noise.GetNoise(left.x, left.y) * 0.5 + 0.5) * 10);
  if (left.z != middle.z)
    return true;

  buttom.z = floorf((noise.GetNoise(buttom.x, buttom.y) * 0.5 + 0.5) * 10);
  if (buttom.z != middle.z)
    return true;
  
  return false;
}

int MapGeneration::BiomeGeneration(glm::ivec2 pos, glm::ivec2 blockPosition)
{
  FastNoise noise = _noises[BiomeDefinition];

  float globalX = pos.x * 16, globalY = pos.y * 16;
  float nx = globalX + blockPosition.x, ny = globalY + blockPosition.y;
  
  int biom = floorf((noise.GetNoise(nx, ny) * 0.5 + 0.5) * 10);

  if (biom == 1)
    return OCEAN;
  else if (biom == 2)
    return BEACH;
  else if (biom >= 3 && biom <= 5)
    return GRASSLAND;
  else if (biom < 9)
    return TAIGA;
  else
    return GRASSLAND;
}

MapGeneration::StoredMapData MapGeneration::Generation(glm::ivec2 globalPos, glm::ivec2 blockPosition)
{
  StoredMapData column;
  float globalX = globalPos.x * 16, globalY = globalPos.y * 16;
  glm::vec2 pos = glm::ivec2(globalX + blockPosition.x, globalY + blockPosition.y);

  column.biom = BiomeGeneration(globalPos, blockPosition);
  switch (column.biom)
  {
    case GRASSLAND:
    {
      column.firstBlockLayer = BlockType::Dirt;
      column.lastBlockLayer = BlockType::Grass;
      column.elevation = LandGenerationColumn(pos, column.biom);
      return column;
    }
      break;
    case BEACH:
    {
      column.firstBlockLayer = BlockType::Sand;
      column.lastBlockLayer = BlockType::Sand;
      column.elevation = BeachGenerationColumn(pos, column.biom);
      return column;
    }
    case TAIGA:
    {
      column.firstBlockLayer = BlockType::Stone;
      column.lastBlockLayer = BlockType::Dirt;
      column.elevation = HighLandGenerationColumn(pos, column.biom);
      
      if (column.elevation > 75 && column.elevation < 86)
        column.lastBlockLayer = BlockType::Planks; //As planned, it should be snowing
      else if (column.elevation >= 86)
        column.lastBlockLayer = BlockType::Grass; //As planned, it should be ice

      return column;
    }
      break;
    default:
    {
      column.firstBlockLayer = BlockType::Dirt;
      column.lastBlockLayer = BlockType::Grass;
      column.elevation = LandGenerationColumn(pos, column.biom);
      return column;
    }
  }
}


MapGeneration::StoredMapData MapGeneration::Generation(glm::ivec2 globalPos, glm::ivec2 blockPosition, MapGeneration::GenerationType genType)
{
  StoredMapData column;

  float globalX = globalPos.x * 16, globalY = globalPos.y * 16;
  glm::vec2 pos = glm::ivec2(globalX + blockPosition.x, globalY + blockPosition.y);

  switch (genType)
  {
    case GenerationType::Basic:
    {
      column.biom = 0;
      column.elevation = BasicGenerationColumn(pos);
      column.firstBlockLayer = BlockType::Stone;
      column.lastBlockLayer = BlockType::Stone;
      return column;
    }
      break;
    case GenerationType::Land:
    {
      column.biom = 0;
      column.elevation = LandGenerationColumn(pos, column.biom);
      column.firstBlockLayer = BlockType::Dirt;
      column.lastBlockLayer = BlockType::Grass;
      return column;
    }
      break;
    case GenerationType::HighLand:
    {
      column.biom = 0;
      column.elevation = HighLandGenerationColumn(pos, column.biom);
      column.firstBlockLayer = BlockType::Stone;
      column.lastBlockLayer = BlockType::Dirt;
      return column;
    }
      break;
    default:
    {
      column.biom = 0;
      column.elevation = BasicGenerationColumn(pos);
      column.firstBlockLayer = BlockType::Stone;
      column.lastBlockLayer = BlockType::Stone;
      return column;
    }
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

  _noises[BeachLand].SetNoiseType(FastNoise::Simplex);
  _noises[BeachLand].SetFrequency(0.01);

  _noises[HighLand].SetNoiseType(FastNoise::Perlin);
  _noises[HighLand].SetFrequency(0.01);

  _noiseNames[Basic] = "Basic";
  _noiseNames[Land] = "Land";
  _noiseNames[BeachLand] = "BeachLand";
  _noiseNames[HighLand] = "HighLand";
  _noiseNames[BiomeDefinition] = "BiomeDefinition";
}

FastNoise& MapGeneration::GetNoise(MapGeneration::GenerationType genType) {return _noises[genType];};

float MapGeneration::GetExpValue() {return _exp;};

float MapGeneration::GetTeracceValue() {return _terraceValue;};

void MapGeneration::SetExpValue(float value) {_exp = value;};

void MapGeneration::SetTerraceValue(float value) {_terraceValue = value;};

std::string MapGeneration::GetNoiseName(GenerationType t) {return _noiseNames[t];};