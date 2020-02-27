#pragma once

#include <string>

#include <glm/glm.hpp>
#include <glm/gtx/hash.hpp>

#include "Trees.h"
#include "FastNoise.h"
#include "Types.h"

#define MAX_DIST_TO_CHECK_BIOME 5
#define MAX_DIST_TO_SMOOTHING 25
#define MAX_DIAGONAL_DIST_TO_SMOITHING 5
#define STEP 1 / MAX_DIST_TO_SMOOTHING

#define MIN_WATER_LEVEL 40
#define MAX_WATER_LEVEL 60
#define WATER_ELEVATION 0.2F

#define SWAMP_HEIGHT 0.382f

class MapGeneration
{
public:
    MapGeneration();
    ~MapGeneration() {};
    Trees tree;

    enum GenerationType {
        Basic = 0,
        Ocean,
        Swamp,
        River,
        Glade,
        Beach,
        BeachBordered,
        GrassLand,
        Snow,
        Desert,
        HighLand,
        ChineseMountains,
        Caves,
        Crevices,
        Vegetation,
        GladeVegetation,
        Ore,
        OreDimond,
        Tree,
        PerlinX,
        PerlinY,
        Biomes,
        Nothing, //! NoBiome mb
        First = Basic,
        Last = Biomes,
        Size = Last + 1
    };

    struct StoredMapData
    {
		float approximateElevation;
		int exactElevation;
		GenerationType biom;
		__BLOCK_TYPE firstBlockLayer;
		__BLOCK_TYPE lastBlockLayer;
		__BLOCK_TYPE treeType;
		int aboveRiverElevation;
		int aboveRiverBiome;
		StoredMapData() {};
    };

    struct StoredOreData
    {
        glm::ivec3 pos;
        __BLOCK_TYPE type;
        StoredOreData() {};
    };


    StoredMapData Generation(glm::ivec2 chunkPos, glm::ivec2 columnPos);
    StoredMapData Generation(glm::ivec2 chunkPos, glm::ivec2 columnPos, GenerationType genType);
    StoredOreData OreGeneration(glm::ivec2 chunkPos, glm::ivec3 columnPos, int maxHeight);
    __BLOCK_TYPE VegetationGeneration(glm::ivec2 chunkPos, glm::ivec2 columnPos, GenerationType biome);
    bool CrevicesGeneration(glm::ivec2 chunkPos, glm::ivec3 columnPos);
    bool CavesGeneration(glm::ivec2 chunkPos, glm::ivec3 columnPos);
    FastNoise& GetNoise(GenerationType);
    float GetExpValue();
    void SetExpValue(float value);
    float GetTeracceValue();
    void SetTerraceValue(float value);
	std::string GetNoiseName(GenerationType);
private:

    float _exp; // For sharp mountain peaks
    float _terraceValue; // For terrace
    FastNoise _noises[Size];
	std::string _noiseNames[Size];
    struct BiomeInfo
    {
        GenerationType biome;
        int distanceToTheNextBiome;
    };

    enum VegetationType {
       RedFlower = 0,
       YellowFlower,
       BlueFlower,
       HighGrass,
       RedMushroom,
       BrownMushroom,
       DeadShrub,
       NothingVegetation,
       FirstVegetation = RedFlower,
       LastVegetation = DeadShrub,
       SizeVegetation = Last + 1
    };
    __BLOCK_TYPE ConvertPlantToBlock(VegetationType vegetation);
    VegetationType GrassLandVegetationGeneration(glm::ivec2 pos);
    VegetationType DesertVegetationGeneration(glm::ivec2 pos);
    VegetationType CavesVegetationGeneration(glm::ivec2 pos);
    VegetationType SwampVegetationGeneration(glm::ivec2 pos);
    VegetationType ChineseMountainsVegetationGeneration(glm::ivec2 pos);
    VegetationType GladeVegetationGeneration(glm::ivec2 pos);
    bool IsThereAPlant(glm::ivec2 pos, int R, GenerationType noiseType);

    int     BeachLength(glm::ivec2 pos);

    void GetBasicData(StoredMapData& column, glm::ivec2 pos);
    void GetOceanData(StoredMapData& column, glm::ivec2 pos);
    void GetRiverData(StoredMapData& column, glm::ivec2 pos);
    void GetGrassLandData(StoredMapData& column, glm::ivec2 pos);
    void GetSwampData(StoredMapData& column, glm::ivec2 pos);
    void GetDesertData(StoredMapData& column, glm::ivec2 pos);
    void GetSnowLandData(StoredMapData& column, glm::ivec2 pos);
    void GetHighLandData(StoredMapData& column, glm::ivec2 pos);
    void GetChineseMountainsData(StoredMapData& column, glm::ivec2 pos);
    void GetGladeData(StoredMapData& column, glm::ivec2 pos);
   
    float GetBasicElevation(glm::ivec2 pos);
    float GetLandElevation(glm::ivec2 pos);
    float GetSwampElevation(glm::ivec2 pos);
    float GetDesertElevation(glm::ivec2 pos);
    float GetSnowLandElevation(glm::ivec2 pos);
    float GetHighLandElevation(glm::ivec2 pos);
    float GetChineseMountainsElevation(glm::ivec2 pos);
    float GetGladeElevation(glm::ivec2 pos);


    float TreeGeneration(glm::ivec2 pos);

    float GetRiverElevation(glm::ivec2 pos);
  
    GenerationType GenerationBiome(glm::ivec2 pos);
    GenerationType GenerationBiomeWithoutRiver(glm::ivec2 pos);

    __BLOCK_TYPE GetOreType(float noiseValue, int currBlockHeight, int maxBlockHeight);
    __BLOCK_TYPE RegenerateDimond(glm::vec3 pos);

    float SmoothBiomeSeams(float elevation, glm::ivec2 pos, GenerationType biome); // Smoothes the seams between biomes
    
    BiomeInfo FindBiome(const glm::ivec2 pos, const int maxDistToCheckBiome, const GenerationType biome);
    GenerationType BiomeInPositionOfInterest(const glm::ivec2 origPos, const glm::vec2 distance);
    float CheckingTheElevationOfBiomeInTheNextColumn(glm::ivec2 originPos, glm::ivec2 distance, GenerationType originBiome);
    
    // float GetApprox(float e0, float e1, float e2, float e3, float e4, float e5, float e6, float e7); // returns average height among nearby blocks
    float GetApprox(float e0, float e1, float e2, float e3); // returns average height among nearby blocks

    template <typename T> inline T Lerp(const T &v0, const T &v1, float t)
    {
        t = glm::clamp(t, 0.f, 1.f);
        return (1.f - t) * v0 + t * v1;
    };
};

