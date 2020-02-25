#include "Generation/MapGeneration.h"
#include "World/Block.h"
#include "Utilities/Rand.h"

#define STRETCHING_SWAMP 21.6543f

float MapGeneration::GetSwampElevation(glm::ivec2 pos)
{
    FastNoise& swampNoise = _noises[Swamp];
    FastNoise& perlinX = _noises[PerlinX];
    FastNoise& perlinY = _noises[PerlinY];
    float exp = _exp;
    float terraceValue = _terraceValue;

    float pX = perlinX.GetNoise(pos.x, pos.y) * STRETCHING_SWAMP;
    float pY = perlinY.GetNoise(pos.x, pos.y) * STRETCHING_SWAMP;

    float e = 0.25f * (swampNoise.GetNoise(4.f * (pos.x + pX), 4.f * (pos.y + pY)));
    float e2 = 0.13f * (swampNoise.GetNoise(8.f * (pos.x + pX), 8.f * (pos.y + pY)));
    e += e2;
    e = (e * 0.5f + 0.5f);
    e = e - SWAMP_HEIGHT;

    float elevation = round(e * terraceValue) / terraceValue;
    return elevation;
}

void MapGeneration::GetSwampData(StoredMapData& column, glm::ivec2 pos)
{

    column.approximateElevation = GetSwampElevation(pos);
    column.approximateElevation = column.approximateElevation > 0.f ? (int)floorf(column.approximateElevation * 10.f) : (int)floorf((column.approximateElevation + SWAMP_HEIGHT) * 10.f) - 4.f;
    column.firstBlockLayer = Block::Dirt;
    column.lastBlockLayer = Block::GrassForSwamp;
    if (column.approximateElevation > 0.f && FindBiome(pos, BeachLength(pos), MapGeneration::Ocean).biome == Ocean)
    {
        column.biom = Beach;
        column.firstBlockLayer = Block::Sand;
        column.lastBlockLayer = Block::Sand;
    }
    else if (TreeGeneration(pos) != tree.Nothing)
        column.treeType = IntRand(Trees::First, Trees::OakTreeTypeTwo);
}
