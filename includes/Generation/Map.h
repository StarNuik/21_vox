#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "glm/gtx/hash.hpp"

#include <vector>
#include <unordered_map>
#include <iostream>
#include <math.h>

#include "BiomeDefine.h"
#include "Engine/Engine.h"
#include <cstdint>
#include "FastNoise.h"
#define FREQ 1

class Game;

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
        StoredMapData() {};
    };

    enum GenerationType {
        Basic = 0,
        Land,
        HighLand,
        First = Basic,
        Last = HighLand,
        Size = Last + 1
    };

    StoredMapData Generation(GenerationType genType, glm::ivec2 globalPos, glm::ivec2 blockPosition);
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
    StoredMapData BasicGenerationColumn(glm::ivec2 pos, glm::ivec2 blockPosition);
    StoredMapData LandGenerationColumn(glm::ivec2 pos, glm::ivec2 blockPosition);
    StoredMapData HighLandGenerationColumn(glm::ivec2 pos, glm::ivec2 blockPosition);
    int BiomeDefinition(float e, float m);
    float _Hash(const float n);
    float Noise(const glm::vec3 &x);
    float random (glm::vec2 st)
    {
        return glm::fract(sin(glm::dot(st, glm::vec2(12.9898, 78.233))) * 43758.5453123);
    }
    template <typename T> inline T _Lerp(const T &v0, const T &v1, float t)
    {
        return v0 + (v1 - v0) * std::max(0.f, std::min(1.f, t));
    };
};

