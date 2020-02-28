#include "Generation/MapGeneration.h"
#include "World/Block.h"

float MapGeneration::GetGladeElevation(glm::ivec2 pos)
{
	FastNoise& gladeNoise = _noises[Glade];

    float e = 0.25f * (gladeNoise.GetNoise(4.f * pos.x, 4.f * pos.y));
    return e;
}

void MapGeneration::GetGladeData(StoredMapData& column, glm::ivec2 pos)
{
    column.approximateElevation = GetGladeElevation(pos);
    column.approximateElevation = SmoothBiomeSeams(column.approximateElevation, pos, column.biom);
    column.approximateElevation = (int)floorf(column.approximateElevation * 10.f);
    column.firstBlockLayer = Block::Dirt;
    column.lastBlockLayer = Block::Grass;
}
