#include "Generation/MapGeneration.h"
#include "World/Block.h"
#include <iostream>
#include "Utilities/Rand.h"

MapGeneration::GenerationType MapGeneration::BiomeDefinition(float e,  glm::ivec2 pos)
{
  if (e < 0.15)
    return Ocean;
  else if (e < 0.25f)
    return Swamp;
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

MapGeneration::GenerationType MapGeneration::BiomeGenerationWithoutRiver(glm::ivec2 pos)
{
  FastNoise& noise = _noises[Biomes];
  FastNoise& perlinX = _noises[PerlinX];
  FastNoise& perlinY = _noises[PerlinY];

  float pX = perlinX.GetNoise(pos.x, pos.y) * 10.421f;
  float pY = perlinY.GetNoise(pos.x, pos.y) * 10.421f;

  float e = noise.GetNoise(pos.x + pX, pos.y + pY) * 0.5f + 0.5f;
  return BiomeDefinition(e, pos);
}

MapGeneration::GenerationType MapGeneration::BiomeGeneration(glm::ivec2 pos)
{
  FastNoise& noise = _noises[Biomes];
  FastNoise& perlinX = _noises[PerlinX];
  FastNoise& perlinY = _noises[PerlinY];

  float pX = perlinX.GetNoise(pos.x, pos.y) * 10.421f;
  float pY = perlinY.GetNoise(pos.x, pos.y) * 10.421f;

  float e = noise.GetNoise(pos.x + pX, pos.y + pY) * 0.5f + 0.5f;
  GenerationType finalBiome = Nothing;
  float riverBiome = 0.f;

  finalBiome = BiomeDefinition(e, pos);
  riverBiome = RiverElevationGeneration(pos);

  if (finalBiome != Ocean && riverBiome != 0.f)
    finalBiome = River;
  return finalBiome;
}