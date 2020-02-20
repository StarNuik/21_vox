#include "Generation/MapGeneration.h"
#include "World/Block.h"

//! rename to GetDesertElevation
float MapGeneration::DesertGenerationColumn(glm::ivec2 pos)
{
    FastNoise& noise = _noises[Desert];
    float terraceValue = _terraceValue;
    float e = 1.f * (noise.GetNoise(1.f * pos.x, 1.f * pos.y));
    e = (e * 0.5f + 0.5f); // range 0..1;
    float terrace = round(e * terraceValue) / terraceValue;  
    float elevation = terrace;

    return elevation;
}

//! WHY
//! change to return type MapGeneration::GenerationType
void MapGeneration::GetDesertData(StoredMapData& column, glm::ivec2 pos)
{
    column.approximateElevation = DesertGenerationColumn(pos);
    SmoothingButtJoint(column.approximateElevation, pos, column.biom);
    //! column.approximateElevation = SmoothingButtJoint(column.approximateElevation, pos, column.biom);
    column.approximateElevation = (int)floorf(column.approximateElevation * 10.f);
    column.firstBlockLayer = Block::Sand;
    column.lastBlockLayer = Block::Sand;
    if (TreeGeneration(pos) != tree.Nothing)
        column.treeType = Trees::Сactus;
}

