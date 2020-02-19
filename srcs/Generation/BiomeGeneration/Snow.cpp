#include "Generation/MapGeneration.h"
#include "World/Block.h"
#include "Utilities/Rand.h"

float MapGeneration::SnowLangGenerationColumn(glm::ivec2 pos)
{
    FastNoise& noise = _noises[Snow];
    float terraceValue = _terraceValue;

    float e = 1.f * (noise.GetNoise(pos.x, pos.y));
    float e1 = 0.50f * (noise.GetNoise(2.f * pos.x, 2.f * pos.y));
    float e2 = 0.25f * (noise.GetNoise(4.f * pos.x, 4.f * pos.y));
    float e3 = 0.13f * (noise.GetNoise(8.f * pos.x, 8.f * pos.y));

    e += e1 + e2 + e3;

    float elevation = round(e * terraceValue) / terraceValue;
    return elevation;
}

void MapGeneration::GetSnowLandData(StoredMapData& column, glm::ivec2 pos)
{
    column.approximateElevation = SnowLangGenerationColumn(pos);
    SmoothingButtJoint(column.approximateElevation, pos, column.biom);
    column.approximateElevation = (int)floorf(column.approximateElevation * 10.f);
    column.firstBlockLayer = Block::Dirt;
    column.lastBlockLayer = Block::SnowGrass;
    if (TreeGeneration(pos) != tree.Nothing)
        column.treeType = intRand(Trees::OakTreeTypeTwo, Trees::SpruceTreeTypeTwo + 1);
}