#include "Generation/MapGeneration.h"
#include "World/Block.h"
#include <iostream>
#include "Utilities/Rand.h"

#define LERP MapGeneration::Lerp

float MapGeneration::CheckingTheElevationOfBiomeInTheNextColumn(glm::ivec2 originPos, int originBiome, int distance_x, int distance_y) // x,y - coord. z - bioms
{
  float ret = -1;

  FastNoise& noise = _noises[Biomes];
  glm::ivec2 nextBlock = glm::ivec2(originPos.x + distance_x, originPos.y + distance_y); //! Vector maths

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
      case GenerationType::Swamp:
        ret = 0.2f;
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

// float MapGeneration::GetApprox(float e0, float e1, float e2, float e3, float e4, float e5, float e6, float e7)
float MapGeneration::GetApprox(float e0, float e1, float e2, float e3)
{
  float count = 0.f;
  float sum = 0.f;
  float retEvelation;

  e0 != -1.f ? (count++, sum += e0) : 0;
  e1 != -1.f ? (count++, sum += e1) : 0;
  e2 != -1.f ? (count++, sum += e2) : 0;
  e3 != -1.f ? (count++, sum += e3) : 0;
  // e4 != -1.f ? (count++, sum += e4) : 0;
  // e5 != -1.f ? (count++, sum += e5) : 0;
  // e6 != -1.f ? (count++, sum += e6) : 0;
  // e7 != -1.f ? (count++, sum += e7) : 0;
  if (count != 0)
  {
    retEvelation = sum / count;
    return retEvelation;
  }
  return -1.f;
}

//! change return type to float
//! rename to SmoothSeams
//! or SmoothBiomeSeams
void MapGeneration::SmoothingButtJoint(float& elevation, glm::ivec2 pos, int biome)
{
  float distance = 1.f; //! Change to int
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
    // rightTopBlockElevation = CheckingTheElevationOfBiomeInTheNextColumn(pos, biome, (int)diagonalDistance, (int)diagonalDistance);
    leftBlockElevation = CheckingTheElevationOfBiomeInTheNextColumn(pos, biome, (int)-distance, 0);
    // leftTopBlockElevation = CheckingTheElevationOfBiomeInTheNextColumn(pos, biome, (int)-diagonalDistance, (int)diagonalDistance);
    topBlockElevation = CheckingTheElevationOfBiomeInTheNextColumn(pos, biome, 0, (int)distance);
    buttomBlockElevation = CheckingTheElevationOfBiomeInTheNextColumn(pos, biome, 0, (int) -distance);
    // rightButtomBlockElevation = CheckingTheElevationOfBiomeInTheNextColumn(pos, biome, (int)diagonalDistance, (int) -diagonalDistance);
    // leftButtomBlockElevation = CheckingTheElevationOfBiomeInTheNextColumn(pos, biome, (int) -diagonalDistance, (int) -diagonalDistance);
   
    // float approx = GetApprox(rightBlockElevation, rightTopBlockElevation, leftBlockElevation, leftTopBlockElevation, topBlockElevation, buttomBlockElevation, rightButtomBlockElevation, leftButtomBlockElevation);
    float approx = GetApprox(rightBlockElevation, leftBlockElevation, topBlockElevation, buttomBlockElevation);
    if (approx != -1.f)
    {
      elevation = LERP(approx, elevation, distance * STEP); //! 1 cast to float vs 4 casts to int
      return;
    }
    if (distance >= MAX_DIST_TO_SMOOTHING)
      return;
    distance++;
    // diagonalDistance = (diagonalDistance++ > MAX_DIAGONAL_DIST_TO_SMOITHING ? diagonalDistance: diagonalDistance++);
  }
}