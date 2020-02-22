#include "Generation/MapGeneration.h"
#include "World/Block.h"

__BLOCK_TYPE MapGeneration::RegenerateDimond(glm::vec3 pos)
{
  FastNoise& noise = _noises[OreDimond];
  float elevation = noise.GetNoise(pos.x, pos.y, pos.z) * 0.5f + 0.5f;
  if (elevation < 0.5f)
    return Block::OreDiamond;
  return Block::Stone;
}

__BLOCK_TYPE MapGeneration::GetOreType(float noiseValue, int currBlockHeight, int maxBlockHeight)
{
  if (noiseValue > 0.545f && noiseValue < 0.558f)
    return Block::OreCoal;
  else if (noiseValue > 0.166f && noiseValue < 0.168f)
    return Block::OreIron;
  else if (noiseValue > 0.8675f && noiseValue < 0.8680f )
    return Block::OreGold;
  else if (noiseValue > 0.9674f && noiseValue < 0.9680f && currBlockHeight <= floorf(maxBlockHeight * 0.3f))
    return Block::OreDiamond;
  else
    return Block::Stone;
}

MapGeneration::StoredOreData MapGeneration::OreGeneration(glm::ivec2 chunkPos, glm::ivec3 columnPos, int maxHeight)
{
  StoredOreData ore;
  FastNoise& noise = _noises[Ore];
  glm::ivec2 globalChunkPos = chunkPos * 16;
	glm::ivec3 pos = glm::ivec3(globalChunkPos.x + columnPos.x, columnPos.y, globalChunkPos.y + columnPos.z);
  float noiseValue = noise.GetNoise(pos.x, pos.y, pos.z) * 0.5f + 0.5f;
  ore.type = GetOreType(noiseValue, pos.y, maxHeight);
  if (ore.type == Block::OreDiamond)
    ore.type = RegenerateDimond(pos);
  return ore;
}