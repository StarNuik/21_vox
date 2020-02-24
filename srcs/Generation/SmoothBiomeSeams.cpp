#include "Generation/MapGeneration.h"

#define LERP MapGeneration::Lerp

float MapGeneration::CheckingTheElevationOfBiomeInTheNextColumn(glm::ivec2 originPos,  glm::ivec2 distance, GenerationType originBiome) // x,y - coord. z - bioms
{
  float ret = -1;
  glm::ivec2 nextColumn = originPos + distance;

  GenerationType biome = GenerationBiome(nextColumn);
  if (originBiome == HighLand && biome == River)
    return ret;
  if (biome < originBiome)
  {
    switch (biome)
    {
      case GenerationType::GrassLand:
        ret = GetLandElevation(nextColumn);
        ret = SmoothBiomeSeams(ret, nextColumn, biome);
        break;
      case GenerationType::Desert:
        ret = GetDesertElevation(nextColumn);
        ret = SmoothBiomeSeams(ret, nextColumn, biome);
        break;
      case GenerationType::Snow:
        ret = GetSnowLandElevation(nextColumn);
        ret = SmoothBiomeSeams(ret, nextColumn, biome);
        break;
      case GenerationType::HighLand:
        ret = GetHighLandElevation(nextColumn);
        ret = SmoothBiomeSeams(ret, nextColumn, biome);
        break;
      case GenerationType::ChineseMountains:
        ret = GetChineseMountainsElevation(nextColumn);
        ret = SmoothBiomeSeams(ret, nextColumn, biome);
      case GenerationType::Swamp:
        ret = WATER_ELEVATION;
        break;
      case GenerationType::Ocean:
        ret = WATER_ELEVATION;
        break;
      case GenerationType::River:
        ret = WATER_ELEVATION;
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
float MapGeneration::SmoothBiomeSeams(float elevation, glm::ivec2 pos, GenerationType biome)
{
  int distance = 1; //! Change to int
  // float diagonalDistance = 1.f;
  float rightBlockElevation;
  // float rightTopBlockElevation;
  float leftBlockElevation;
  // float leftTopBlockElevation;
  float topBlockElevation;
  float buttomBlockElevation;
  // float rightButtomBlockElevation;
  // float leftButtomBlockElevation;

  while (1337)
  {
    rightBlockElevation = CheckingTheElevationOfBiomeInTheNextColumn(pos, glm::ivec2(distance, 0), biome);
    // rightTopBlockElevation = CheckingTheElevationOfBiomeInTheNextColumn(pos, biome, diagonalDistance, diagonalDistance);
    leftBlockElevation = CheckingTheElevationOfBiomeInTheNextColumn(pos, glm::ivec2(-distance, 0), biome);
    // leftTopBlockElevation = CheckingTheElevationOfBiomeInTheNextColumn(pos, biome, -diagonalDistance, diagonalDistance);
    topBlockElevation = CheckingTheElevationOfBiomeInTheNextColumn(pos, glm::ivec2(0, distance), biome);
    buttomBlockElevation = CheckingTheElevationOfBiomeInTheNextColumn(pos, glm::ivec2(0,  -distance), biome);
    // rightButtomBlockElevation = CheckingTheElevationOfBiomeInTheNextColumn(pos, biome, diagonalDistance,  -diagonalDistance);
    // leftButtomBlockElevation = CheckingTheElevationOfBiomeInTheNextColumn(pos, biome,  -diagonalDistance,  -diagonalDistance);
   
    // float approx = GetApprox(rightBlockElevation, rightTopBlockElevation, leftBlockElevation, leftTopBlockElevation, topBlockElevation, buttomBlockElevation, rightButtomBlockElevation, leftButtomBlockElevation);
    float approx = GetApprox(rightBlockElevation, leftBlockElevation, topBlockElevation, buttomBlockElevation);
    if (approx != -1.f)
    {
      elevation = LERP(approx, elevation, (float)distance * STEP);
      return elevation;
    }
    if (distance >= MAX_DIST_TO_SMOOTHING)
      return elevation;
    distance++;
    // diagonalDistance = (diagonalDistance++ > MAX_DIAGONAL_DIST_TO_SMOITHING ? diagonalDistance: diagonalDistance++);
  }
}