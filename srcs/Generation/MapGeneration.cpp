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

float MapGeneration::CrevicesGeneration(glm::ivec2 pos)
{
  FastNoise& noise = _noises[Crevices];

  float terraceValue = _terraceValue;

  float e = 1.f * (noise.GetNoise(pos.x * 0.55f, pos.y * 3.5f));
  e = (e * 0.5f + 0.5f) * 10;

  if (e < 8)
    return -1.f;
  else
    e = 9;

  float terrace = round(e * terraceValue) / terraceValue;
  return terrace;
}

float MapGeneration::SecondElevationCavesGeneration(glm::ivec2 pos)
{
  FastNoise& noise = _noises[SecondElevationCaves];

  float terraceValue = _terraceValue;
  float exp = _exp;

  float e = 1.f * (noise.GetNoise(pos.x, pos.y));
  float e1 = 0.50f * (noise.GetNoise(2.f * pos.x, 2.f * pos.y));

  e += e1;
  e = (e * 0.5f + 0.5f) * 10;

  float heightDifference = pow(e, exp);
  float elevation = round(heightDifference * terraceValue) / terraceValue;
  return elevation;
}

float MapGeneration::SecondShapeCavesGeneration(glm::ivec2 pos)
{
  FastNoise& noise = _noises[SecondShapeCaves];

  float terraceValue = _terraceValue;

  float e = 1.f * (noise.GetNoise(pos.x, pos.y));
  e = (e * 0.5f + 0.5f) * 10;
  e = glm::clamp((int)e, 0, 255);
  if (e == 3 || e == 4)
    e = 4.f;
  else
    return -1.f;

  float terrace = round(e * terraceValue) / terraceValue;
  return terrace;
}

float MapGeneration::ElevationCavesGeneration(glm::ivec2 pos)
{
  FastNoise& noise = _noises[ElevationCaves];

  float terraceValue = _terraceValue;
  float exp = _exp;


  float e = 1.f * (noise.GetNoise(pos.x, pos.y));
  float e1 = 0.50f * (noise.GetNoise(2.f * pos.x, 2.f * pos.y));
  float e2 = 0.25f * (noise.GetNoise(4.f * pos.x, 4.f * pos.y));
  float e3 = 0.13f * (noise.GetNoise(8.f * pos.x, 8.f * pos.y));
  e += e1 + e2 + e3;
  e = (e * 0.5f + 0.5f) * 10;


  float heightDifference = pow(e, exp);
  float elevation = round(heightDifference * terraceValue) / terraceValue;
  return elevation;
}

float MapGeneration::ShapeCavesGeneration(glm::ivec2 pos)
{
  FastNoise& noise = _noises[ShapeCaves];

  float terraceValue = _terraceValue;

  float e = 1.f * (noise.GetNoise(pos.x, pos.y));
  e = (e * 0.5f + 0.5f) * 10;
  e = glm::clamp((int)e, 0, 255);
  if (e == 3 || e == 4)
    e = 4.f;
  else
    return -1.f;

  float terrace = round(e * terraceValue) / terraceValue;
  return terrace;
}

float MapGeneration::TreeGeneration(glm::ivec2 pos)
{
  FastNoise& noise = _noises[Tree];
  int R = 3;
  float max = 0;

  for (int yn = pos.y - R; yn <= pos.y + R; yn++) {
    for (int xn = pos.x - R; xn <= pos.x + R; xn++) {
      float e = noise.GetNoise(xn, yn);
      if (e > max)
        max = e;
    }
  }
  if (noise.GetNoise(pos.x, pos.y) == max)
    return 1.f;
  return -1.f;
}

float  MapGeneration::HighLandGenerationColumn(glm::ivec2 pos)
{
  FastNoise& noise = _noises[HighLand];
  float exp = _exp;
  float terraceValue = _terraceValue;

  float e = 1.f * (noise.GetNoise(pos.x, pos.y));
  float e1 = 0.50f * (noise.GetNoise(2.f * pos.x, 2.f * pos.y));
  float e2 = 0.25f * (noise.GetNoise(4.f * pos.x, 4.f * pos.y));
  // float e3 = 0.13f * (noise.GetNoise(8.f * pos.x, 8.f * pos.y));
  // float e4 = 0.06f * (noise.GetNoise(16.f * pos.x, 16.f * pos.y));
  // float e5 = 0.03f * (noise.GetNoise(32.f * pos.x, 32.f * pos.y));
  // e += e1 + e2 + e3 + e4 + e5;
  e += e1 + e2;
  e = (e * 0.5f + 0.5f) * 10; // range 0..~10.3

  float mountainPeaks = pow(e, exp);
  float terrace = round(mountainPeaks * terraceValue) / terraceValue;

  // float elevation = (int)floorf(terrace);
  float elevation = terrace;
  return elevation;
}

float MapGeneration::BeachGenerationColumn(glm::ivec2 pos)
{
	FastNoise& noise = _noises[Beach];
  float terraceValue = _terraceValue;
  float e = 1.f * (noise.GetNoise(1.f * pos.x, 1.f * pos.y));
  e = (e * 0.5f + 0.5f); // range 0..1;
  float terrace = round(e * terraceValue) / terraceValue;  
  float elevation = terrace;

  return elevation;
}

float MapGeneration::LandGenerationColumn(glm::ivec2 pos)
{
	FastNoise& noise = _noises[GrassLand];
  float terraceValue = _terraceValue;

  float e = 0.5f * (noise.GetNoise(2.f * pos.x, 2.f * pos.y));
  e = (e * 0.5f + 0.5f);
  
  float terrace = round(e * terraceValue) / terraceValue;  
  float elevation = terrace;
  return elevation;
}

float MapGeneration::BasicGenerationColumn(glm::ivec2 pos)
{
  FastNoise& noise = _noises[Basic];
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
  float ret = -1;

  FastNoise& noise = _noises[Biomes];
  glm::ivec2 nextBlock = glm::ivec2(originPos.x + distance_x, originPos.y + distance_y);

  int biome = floorf((noise.GetNoise(nextBlock.x, nextBlock.y) * 0.5 + 0.5) * 10);
  biome = BiomeDefinition(biome);
  if (biome < originBiome)
  {
    switch (biome)
    {
      case GenerationType::GrassLand:
        ret = LandGenerationColumn(nextBlock);
        break;
      case GenerationType::Beach:
        ret = BeachGenerationColumn(nextBlock);
        break;
      case GenerationType::HighLand:
        ret = HighLandGenerationColumn(nextBlock);
        break;
      case GenerationType::Ocean:
        ret = 0.2f;
        break;
      default:
        ret = -1.f;
        break;
    }
    return ret;
  }

  return ret;
}

float MapGeneration::GetApprox(float e0, float e1, float e2, float e3)
{
  float count = 0.f;
  float sum = 0.f;
  float retEvelation;

  e0 != -1.f ? (count++, sum += e0) : 0;
  e1 != -1.f ? (count++, sum += e1) : 0;
  e2 != -1.f ? (count++, sum += e2) : 0;
  e3 != -1.f ? (count++, sum += e3) : 0;
  if (count != 0)
  {
    retEvelation = sum / count;
    return retEvelation;
  }
  return -1.f;
} 

void MapGeneration::SmoothingButtJoint(float& elevation, glm::ivec2 pos, int biome)
{
  float distance = 1.f;
  float rightBlockElevation;
  float leftBlockElevation;
  float topBlockElevation;
  float buttomBlockElevation;

  while (1337)
  {
    rightBlockElevation = CheckingTheBiomeInTheNextColumn(pos, biome, (int)distance, 0);
    leftBlockElevation = CheckingTheBiomeInTheNextColumn(pos, biome, (int)-distance, 0);
    topBlockElevation = CheckingTheBiomeInTheNextColumn(pos, biome, 0, (int)distance);
    buttomBlockElevation = CheckingTheBiomeInTheNextColumn(pos, biome, 0, (int) -distance);
   
    float approx = GetApprox(rightBlockElevation, leftBlockElevation, topBlockElevation, buttomBlockElevation);
    if (approx != -1.f)
    {
      elevation = LERP(approx, elevation, distance * STEP);
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
    case GenerationType::Ocean:
    {
      column.elevation = 1.f;
      column.firstBlockLayer = BlockType::Planks; // it should be water ofc
      column.lastBlockLayer = BlockType::Planks; // it should be water ofc
    }
      break;
    case GenerationType::GrassLand:
    {
      column.elevation = LandGenerationColumn(pos);
      SmoothingButtJoint(column.elevation, pos, column.biom);
      column.elevation = (int)floorf(column.elevation * 10.f);
      column.firstBlockLayer = BlockType::Dirt;
      column.lastBlockLayer = BlockType::Grass;
    }
      break;
    case GenerationType::Beach:
    {
      column.elevation = BeachGenerationColumn(pos);
      SmoothingButtJoint(column.elevation, pos, column.biom);
      column.elevation = (int)floorf(column.elevation * 10.f);
      column.firstBlockLayer = BlockType::Sand;
      column.lastBlockLayer = BlockType::Sand;
    }
      break;
    case GenerationType::HighLand:
    {
      column.elevation = HighLandGenerationColumn(pos);
      SmoothingButtJoint(column.elevation, pos, column.biom);
      column.elevation = (int)floorf(column.elevation + 5.216f);
      column.firstBlockLayer = BlockType::Stone;
      column.lastBlockLayer = BlockType::Stone;
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
      column.elevation = LandGenerationColumn(pos);
      column.elevation = (int)floorf(column.elevation * 10.f);
    }
  }
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
    case GenerationType::Crevices:
    {
      column.biom = 0;
      column.elevation = CrevicesGeneration(pos);
      column.firstBlockLayer = BlockType::Air;
      column.lastBlockLayer = BlockType::Air;
      return column;
    }
    case GenerationType::Tree:
    {
      column.biom = 0;
      column.elevation = TreeGeneration(pos);
      column.firstBlockLayer = BlockType::Leaves;
      column.lastBlockLayer = BlockType::Log;
      return column;
    }
    case GenerationType::ShapeCaves:
    {
      column.biom = 0;
      column.elevation = ShapeCavesGeneration(pos);
      column.firstBlockLayer = BlockType::Air;
      column.lastBlockLayer = BlockType::Air;
      return column;
    }
    case GenerationType::ElevationCaves:
    {
      column.biom = 0;
      column.elevation = ElevationCavesGeneration(pos);
      column.firstBlockLayer = BlockType::Bedrock;
      column.lastBlockLayer = BlockType::Bedrock;
      return column;
    }
    case GenerationType::SecondShapeCaves:
    {
      column.biom = 0;
      column.elevation = SecondShapeCavesGeneration(pos);
      if (column.elevation == -1.f)
        column.firstBlockLayer = BlockType::Air;
      else
        column.firstBlockLayer = BlockType::Dirt;
      column.lastBlockLayer = BlockType::Air;
      return column;
    }
    case GenerationType::SecondElevationCaves:
    {
      column.biom = 0;
      column.elevation = SecondElevationCavesGeneration(pos);
      column.firstBlockLayer = BlockType::Bedrock;
      column.lastBlockLayer = BlockType::Bedrock;
      return column;
    }
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
  _noises[Biomes].SetSeed(1339);
  _noises[Biomes].SetFrequency(0.01);
  _noises[Biomes].SetCellularReturnType(FastNoise::CellValue);
  _noises[Biomes].SetCellularDistanceFunction(FastNoise::Natural);

  _noises[Tree].SetNoiseType(FastNoise::WhiteNoise);
  _noises[Tree].SetSeed(200);
  _noises[Tree].SetFrequency(0.01);

  _noises[ShapeCaves].SetNoiseType(FastNoise::SimplexFractal);
  _noises[ShapeCaves].SetFrequency(0.01);
  _noises[ElevationCaves].SetNoiseType(FastNoise::Perlin);
  _noises[ElevationCaves].SetFrequency(0.01);
  
  _noises[SecondShapeCaves].SetNoiseType(FastNoise::Simplex);
  _noises[SecondShapeCaves].SetFrequency(0.018);
  _noises[SecondElevationCaves].SetNoiseType(FastNoise::Perlin);
  _noises[SecondElevationCaves].SetSeed(1339);
  _noises[SecondElevationCaves].SetFrequency(0.01);

  _noises[Crevices].SetNoiseType(FastNoise::Perlin);
  _noises[Crevices].SetSeed(1333);
  _noises[Crevices].SetFrequency(0.016);

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
  _noiseNames[Tree] = "Tree";
  _noiseNames[ShapeCaves] = "ShapeCaves";
  _noiseNames[SecondShapeCaves] = "SecondShapeCaves";
  _noiseNames[ElevationCaves] = "ElevationleCaves";
  _noiseNames[SecondElevationCaves] = "SecondElevationCaves";
  _noiseNames[Crevices] = "Crevices";
}

FastNoise& MapGeneration::GetNoise(MapGeneration::GenerationType genType) {return _noises[genType];};

float MapGeneration::GetExpValue() {return _exp;};

float MapGeneration::GetTeracceValue() {return _terraceValue;};

void MapGeneration::SetExpValue(float value) {_exp = value;};

void MapGeneration::SetTerraceValue(float value) {_terraceValue = value;};

std::string MapGeneration::GetNoiseName(GenerationType t) {return _noiseNames[t];};