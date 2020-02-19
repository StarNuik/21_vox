#include "Generation/MapGeneration.h"
#include "World/Block.h"

float  MapGeneration::HighLandGenerationColumn(glm::ivec2 pos)
{
    FastNoise& noise = _noises[HighLand];
    float exp = _exp;
    float terraceValue = _terraceValue;

    float e = 1.f * (noise.GetNoise(pos.x, pos.y));
    float e1 = 0.50f * (noise.GetNoise(2.f * pos.x, 2.f * pos.y));
    float e2 = 0.25f * (noise.GetNoise(4.f * pos.x, 4.f * pos.y));
    e += e1 + e2;
    e = (e * 0.5f + 0.5f) * 10; // range 0..~10.3

    float mountainPeaks = pow(e, exp);
    float terrace = round(mountainPeaks * terraceValue) / terraceValue;

    float elevation = terrace;
    return elevation;
}

void MapGeneration::GetHighLandData(StoredMapData& column, glm::ivec2 pos)
{
    column.approximateElevation = HighLandGenerationColumn(pos);
    column.approximateElevation += 5.216f;
    SmoothingButtJoint(column.approximateElevation, pos, HighLand);
    column.approximateElevation = (int)floorf(column.approximateElevation + 5.216f);
    column.firstBlockLayer = Block::Stone;
    column.lastBlockLayer = Block::Stone;
    if (column.approximateElevation > 75 && column.approximateElevation < 86)
        column.lastBlockLayer = Block::SnowGrass;
    else if (column.approximateElevation >= 86)
        column.lastBlockLayer = Block::Ice;
}