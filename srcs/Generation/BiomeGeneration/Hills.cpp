#include "Generation/MapGeneration.h"
#include "World/Block.h"
#include "Utilities/Rand.h"

float MapGeneration::GetHillsElevation(glm::ivec2 pos)
{
	FastNoise& hillsNoise = _noises[Hills];
    float terraceValue = 32.f;

    float e = hillsNoise.GetNoise(pos.x, pos.y);
    float e1 = 0.5f * (hillsNoise.GetNoise(2.f * pos.x, 2.f * pos.y));
    float e2 = 0.25f * (hillsNoise.GetNoise(4.f * pos.x, 4.f * pos.y));
    float e3 = 0.13f * (hillsNoise.GetNoise(8.f * pos.x, 8.f * pos.y));
    float e4 = 0.06f * (hillsNoise.GetNoise(16.f * pos.x, 16.f * pos.y));

    e += e1 + e2 + e3 + e4;
    e = ((e + 0.5f) * 0.5f) * 1.75f;
    float elevation = round(e * terraceValue) / terraceValue;
    return elevation;
}

void MapGeneration::GetGrassLandHillsData(StoredMapData& column, glm::ivec2 pos)
{
    column.approximateElevation = GetHillsElevation(pos);
    column.approximateElevation = SmoothBiomeSeams(column.approximateElevation, pos, column.biom);
    column.approximateElevation = (int)floorf(column.approximateElevation * 10.f);
    column.firstBlockLayer = Block::Dirt;
    column.lastBlockLayer = Block::Grass;
    if (FindBiome(pos, BeachLength(pos), MapGeneration::Ocean).biome == Ocean)
    {
        column.biom = Beach;
        column.firstBlockLayer = Block::Sand;
        column.lastBlockLayer = Block::Sand;
    }
    else if (TreeGeneration(pos) != tree.Nothing)
        column.treeType = IntRand(Trees::First, Trees::OakTreeTypeTwo);
}

void MapGeneration::GetDesertHillsData(StoredMapData& column, glm::ivec2 pos)
{
    column.approximateElevation = GetHillsElevation(pos);
    column.approximateElevation = SmoothBiomeSeams(column.approximateElevation, pos, column.biom);
    column.approximateElevation = (int)floorf(column.approximateElevation * 10.f);
    column.firstBlockLayer = Block::Sand;
    column.lastBlockLayer = Block::Sand;
    if (TreeGeneration(pos) != tree.Nothing)
        column.treeType = Trees::Сactus;
}

void MapGeneration::GetSnowHillsData(StoredMapData& column, glm::ivec2 pos)
{
    column.approximateElevation = GetSnowLandElevation(pos);
    column.approximateElevation = SmoothBiomeSeams(column.approximateElevation, pos, column.biom);
    column.approximateElevation = (int)floorf(column.approximateElevation * 10.f);
    column.firstBlockLayer = Block::Dirt;
    column.lastBlockLayer = Block::SnowGrass;
    if (TreeGeneration(pos) != tree.Nothing)
        column.treeType = IntRand(Trees::OakTreeTypeTwo, Trees::SpruceTreeTypeTwo + 1);
}

