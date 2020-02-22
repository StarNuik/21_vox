#include "Generation/MapGeneration.h"
#include "World/Block.h"
#include "Utilities/Rand.h"

float MapGeneration::GetLandElevation(glm::ivec2 pos)
{
	FastNoise& grassNoise = _noises[GrassLand];
    float terraceValue = _terraceValue;

    float e = 0.5f * (grassNoise.GetNoise(2.f * pos.x, 2.f * pos.y));
    e = (e * 0.5f + 0.5f);
  
    float elevation = round(e * terraceValue) / terraceValue;
    return elevation;
}

void MapGeneration::GetGrassLandData(StoredMapData& column, glm::ivec2 pos)
{
    column.approximateElevation = GetLandElevation(pos);
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
