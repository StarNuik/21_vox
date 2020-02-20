#include "Generation/MapGeneration.h"
#include "World/Block.h"
#include "Utilities/Rand.h"

float MapGeneration::LandGenerationColumn(glm::ivec2 pos)
{
	FastNoise& noise = _noises[GrassLand];
    float terraceValue = _terraceValue;

    float e = 0.5f * (noise.GetNoise(2.f * pos.x, 2.f * pos.y));
    e = (e * 0.5f + 0.5f);
  
    float elevation = round(e * terraceValue) / terraceValue;
    return elevation;
}

void MapGeneration::GetGrassLandData(StoredMapData& column, glm::ivec2 pos)
{
    FastNoise& noise = _noises[BeachBordered];
    float beachLength = (noise.GetNoise(pos.x, pos.y) * 0.5f + 0.5f); 
    int checkLength;
    if (beachLength > 0.5f)
        checkLength = (int)beachLength + beachLength * 10;
    else
        checkLength = 0; 
    column.approximateElevation = LandGenerationColumn(pos); //! GetLandHeight
    SmoothingButtJoint(column.approximateElevation, pos, column.biom);
    column.approximateElevation = (int)floorf(column.approximateElevation * 10.f);
    column.firstBlockLayer = Block::Dirt;
    column.lastBlockLayer = Block::Grass;
    if (CheckingTheBiomeIntTheNextColumn(pos, column.biom, checkLength).biome == Ocean)
    {
        column.biom = Beach;
        column.firstBlockLayer = Block::Sand;
        column.lastBlockLayer = Block::Sand;
    }
    else if (TreeGeneration(pos) != tree.Nothing)
        column.treeType = intRand(Trees::First, Trees::OakTreeTypeTwo); //! OMG BAD METHOD NAME
}