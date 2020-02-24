#include "Generation/MapGeneration.h"

MapGeneration::GenerationType MapGeneration::BiomeInPositionOfInterest(const glm::ivec2 origPos, const glm::vec2 distance)
{
  FastNoise& noise = _noises[Biomes];
  glm::ivec2 nextBlock = glm::ivec2(origPos.x + distance.x, origPos.y + distance.y);

  GenerationType biome = GenerationBiome(nextBlock);
  return biome;
}

MapGeneration::BiomeInfo MapGeneration::FindBiome(const glm::ivec2 pos, const int maxDistToCheckBiome, const GenerationType biomeOfInterest)
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
