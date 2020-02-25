#include "Generation/MapGeneration.h"
#include "World/Block.h"

#define MAX_DISTANCE_TO_CHECK_BIOME 4

void MapGeneration::GetOceanData(StoredMapData& column, glm::ivec2 pos)
{
  column.approximateElevation = 1.f;
  column.exactElevation = 1;
  column.lastBlockLayer = Block::Water;
  column.firstBlockLayer = Block::Water;
  if (FindBiome(pos, MAX_DISTANCE_TO_CHECK_BIOME, MapGeneration::Snow).biome == GenerationType::Snow)
    column.lastBlockLayer = Block::Ice;
  if (FindBiome(pos, MAX_DIST_TO_CHECK_BIOME, MapGeneration::Swamp).biome == GenerationType::Swamp)
    column.lastBlockLayer = Block::WaterForSwamp;

}
