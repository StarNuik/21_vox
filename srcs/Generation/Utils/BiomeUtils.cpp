#include "Generation/MapGeneration.h"
#include "World/Block.h"
#include <iostream>
#include "Utilities/Rand.h"

MapGeneration::GenerationType MapGeneration::BiomeInPositionOfInterest(const glm::ivec2 origPos, const glm::vec2 distance)
{
  FastNoise& noise = _noises[Biomes];
  glm::ivec2 nextBlock = glm::ivec2(origPos.x + distance.x, origPos.y + distance.y);

  GenerationType biome = BiomeGeneration(nextBlock);
  return biome;
}

MapGeneration::BiomeInfo MapGeneration::CheckingTheBiomeIntTheNextColumn(const glm::ivec2 pos, const GenerationType biome, const int maxDistToCheckBiome)
{
  int distance = 1;
  GenerationType rightBlockBiome;
  GenerationType leftBlockBiome;
  GenerationType topBlockBiome;
  GenerationType buttomBlockBiome;

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

MapGeneration::BiomeInfo MapGeneration::FindTheBiomeIntTheNextColumn(const glm::ivec2 pos, const GenerationType biomeOfInterest, const int maxDistToCheckBiome)
{
  int distance = 1;
  GenerationType rightBlockBiome;
  GenerationType leftBlockBiome;
  GenerationType topBlockBiome;
  GenerationType buttomBlockBiome;

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
