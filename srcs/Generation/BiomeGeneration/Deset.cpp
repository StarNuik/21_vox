#include "Generation/MapGeneration.h"
#include "World/Block.h"

float MapGeneration::GetDesertElevation(glm::ivec2 pos)
{
    FastNoise& desertNoise = _noises[Desert];
    float terraceValue = _terraceValue;
    float e = 1.f * (desertNoise.GetNoise(1.f * pos.x, 1.f * pos.y));
    e = (e * 0.5f + 0.5f);
    float terrace = round(e * terraceValue) / terraceValue;  
    float elevation = terrace;

    return elevation;
}

void MapGeneration::GetDesertData(StoredMapData& column, glm::ivec2 pos)
{
    column.approximateElevation = GetDesertElevation(pos);
    column.approximateElevation = SmoothBiomeSeams(column.approximateElevation, pos, column.biom);
    column.approximateElevation = (int)floorf(column.approximateElevation * 10.f);
    column.firstBlockLayer = Block::Sand;
    column.lastBlockLayer = Block::Sand;
    if (TreeGeneration(pos) != tree.Nothing)
        column.treeType = Trees::Сactus;
}
