#include "Generation/MapGeneration.h"
// #include "Generation/BiomeDefine.h"
#include <iostream>
#define LERP MapGeneration::Lerp

glm::vec2 MapGeneration::random2(glm::vec2 p)
{
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

float MapGeneration::Lerp(float v0, float v1, float t)
{
  t = glm::clamp(t, 0.f, 1.f);
	return (1.f - t) * v0 + t * v1;
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
  e = (e * 0.5f + 0.5f); // range 0..~10.3

  float mountainPeaks = pow(e, exp);
  float terrace = round(mountainPeaks * terraceValue) / terraceValue;

  // int elevation = (int)floorf(terrace);
  float elevation = terrace;
  return elevation;
}

float MapGeneration::BeachGenerationColumn(glm::ivec2 pos, int biome)
{
	FastNoise noise = _noises[Beach];
  float terraceValue = _terraceValue;
  float e = 1.f * (noise.GetNoise(1.f * pos.x, 1.f * pos.y));
  e = (e * 0.5f + 0.5f); // range 0..1;
  float terrace = round(e * terraceValue) / terraceValue;  
  float elevation = terrace;
  // int elevation = (int)floorf(terrace);
  return elevation;
}

float MapGeneration::LandGenerationColumn(glm::ivec2 pos, int biome)
{
	FastNoise noise = _noises[GrassLand];
  float terraceValue = _terraceValue;

  float e = 0.5f * (noise.GetNoise(2.f * pos.x, 2.f * pos.y));
  e = (e * 0.5f + 0.5f); // range 0..1;
  

  float terrace = round(e * terraceValue) / terraceValue;  
  float elevation = terrace;
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

float MapGeneration::CheckingTheBiomeInTheNextColumn(glm::ivec2 originPos, int originBiome, int distance_x, int distance_y) // x,y - coord. z - bioms
{
  FastNoise noise = _noises[Biomes];
  glm::vec2 nextBlock = glm::vec2(originPos.x + (1 * distance_x), originPos.y + (1 * distance_y));

  int biome = floorf((noise.GetNoise(nextBlock.x, nextBlock.y) * 0.5 + 0.5) * 10);
  biome = BiomeDefinition(biome);
  if (biome != originBiome)
  {
    FastNoise nextNoise[Size];
    float elevation = (nextNoise[biome].GetNoise((float)nextBlock.x, (float)nextBlock.y) * 0.5 + 0.5);
    return elevation;
  }

  return -1.f;
}

void MapGeneration::SmoothingButtJoint(float& elevation, glm::ivec2 pos, int biome)
{
  float distance = 1.f;
  float rightBlock;
  float leftBlock;
  float topBlock;
  float buttomBlock;

  while (1337)
  {
    rightBlock = CheckingTheBiomeInTheNextColumn(pos, biome, (int)distance, 0);
    if (rightBlock != -1.f)
    {
      distance = distance * STEP;
      elevation = Lerp(rightBlock, elevation, distance);
      return;
    }

    leftBlock = CheckingTheBiomeInTheNextColumn(pos, biome, (int)-distance, 0);
    if (leftBlock != -1.f)
    {
      distance = distance * STEP;
      elevation = Lerp(leftBlock, elevation, distance);
      return;
    }

    topBlock = CheckingTheBiomeInTheNextColumn(pos, biome, 0, (int)distance);
    if (topBlock != -1.f)
    {
      distance = distance * STEP;
      elevation = Lerp(topBlock, elevation, distance);
      return;
    }

    buttomBlock = CheckingTheBiomeInTheNextColumn(pos, biome, 0, (int) -distance);
    if (buttomBlock != -1.f)
    {
      distance = distance * STEP;
      elevation = Lerp(buttomBlock, elevation, distance);
      return;
    }
    if (distance >= MAX_DIST_TO_SMOOTHING)
      return;
    distance++;
  }
}

int MapGeneration::BiomeDefinition(int elevation)
{
  if (elevation == 1)
    return Ocean;
  else if (elevation == 2)
    return Beach;
  else if (elevation >= 3 && elevation < 8)
    return GrassLand;
  // // else if (elevation == 4)
  //   return TEMPERATE_DECIDUOUS_FOREST;
  // else if (elevation == 5)
  //   return TEMPERATE_RAIN_FOREST;
  // else if (elevation == 6)
  //   return TUNDRA;
  // else if (elevation == 7)
  //   return SNOW;
  // else if (elevation == 8)
  //   return SNOW;
  else if (elevation == 9)
    return HighLand;
  else
    return GrassLand;
}

int MapGeneration::BiomeGeneration(glm::ivec2 pos, glm::ivec2 blockPosition)
{
  FastNoise& noise = _noises[Biomes];

  float globalX = pos.x * 16, globalY = pos.y * 16;
  float nx = globalX + blockPosition.x, ny = globalY + blockPosition.y;

  int elevation = floorf((noise.GetNoise(nx, ny) * 0.5 + 0.5) * 10);
  return BiomeDefinition(elevation);
}

MapGeneration::StoredMapData MapGeneration::Generation(glm::ivec2 globalPos, glm::ivec2 blockPosition)
{
  StoredMapData column;
  float globalX = globalPos.x * 16, globalY = globalPos.y * 16;
  glm::vec2 pos = glm::ivec2(globalX + blockPosition.x, globalY + blockPosition.y);

  column.biom = BiomeGeneration(globalPos, blockPosition);
  switch (column.biom)
  {
    case GenerationType::GrassLand:
    {
      column.firstBlockLayer = BlockType::Dirt;
      column.lastBlockLayer = BlockType::Grass;
      column.elevation = LandGenerationColumn(pos, column.biom);
    }
      break;
    case GenerationType::Beach:
    {
      column.firstBlockLayer = BlockType::Sand;
      column.lastBlockLayer = BlockType::Sand;
      column.elevation = BeachGenerationColumn(pos, column.biom);
    }
      break;
    case GenerationType::HighLand:
    {
      column.firstBlockLayer = BlockType::Stone;
      column.lastBlockLayer = BlockType::Stone;
      column.elevation = HighLandGenerationColumn(pos, column.biom);
      
      if (column.elevation > 75 && column.elevation < 86)
        column.lastBlockLayer = BlockType::Planks; //As planned, it should be snowing
      else if (column.elevation >= 86)
        column.lastBlockLayer = BlockType::Grass; //As planned, it should be ice
    }
      break;
    default:
    {
      column.firstBlockLayer = BlockType::Dirt;
      column.lastBlockLayer = BlockType::Grass;
      column.elevation = LandGenerationColumn(pos, column.biom);
    }
  }
  SmoothingButtJoint(column.elevation, pos, column.biom);
  return column;
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
    case GenerationType::GrassLand:
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
  _noises[Biomes].SetNoiseType(FastNoise::Cellular);
  _noises[Biomes].SetSeed(1330);
  _noises[Biomes].SetFrequency(0.001);
  _noises[Biomes].SetCellularReturnType(FastNoise::CellValue);
  _noises[Biomes].SetCellularDistanceFunction(FastNoise::Natural);

  _noises[Basic].SetNoiseType(FastNoise::Perlin);
  _noises[Basic].SetFrequency(0.1);

  _noises[GrassLand].SetNoiseType(FastNoise::Simplex);
  _noises[GrassLand].SetFrequency(0.01);

  _noises[Beach].SetNoiseType(FastNoise::Simplex);
  _noises[Beach].SetFrequency(0.01);

  _noises[HighLand].SetNoiseType(FastNoise::Perlin);
  _noises[HighLand].SetFrequency(0.01);

  _noiseNames[Basic] = "Basic";
  _noiseNames[GrassLand] = "GrassLand";
  _noiseNames[Beach] = "Beach";
  _noiseNames[HighLand] = "HighLand";
  _noiseNames[Biomes] = "BiomeDefinition";
}

FastNoise& MapGeneration::GetNoise(MapGeneration::GenerationType genType) {return _noises[genType];};

float MapGeneration::GetExpValue() {return _exp;};

float MapGeneration::GetTeracceValue() {return _terraceValue;};

void MapGeneration::SetExpValue(float value) {_exp = value;};

void MapGeneration::SetTerraceValue(float value) {_terraceValue = value;};

std::string MapGeneration::GetNoiseName(GenerationType t) {return _noiseNames[t];};