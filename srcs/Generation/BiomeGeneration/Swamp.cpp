#include "Generation/MapGeneration.h"
#include "World/Block.h"
#include "Utilities/Rand.h"

#define STRETCHIBG_SWAMP 21.6543f

float MapGeneration::SwampGenerationColumn(glm::ivec2 pos)
{
    FastNoise& noise = _noises[Swamp]; //0.021
    FastNoise& perlinX = _noises[PerlinX];
    FastNoise& perlinY = _noises[PerlinY];
    float exp = _exp;
    float terraceValue = _terraceValue;

    float pX = perlinX.GetNoise(pos.x, pos.y) * STRETCHIBG_SWAMP;
    float pY = perlinY.GetNoise(pos.x, pos.y) * STRETCHIBG_SWAMP;

    float e = 0.25f * (noise.GetNoise(4.f * (pos.x + pX), 4.f * (pos.y + pY)));
    float e2 = 0.13f * (noise.GetNoise(8.f * (pos.x + pX), 8.f * (pos.y + pY)));
    e += e2;
    e = (e * 0.5f + 0.5f);
    e = e - SWAMP_HEGHT;

    float elevation = round(e * terraceValue) / terraceValue;
    return elevation;
}

void MapGeneration::GetSwampData(StoredMapData& column, glm::ivec2 pos)
{
    FastNoise& noise = _noises[BeachBordered];
    float beachLength = (noise.GetNoise(pos.x, pos.y) * 0.5f + 0.5f);
    int checkLength;
    if (beachLength > 0.5f)
        checkLength = (int)beachLength + beachLength * 10;
    else
        checkLength = 0;
    column.approximateElevation = SwampGenerationColumn(pos);
    column.approximateElevation = column.approximateElevation > 0.f ? (int)floorf(column.approximateElevation * 10.f) : (int)floorf((column.approximateElevation + SWAMP_HEGHT) * 10.f) - 4.f;
    column.firstBlockLayer = Block::Dirt;
    column.lastBlockLayer = Block::Grass;
    if (column.approximateElevation > 0.f && CheckingTheBiomeIntTheNextColumn(pos, column.biom, checkLength).biome == Ocean)
    {
        column.biom = Beach;
        column.firstBlockLayer = Block::Sand;
        column.lastBlockLayer = Block::Sand;
    }
    else if (TreeGeneration(pos) != tree.Nothing)
        column.treeType = intRand(Trees::First, Trees::OakTreeTypeTwo);
}
