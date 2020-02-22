#include "Generation/MapGeneration.h"

#define STRETCHING_BIOMES 10.421f

MapGeneration::GenerationType GetBiomeType(float e)
{
  if (e < 0.15)
    return MapGeneration::Ocean;
  else if (e < 0.25f)
    return MapGeneration::Swamp;
  else if (e < 0.31f)
    return MapGeneration::Desert;
  else if (e < 0.8f)
    return MapGeneration::GrassLand;
  else if (e < 0.9f)
    return MapGeneration::Snow;
  else if (e > 0.9f)
    return MapGeneration::HighLand;
  return MapGeneration::GrassLand;
}


MapGeneration::GenerationType MapGeneration::GenerationBiomeWithoutRiver(glm::ivec2 pos)
{
  FastNoise& biomesNoise = _noises[Biomes];
  FastNoise& perlinX = _noises[PerlinX];
  FastNoise& perlinY = _noises[PerlinY];

  float pX = perlinX.GetNoise(pos.x, pos.y) * STRETCHING_BIOMES;
  float pY = perlinY.GetNoise(pos.x, pos.y) * STRETCHING_BIOMES;

  float e = biomesNoise.GetNoise(pos.x + pX, pos.y + pY) * 0.5f + 0.5f;
  return GetBiomeType(e);
}

MapGeneration::GenerationType MapGeneration::GenerationBiome(glm::ivec2 pos)
{
  FastNoise& biomesNoise = _noises[Biomes];
  FastNoise& perlinX = _noises[PerlinX];
  FastNoise& perlinY = _noises[PerlinY];

  float pX = perlinX.GetNoise(pos.x, pos.y) * STRETCHING_BIOMES;
  float pY = perlinY.GetNoise(pos.x, pos.y) * STRETCHING_BIOMES;

  float e = biomesNoise.GetNoise(pos.x + pX, pos.y + pY) * 0.5f + 0.5f;
  GenerationType finalBiome = Nothing;
  float riverBiome = 0.f;

  finalBiome = GetBiomeType(e);
  riverBiome = GetRiverElevation(pos);

  if (finalBiome != Ocean && riverBiome != 0.f)
    finalBiome = River;
  return finalBiome;
}
