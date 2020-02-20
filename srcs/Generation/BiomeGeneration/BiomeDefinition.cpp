#include "Generation/MapGeneration.h"
#include "World/Block.h"
#include <iostream>
#include "Utilities/Rand.h"

//! Rename to GetBiome / GetBiomeType
MapGeneration::GenerationType BiomeDefinition(float e,  glm::ivec2 pos) //! remove pos
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

//! GetBiomeWithoutRiver
MapGeneration::GenerationType MapGeneration::BiomeGenerationWithoutRiver(glm::ivec2 pos)
{
  FastNoise& noise = _noises[Biomes]; //! Maybe call this biomes / biomeNoise / smth else
  FastNoise& perlinX = _noises[PerlinX];
  FastNoise& perlinY = _noises[PerlinY];

  float pX = perlinX.GetNoise(pos.x, pos.y) * 10.421f;
  float pY = perlinY.GetNoise(pos.x, pos.y) * 10.421f;

  float e = noise.GetNoise(pos.x + pX, pos.y + pY) * 0.5f + 0.5f;
  return BiomeDefinition(e, pos);
}

//! rename to GenerateBiome / GetBiome
MapGeneration::GenerationType MapGeneration::BiomeGeneration(glm::ivec2 pos)
{ //! We're gonna get a Linter then motherf*****
  FastNoise& noise = _noises[Biomes]; //!
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