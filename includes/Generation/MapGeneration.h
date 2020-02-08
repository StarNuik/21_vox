#pragma once

// #include <vector>
// #include <unordered_map>
// #include <iostream>
// #include <math.h>
#include <string>

// #include <GL/glew.h>
// #include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/hash.hpp>


// #include "BiomeDefine.h"
// #include "Engine/Engine.h"
// #include <cstdint>
#include "Trees.h"
#include "FastNoise.h"
#include "Types.h"
#define MAX_DIST_TO_SMOOTHING 25
#define STEP 1 / MAX_DIST_TO_SMOOTHING

#define MAX_DIST_TO_CHECK_BIOME 5

class MapGeneration
{
public:
    MapGeneration();
    ~MapGeneration() {};
    Trees tree;

    struct StoredMapData
    {
        float approximateElevation;
        int exactElevation;
        int biom;
        __BLOCK_TYPE firstBlockLayer;
        __BLOCK_TYPE lastBlockLayer;
        __BLOCK_TYPE treeType;
        StoredMapData() {};
    };

    struct StoredOreData
    {
        glm::ivec3 pos;
        __BLOCK_TYPE type;
        StoredOreData() {};
    };

    enum GenerationType {
        Basic = 0,
        Ocean,
        Beach,
        BeachBordered,
        GrassLand,
        Snow,
        HighLand,
        ShapeCaves,
        SecondShapeCaves,
        ElevationCaves,
        SecondElevationCaves,
        Crevices,
        Ore,
        OreDimond,
        Tree,
        River,
        PerlinX,
        PerlinY,
        Biomes,
        First = Basic,
        Last = Biomes,
        Size = Last + 1
    };

    StoredMapData Generation(glm::ivec2 globalPos, glm::ivec2 blockPosition);
    StoredMapData Generation(glm::ivec2 globalPos, glm::ivec2 blockPosition, GenerationType genType);
    StoredOreData OreGeneration(glm::ivec2 globalPos, glm::ivec3 blockPosition, int maxHeight);
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
        int biome;
        int distanceToTheNextBiome;
    };
   
    float BasicGenerationColumn(glm::ivec2 pos);
    float LandGenerationColumn(glm::ivec2 pos);
    float BeachGenerationColumn(glm::ivec2 pos);
    float HighLandGenerationColumn(glm::ivec2 pos);
    float SnowLangGenerationColumn(glm::ivec2);

    float TreeGeneration(glm::ivec2 pos);

    float ShapeCavesGeneration(glm::ivec2 pos);
    float ElevationCavesGeneration(glm::ivec2 pos);

    float SecondShapeCavesGeneration(glm::ivec2 pos);
    float SecondElevationCavesGeneration(glm::ivec2 pos);

    float RiverElevationGeneration(glm::ivec2 pos);

    float CrevicesGeneration(glm::ivec2 pos);
  
    int BiomeGeneration(glm::ivec2 pos);
    int BiomeDefinition(int elevation, glm::ivec2 pos);
    int TestBiomeDefinition(float e, glm::ivec2 pos);

    __BLOCK_TYPE OreDefinition(float elevation, int currBlockHeight, int maxBlockHeight);
    __BLOCK_TYPE RegenerateDimond(glm::vec3 pos);

    void SmoothingButtJoint(float& elevation, glm::ivec2 pos, int biome);
    BiomeInfo CheckingTheBiomeIntTheNextColumn(const glm::ivec2 pos, const int biome, const int maxDistToCheckBiome); // return distance to closest biome and their biome number
    int BiomeInPositionOfInterest(const glm::ivec2 origPos, const glm::vec2 distance); // return biome
    float CheckingTheElevationOfBiomeInTheNextColumn(glm::ivec2 originPos, int originBiome, int distance_x, int distance_y); // return elevation
    float GetApprox(float e1, float e2, float e3, float e4); // returns average height among nearby blocks


    float _Hash(const float n);
    float Noise(const glm::vec3 &x);
    float Smoothstep(float edge0, float edge1, float x);
    glm::vec2 random2(glm::vec2 p);

    float Lerp(float v0, float v1, float t);

    float random (glm::vec2 st)
    {
        return glm::fract(sin(glm::dot(st, glm::vec2(12.9898, 78.233))) * 43758.5453123);
    }

    template <typename T> inline T _Lerp(const T &v0, const T &v1, float t)
    {
        return v0 + (v1 - v0) * std::max(0.f, std::min(1.f, t));
    };
};

