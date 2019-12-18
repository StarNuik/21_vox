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
#include "FastNoise.h"
#include "Types.h"
#define MAX_DIST_TO_SMOOTHING 30
#define STEP 1 / MAX_DIST_TO_SMOOTHING

class MapGeneration
{
public:
    MapGeneration();
    ~MapGeneration() {};
public:
    struct StoredMapData
    {
        float elevation;
        int biom;
        BlockType firstBlockLayer;
        BlockType lastBlockLayer;
        StoredMapData() {};
    };

    enum GenerationType {
        Basic = 0,
        Ocean,
        Beach,
        GrassLand,
        Snow,
        HighLand,
        Sky,
        Biomes,
        First = Basic,
        Last = Biomes,
        Size = Last + 1
    };

    StoredMapData Generation(glm::ivec2 globalPos, glm::ivec2 blockPosition);
    StoredMapData Generation(glm::ivec2 globalPos, glm::ivec2 blockPosition, GenerationType genType);
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
   
    float BasicGenerationColumn(glm::ivec2 pos);
    float LandGenerationColumn(glm::ivec2 pos);
    float BeachGenerationColumn(glm::ivec2 pos);
    float HighLandGenerationColumn(glm::ivec2 pos);
    float SkyBlockGeneration(glm::ivec2 pos);
  
    int BiomeGeneration(glm::ivec2 pos, glm::ivec2 blockPosition);
    int BiomeDefinition(int elevation);

    void SmoothingButtJoint(float& elevation, glm::ivec2 pos, int biome);
    float CheckingTheBiomeInTheNextColumn(glm::ivec2 originPos, int originBiome, int distance_x, int distance_y); // return elevation
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

