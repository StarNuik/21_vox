#include "Generation/MapGeneration.h"
#include "World/Block.h"

float  MapGeneration::GetChineseMountainsElevation(glm::ivec2 pos)
{
    FastNoise& chineseMountainsNoise = _noises[ChineseMountains];
    float exp = 2.9f;
    float terraceValue = _terraceValue;

    float e = 1.f * (chineseMountainsNoise.GetNoise(pos.x, pos.y));
    float e1 = 0.50f * (chineseMountainsNoise.GetNoise(2.f * pos.x, 2.f * pos.y));
    float e2 = 0.25f * (chineseMountainsNoise.GetNoise(4.f * pos.x, 4.f * pos.y));
    e += e1 + e2;
    e = (e * 0.5f + 0.5f);

    float mountainPeaks = pow(e, exp);
    float terrace = round(mountainPeaks * terraceValue) / terraceValue;
    float elevation = terrace;
    return elevation;
}

void MapGeneration::GetChineseMountainsData(StoredMapData& column, glm::ivec2 pos)
{
    column.approximateElevation = GetChineseMountainsElevation(pos);
    column.approximateElevation = SmoothBiomeSeams(column.approximateElevation, pos, HighLand);
    column.approximateElevation = (int)floorf(column.approximateElevation * 10.f);
    column.firstBlockLayer = Block::Stone;
    column.lastBlockLayer = Block::Grass;
}
