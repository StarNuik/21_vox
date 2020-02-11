#include "Generation/MapGeneration.h"
#include "World/Block.h"
#include <iostream>
#include "Utilities/Rand.h"
// #include "Utill"
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

float MapGeneration::RiverElevationGeneration(glm::ivec2 pos)
{
  FastNoise& noise = _noises[River];
  FastNoise& perlinX = _noises[PerlinX];
  FastNoise& perlinY = _noises[PerlinY];

  float pX = perlinX.GetNoise(pos.x, pos.y) * 43.52124f;
  float pY = perlinY.GetNoise(pos.x, pos.y) * 43.52124f;


  float e = noise.GetNoise(pos.x + pX, pos.y + pY) * 0.5f + 0.5f;
  if (e > 0.525f)
    return (0.f);
  return e;
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
  return tree.Nothing;
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

float MapGeneration::SnowLangGenerationColumn(glm::ivec2 pos)
{
  FastNoise& noise = _noises[Snow];
  float terraceValue = _terraceValue;

  float e = 1.f * (noise.GetNoise(pos.x, pos.y));
  float e1 = 0.50f * (noise.GetNoise(2.f * pos.x, 2.f * pos.y));
  float e2 = 0.25f * (noise.GetNoise(4.f * pos.x, 4.f * pos.y));
  float e3 = 0.13f * (noise.GetNoise(8.f * pos.x, 8.f * pos.y));

  e += e1 + e2 + e3;
  
  float elevation = round(e * terraceValue) / terraceValue;
  return elevation;
}

float MapGeneration::DesertGenerationColumn(glm::ivec2 pos)
{
	FastNoise& noise = _noises[Desert];
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
  
  float elevation = round(e * terraceValue) / terraceValue;
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

int MapGeneration::BiomeInPositionOfInterest(const glm::ivec2 origPos, const glm::vec2 distance)
{
  FastNoise& noise = _noises[Biomes];
  glm::ivec2 nextBlock = glm::ivec2(origPos.x + distance.x, origPos.y + distance.y);

  int biome = BiomeGeneration(nextBlock);
  return biome;
}

MapGeneration::BiomeInfo MapGeneration::CheckingTheBiomeIntTheNextColumn(const glm::ivec2 pos, const int biome, const int maxDistToCheckBiome)
{
  int distance = 1;
  int rightBlockBiome;
  int leftBlockBiome;
  int topBlockBiome;
  int buttomBlockBiome;

  if (maxDistToCheckBiome == 0)
    return (BiomeInfo{biome, 0});

  while (distance < maxDistToCheckBiome)
  {
    rightBlockBiome = BiomeInPositionOfInterest(pos, glm::ivec2(distance, 0));
    if (rightBlockBiome != biome)
      return (BiomeInfo{rightBlockBiome, distance});

    leftBlockBiome = BiomeInPositionOfInterest(pos, glm::ivec2(-distance, 0));
    if (leftBlockBiome != biome)
      return (BiomeInfo{leftBlockBiome, distance});

    topBlockBiome = BiomeInPositionOfInterest(pos, glm::ivec2(0, distance));
    if (topBlockBiome != biome)
      return (BiomeInfo{topBlockBiome, distance});

    buttomBlockBiome = BiomeInPositionOfInterest(pos, glm::ivec2(0, -distance));
    if (buttomBlockBiome != biome)
      return (BiomeInfo{buttomBlockBiome, distance});

    distance++;
  }
  return BiomeInfo{biome, 0};
}

MapGeneration::BiomeInfo MapGeneration::FindTheBiomeIntTheNextColumn(const glm::ivec2 pos, const int biomeOfInterest, const int maxDistToCheckBiome)
{
  int distance = 1;
  int rightBlockBiome;
  int leftBlockBiome;
  int topBlockBiome;
  int buttomBlockBiome;

  if (maxDistToCheckBiome < distance)
    return (BiomeInfo{Nothing, 0});

  while (distance < maxDistToCheckBiome)
  {
    rightBlockBiome = BiomeInPositionOfInterest(pos, glm::ivec2(distance, 0));
    if (rightBlockBiome == biomeOfInterest)
      return (BiomeInfo{rightBlockBiome, distance});

    leftBlockBiome = BiomeInPositionOfInterest(pos, glm::ivec2(-distance, 0));
    if (leftBlockBiome == biomeOfInterest)
      return (BiomeInfo{leftBlockBiome, distance});

    topBlockBiome = BiomeInPositionOfInterest(pos, glm::ivec2(0, distance));
    if (topBlockBiome == biomeOfInterest)
      return (BiomeInfo{topBlockBiome, distance});

    buttomBlockBiome = BiomeInPositionOfInterest(pos, glm::ivec2(0, -distance));
    if (buttomBlockBiome == biomeOfInterest)
      return (BiomeInfo{buttomBlockBiome, distance});

    distance++;
  }
  return BiomeInfo{Nothing, 0};
}

float MapGeneration::CheckingTheElevationOfBiomeInTheNextColumn(glm::ivec2 originPos, int originBiome, int distance_x, int distance_y) // x,y - coord. z - bioms
{
  float ret = -1;

  FastNoise& noise = _noises[Biomes];
  glm::ivec2 nextBlock = glm::ivec2(originPos.x + distance_x, originPos.y + distance_y);


  int biome = BiomeGeneration(nextBlock);
  if (originBiome == HighLand && biome == River)
    return ret;
  if (biome < originBiome)
  {
    switch (biome)
    {
      case GenerationType::GrassLand:
        ret = LandGenerationColumn(nextBlock);
        SmoothingButtJoint(ret, nextBlock, biome);
        break;
      case GenerationType::Desert:
        ret = DesertGenerationColumn(nextBlock);
        SmoothingButtJoint(ret, nextBlock, biome);
        break;
      case GenerationType::Snow:
        ret = SnowLangGenerationColumn(nextBlock);
        SmoothingButtJoint(ret, nextBlock, biome);
        break;
      case GenerationType::HighLand:
        ret = HighLandGenerationColumn(nextBlock) + 5.216f;
        SmoothingButtJoint(ret, nextBlock, biome);
        break;
      case GenerationType::Ocean:
        ret = 0.2f;
        break;
      case GenerationType::River:
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

float MapGeneration::CheckingTheElevationOfBiomeInTheNextColumnWithoutRiver(glm::ivec2 originPos, int originBiome, int distance_x, int distance_y) // x,y - coord. z - bioms
{
  float ret = -1;

  FastNoise& noise = _noises[Biomes];
  glm::ivec2 nextBlock = glm::ivec2(originPos.x + distance_x, originPos.y + distance_y);


  int biome = BiomeGenerationWithoutRiver(nextBlock);
  if (biome < originBiome)
  {
    switch (biome)
    {
      case GenerationType::GrassLand:
        ret = LandGenerationColumn(nextBlock);
        SmoothingButtJoint(ret, nextBlock, biome);
        break;
      case GenerationType::Beach:
        ret = DesertGenerationColumn(nextBlock);
        SmoothingButtJoint(ret, nextBlock, biome);
        break;
      case GenerationType::Snow:
        ret = SnowLangGenerationColumn(nextBlock);
        SmoothingButtJoint(ret, nextBlock, biome);
        break;
      case GenerationType::HighLand:
        ret = HighLandGenerationColumn(nextBlock) + 5.216f;
        SmoothingButtJoint(ret, nextBlock, biome);
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

float MapGeneration::GetApprox(float e0, float e1, float e2, float e3, float e4, float e5, float e6, float e7)
{
  float count = 0.f;
  float sum = 0.f;
  float retEvelation;

  e0 != -1.f ? (count++, sum += e0) : 0;
  e1 != -1.f ? (count++, sum += e1) : 0;
  e2 != -1.f ? (count++, sum += e2) : 0;
  e3 != -1.f ? (count++, sum += e3) : 0;
  e4 != -1.f ? (count++, sum += e4) : 0;
  e5 != -1.f ? (count++, sum += e5) : 0;
  e6 != -1.f ? (count++, sum += e6) : 0;
  e7 != -1.f ? (count++, sum += e7) : 0;
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
  float diagonalDistance = 1.f;
  float rightBlockElevation;
  float rightTopBlockElevation;
  float leftBlockElevation;
  float leftTopBlockElevation;
  float topBlockElevation;
  float buttomBlockElevation;
  float rightButtomBlockElevation;
  float leftButtomBlockElevation;

  while (1337)
  {
    rightBlockElevation = CheckingTheElevationOfBiomeInTheNextColumn(pos, biome, (int)distance, 0);
    rightTopBlockElevation = CheckingTheElevationOfBiomeInTheNextColumn(pos, biome, (int)diagonalDistance, (int)diagonalDistance);
    leftBlockElevation = CheckingTheElevationOfBiomeInTheNextColumn(pos, biome, (int)-distance, 0);
    leftTopBlockElevation = CheckingTheElevationOfBiomeInTheNextColumn(pos, biome, (int)-diagonalDistance, (int)diagonalDistance);
    topBlockElevation = CheckingTheElevationOfBiomeInTheNextColumn(pos, biome, 0, (int)distance);
    buttomBlockElevation = CheckingTheElevationOfBiomeInTheNextColumn(pos, biome, 0, (int) -distance);
    rightButtomBlockElevation = CheckingTheElevationOfBiomeInTheNextColumn(pos, biome, (int)diagonalDistance, (int) -diagonalDistance);
    leftButtomBlockElevation = CheckingTheElevationOfBiomeInTheNextColumn(pos, biome, (int) -diagonalDistance, (int) -diagonalDistance);
   
    // float approx = GetApprox(rightBlockElevation, leftBlockElevation, topBlockElevation, buttomBlockElevation);
    float approx = GetApprox(rightBlockElevation, rightTopBlockElevation, leftBlockElevation, leftBlockElevation, topBlockElevation, buttomBlockElevation, rightButtomBlockElevation, leftButtomBlockElevation);
    if (approx != -1.f)
    {
      elevation = LERP(approx, elevation, distance * STEP);
      return;
    }
    if (distance >= MAX_DIST_TO_SMOOTHING)
      return;
    distance++;
    diagonalDistance = (diagonalDistance++ > MAX_DIAGONAL_DIST_TO_SMOITHING ? diagonalDistance: diagonalDistance++);
  }
}

void MapGeneration::SmoothingButtJointWithoutRiver(float& elevation, glm::ivec2 pos, int biome)
{
  float distance = 1.f;
  float diagonalDistance = 1.f;
  float rightBlockElevation;
  float rightTopBlockElevation;
  float leftBlockElevation;
  float leftTopBlockElevation;
  float topBlockElevation;
  float buttomBlockElevation;
  float rightButtomBlockElevation;
  float leftButtomBlockElevation;

  while (1337)
  {
    rightBlockElevation = CheckingTheElevationOfBiomeInTheNextColumnWithoutRiver(pos, biome, (int)distance, 0);
    rightTopBlockElevation = CheckingTheElevationOfBiomeInTheNextColumnWithoutRiver(pos, biome, (int)diagonalDistance, (int)diagonalDistance);
    leftBlockElevation = CheckingTheElevationOfBiomeInTheNextColumnWithoutRiver(pos, biome, (int)-distance, 0);
    leftTopBlockElevation = CheckingTheElevationOfBiomeInTheNextColumnWithoutRiver(pos, biome, (int)-diagonalDistance, (int)diagonalDistance);
    topBlockElevation = CheckingTheElevationOfBiomeInTheNextColumnWithoutRiver(pos, biome, 0, (int)distance);
    buttomBlockElevation = CheckingTheElevationOfBiomeInTheNextColumnWithoutRiver(pos, biome, 0, (int) -distance);
    rightButtomBlockElevation = CheckingTheElevationOfBiomeInTheNextColumnWithoutRiver(pos, biome, (int)diagonalDistance, (int) -diagonalDistance);
    leftButtomBlockElevation = CheckingTheElevationOfBiomeInTheNextColumnWithoutRiver(pos, biome, (int) -diagonalDistance, (int) -diagonalDistance);
   
    // float approx = GetApprox(rightBlockElevation, leftBlockElevation, topBlockElevation, buttomBlockElevation);
    float approx = GetApprox(rightBlockElevation, rightTopBlockElevation, leftBlockElevation, leftBlockElevation, topBlockElevation, buttomBlockElevation, rightButtomBlockElevation, leftButtomBlockElevation);
    if (approx != -1.f)
    {
      elevation = LERP(approx, elevation, distance * STEP);
      return;
    }
    if (distance >= MAX_DIST_TO_SMOOTHING)
      return;
    distance++;
    diagonalDistance = (diagonalDistance++ > MAX_DIAGONAL_DIST_TO_SMOITHING ? diagonalDistance: diagonalDistance++);
  }
}

int MapGeneration::BiomeDefinition(int elevation, glm::ivec2 pos)
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
  else if (elevation == 8)
    return Snow;
  else if (elevation == 9)
    return HighLand;
  else
    return GrassLand;
}

int MapGeneration::TestBiomeDefinition(float e,  glm::ivec2 pos)
{
  if (e < 0.2)
    return Ocean;
  else if (e < 0.31f)
    return Desert;
  else if (e < 0.8)
    return GrassLand;
  else if (e < 0.9)
    return Snow;
  else if (e > 0.9f)
    return HighLand;
  return GrassLand;
}

int MapGeneration::BiomeGenerationWithoutRiver(glm::ivec2 pos)
{
  FastNoise& noise = _noises[Biomes];
  FastNoise& perlinX = _noises[PerlinX];
  FastNoise& perlinY = _noises[PerlinY];

  float pX = perlinX.GetNoise(pos.x, pos.y) * 10.421f;
  float pY = perlinY.GetNoise(pos.x, pos.y) * 10.421f;

  float e = noise.GetNoise(pos.x + pX, pos.y + pY) * 0.5f + 0.5f;
  return TestBiomeDefinition(e, pos);
}

int MapGeneration::BiomeGeneration(glm::ivec2 pos)
{
  FastNoise& noise = _noises[Biomes];
  FastNoise& perlinX = _noises[PerlinX];
  FastNoise& perlinY = _noises[PerlinY];

  float pX = perlinX.GetNoise(pos.x, pos.y) * 10.421f;
  float pY = perlinY.GetNoise(pos.x, pos.y) * 10.421f;

  float e = noise.GetNoise(pos.x + pX, pos.y + pY) * 0.5f + 0.5f;
  int finalBiome = 0;
  float riverBiome = 0.f;
  finalBiome = TestBiomeDefinition(e, pos);
  riverBiome = RiverElevationGeneration(pos);

  if (finalBiome != Ocean && riverBiome != 0.f)
    finalBiome = River;
  return finalBiome;
}

__BLOCK_TYPE MapGeneration::RegenerateDimond(glm::vec3 pos)
{
  FastNoise& noise = _noises[OreDimond];
  float elevation = noise.GetNoise(pos.x, pos.y, pos.z) * 0.5f + 0.5f;
  if (elevation < 0.5f)
    return Block::OreDiamond;
  return Block::Air;
}

__BLOCK_TYPE MapGeneration::OreDefinition(float elevation, int currBlockHeight, int maxBlockHeight)
{
  if (elevation > 0.545f && elevation < 0.558f)
    return Block::OreCoal;
  else if (elevation > 0.166f && elevation < 0.168f)
    return Block::OreIron;
  else if (elevation > 0.8675f && elevation < 0.8680f )
    return Block::OreGold;
  else if (elevation > 0.9674f && elevation < 0.9680f && currBlockHeight <= floorf(maxBlockHeight * 0.3f))
    return Block::OreDiamond;
  else
    return Block::Air;
}

MapGeneration::StoredOreData MapGeneration::OreGeneration(glm::ivec2 globalPos, glm::ivec3 blockPosition, int maxHeight)
{
  StoredOreData ore;
  FastNoise& noise = _noises[Ore];
  float globalX = globalPos.x * 16, globalZ = globalPos.y * 16;
  glm::ivec3 pos = glm::ivec3(blockPosition.x + globalX, blockPosition.y, blockPosition.z + globalZ);
  ore.type = Block::Cobblestone;
  float elevation = noise.GetNoise(pos.x, pos.y, pos.z) * 0.5f + 0.5f;
  ore.type = OreDefinition(elevation, pos.y, maxHeight);
  if (ore.type == Block::OreDiamond)
    ore.type = RegenerateDimond(pos);
  return ore;
}

float MapGeneration::CrevicesGenerations(glm::ivec2 globalPos, glm::ivec3 blockPosition)
{
  FastNoise& noise = _noises[Crevices];

  float globalX = globalPos.x * 16, globalZ = globalPos.y * 16;
  glm::ivec3 pos = glm::ivec3(blockPosition.x + globalX, blockPosition.y, blockPosition.z + globalZ);

  float terraceValue = _terraceValue;
  float e = noise.GetNoise(pos.x * 0.55f, pos.y, pos.z * 3.5f);
  e = (e * 0.5f + 0.5f);
  if (e < 0.815f)
    return -1.f;

  float terrace = round(e * terraceValue) / terraceValue;
  return terrace;
}

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
    {
      column.approximateElevation = 1.f;
      column.exactElevation = 1;
      column.lastBlockLayer = Block::Water;
      if (FindTheBiomeIntTheNextColumn(pos, Snow, 4).biome == Snow)
        column.lastBlockLayer = Block::Ice;
      column.firstBlockLayer = Block::Water;
    }
      break;
    case GenerationType::River:
    {
      column.aboveRiverBiome = BiomeGenerationWithoutRiver(pos);
      column.lastBlockLayer = Block::Water;
      if (column.aboveRiverBiome == Snow)
        column.lastBlockLayer = Block::Ice;
      column.firstBlockLayer = Block::Water;
      column.approximateElevation = 1.f;
      column.exactElevation = 1;

    }
      break;
    case GenerationType::GrassLand:
    {
      FastNoise& noise = _noises[BeachBordered];
      float beachLength = (noise.GetNoise(pos.x, pos.y) * 0.5f + 0.5f); 
      int checkLength;
      if (beachLength > 0.5f)
        checkLength = (int)beachLength + beachLength * 10;
      else
        checkLength = 0; 
      column.approximateElevation = LandGenerationColumn(pos);
      SmoothingButtJoint(column.approximateElevation, pos, column.biom);
      column.approximateElevation = (int)floorf(column.approximateElevation * 10.f);
      column.firstBlockLayer = Block::Dirt;
      column.lastBlockLayer = Block::Grass;
      if (CheckingTheBiomeIntTheNextColumn(pos, column.biom, checkLength).biome == Ocean)
      {
        column.biom = Beach;
        column.firstBlockLayer = Block::Sand;
        column.lastBlockLayer = Block::Sand;
      }
      else if (TreeGeneration(pos) != tree.Nothing)
        column.treeType = intRand(Trees::First, Trees::OakTreeTypeTwo);
    }
      break;
    case GenerationType::Desert:
    {
      column.approximateElevation = DesertGenerationColumn(pos);
      SmoothingButtJoint(column.approximateElevation, pos, column.biom);
      column.approximateElevation = (int)floorf(column.approximateElevation * 10.f);
      column.firstBlockLayer = Block::Sand;
      column.lastBlockLayer = Block::Sand;
      if (TreeGeneration(pos) != tree.Nothing)
          column.treeType = Trees::Сactus;
    }
      break;
    case GenerationType::HighLand:
    {
      column.approximateElevation = HighLandGenerationColumn(pos);
      column.approximateElevation += 5.216f;
      SmoothingButtJoint(column.approximateElevation, pos, column.biom);
      column.approximateElevation = (int)floorf(column.approximateElevation + 5.216f);
      column.firstBlockLayer = Block::Stone;
      column.lastBlockLayer = Block::Stone;
      if (column.approximateElevation > 75 && column.approximateElevation < 86)
        column.lastBlockLayer = Block::SnowGrass;
      else if (column.approximateElevation >= 86)
        column.lastBlockLayer = Block::Ice;
    }
      break;
    case GenerationType::Snow:
    {
      column.approximateElevation = SnowLangGenerationColumn(pos);
      SmoothingButtJoint(column.approximateElevation, pos, column.biom);
      column.approximateElevation = (int)floorf(column.approximateElevation * 10.f);
      column.firstBlockLayer = Block::Dirt;
      column.lastBlockLayer = Block::SnowGrass;
      if (TreeGeneration(pos) != tree.Nothing)
        column.treeType = intRand(Trees::OakTreeTypeTwo, Trees::SpruceTreeTypeTwo + 1);
    }
      break;
    default:
    {
      column.firstBlockLayer = Block::Dirt;
      column.lastBlockLayer = Block::Grass;
      column.approximateElevation = LandGenerationColumn(pos);
      column.approximateElevation = (int)floorf(column.approximateElevation * 10.f);
    }
      break;
  }
  column.exactElevation = glm::clamp((int)column.approximateElevation, 0, 255);
  return column;
}

MapGeneration::StoredMapData MapGeneration::Generation(glm::ivec2 globalPos, glm::ivec2 blockPosition, MapGeneration::GenerationType genType)
{
  StoredMapData column;

  float globalX = globalPos.x * 16, globalY = globalPos.y * 16;
  glm::vec2 pos = glm::ivec2(globalX + blockPosition.x, globalY + blockPosition.y);
  column.firstBlockLayer = Block::Air;
  column.lastBlockLayer = Block::Air;

  switch (genType)
  {
    case GenerationType::Basic:
    {
      column.approximateElevation = BasicGenerationColumn(pos);
      column.firstBlockLayer = Block::Stone;
      column.lastBlockLayer = Block::Stone;
    }
      break;
    case GenerationType::HighLand:
    {
      column.approximateElevation = HighLandGenerationColumn(pos);
      column.approximateElevation += 5.216f;
      // SmoothingButtJointWithoutRiver(column.approximateElevation, pos, HighLand);
      SmoothingButtJoint(column.approximateElevation, pos, HighLand);
      column.approximateElevation = (int)floorf(column.approximateElevation + 5.216f);
      // column.approximateElevation = (int)floorf(column.approximateElevation);
      column.firstBlockLayer = Block::Stone;
      column.lastBlockLayer = Block::Stone;
      if (column.approximateElevation > 75 && column.approximateElevation < 86)
        column.lastBlockLayer = Block::SnowGrass;
      else if (column.approximateElevation >= 86)
        column.lastBlockLayer = Block::Ice;
    }
      break;
    case GenerationType::Crevices:
    {
      column.approximateElevation = CrevicesGeneration(pos);
      column.firstBlockLayer = Block::Air;
      column.lastBlockLayer = Block::Air;
    }
      break;
    case GenerationType::Tree:
    {
      column.approximateElevation = TreeGeneration(pos);
      column.firstBlockLayer = Block::Leaves;
      column.lastBlockLayer = Block::Log;
    }
      break;
    case GenerationType::ShapeCaves:
    {
      column.approximateElevation = ShapeCavesGeneration(pos);
      column.firstBlockLayer = Block::Air;
      column.lastBlockLayer = Block::Air;
    }
      break;
    case GenerationType::ElevationCaves:
    {
      column.approximateElevation = ElevationCavesGeneration(pos);
      column.firstBlockLayer = Block::Air;
      column.lastBlockLayer = Block::Air;
    }
      break;
    case GenerationType::SecondShapeCaves:
    {
      column.approximateElevation = SecondShapeCavesGeneration(pos);
      column.firstBlockLayer = Block::Air;
      column.lastBlockLayer = Block::Air;
    }
      break;
    case GenerationType::SecondElevationCaves:
    {
      column.approximateElevation = SecondElevationCavesGeneration(pos);
      column.firstBlockLayer = Block::Bedrock;
      column.lastBlockLayer = Block::Bedrock;
    }
      break;
    case GenerationType::River:
    {
      column.approximateElevation = RiverElevationGeneration(pos);
      if (column.approximateElevation != 0.f)
      {
        column.firstBlockLayer = Block::Water;
        column.lastBlockLayer = Block::Water;
      }
    }
      break;
    case GenerationType::Ore:
    {
      column.approximateElevation = LandGenerationColumn(pos);
      column.approximateElevation = (int)floorf(column.approximateElevation * 10.f);
      column.firstBlockLayer = Block::Grass;
      column.lastBlockLayer = Block::Grass;
    }
      break;
    default:
    {
      column.approximateElevation = BasicGenerationColumn(pos);
      column.firstBlockLayer = Block::Stone;
      column.lastBlockLayer = Block::Stone;
    }
      break;
  }
  column.biom = 0;
  column.treeType = tree.Nothing;
  column.exactElevation = glm::clamp((int)column.approximateElevation, 0, 255);
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
  _noises[GrassLand].SetFrequency(0.020214);

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
  _noiseNames[Desert] = "Desert";
  _noiseNames[Snow] = "Snow";
  _noiseNames[HighLand] = "HighLand";
  _noiseNames[Biomes] = "BiomeDefinition";
  _noiseNames[BeachBordered] = "BeachBordered";
  _noiseNames[River] = "River";
  _noiseNames[Tree] = "Tree";
  _noiseNames[ShapeCaves] = "ShapeCaves";
  _noiseNames[SecondShapeCaves] = "SecondShapeCaves";
  _noiseNames[ElevationCaves] = "ElevationleCaves";
  _noiseNames[SecondElevationCaves] = "SecondElevationCaves";
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