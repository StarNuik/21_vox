#include "Generation/MapGeneration.h"
#include "World/Block.h"

void MapGeneration::GetOceanData(StoredMapData& column, glm::ivec2 pos)
{
  column.approximateElevation = 1.f;
  column.exactElevation = 1;
  column.lastBlockLayer = Block::Water;
  if (FindTheBiomeIntTheNextColumn(pos, Snow, 4).biome == Snow)
    column.lastBlockLayer = Block::Ice;
  column.firstBlockLayer = Block::Water;
}