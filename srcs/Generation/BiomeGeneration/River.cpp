#include "Generation/MapGeneration.h"
#include "World/Block.h"

#define STRETCHIBG_RIVER 43.52124f

float MapGeneration::GetRiverElevation(glm::ivec2 pos)
{
  FastNoise& riverNoise = _noises[River];
  FastNoise& perlinX = _noises[PerlinX];
  FastNoise& perlinY = _noises[PerlinY];

  float pX = perlinX.GetNoise(pos.x, pos.y) * STRETCHIBG_RIVER;
  float pY = perlinY.GetNoise(pos.x, pos.y) * STRETCHIBG_RIVER;

  float e = riverNoise.GetNoise(pos.x + pX, pos.y + pY) * 0.5f + 0.5f;
  if (e > 0.525f)
    return (0.f);
  return e;
}

void MapGeneration::GetRiverData(StoredMapData& column, glm::ivec2 pos)
{
  column.aboveRiverBiome = GenerationBiomeWithoutRiver(pos);
  column.lastBlockLayer = Block::Water;
  column.firstBlockLayer = Block::Water;
  if (column.aboveRiverBiome == GenerationType::Snow)
    column.lastBlockLayer = Block::Ice;
  if (column.aboveRiverBiome == GenerationType::Swamp)
  {
    column.lastBlockLayer = Block::WaterForSwamp;
    column.firstBlockLayer = Block::WaterForSwamp;
  }
  column.approximateElevation = 1.f;
  column.exactElevation = 1;
}
