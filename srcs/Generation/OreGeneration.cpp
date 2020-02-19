#include "Generation/MapGeneration.h"
#include "World/Block.h"
#include <iostream>
#include "Utilities/Rand.h"
// #include "Utill"

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