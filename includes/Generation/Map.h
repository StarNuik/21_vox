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
#include "OpenSimplexNoise/OpenSimplexNoise.h"
#define FREQ 1
#define EXP 0.6
// #define EXP 1

class Game;

class StoredMapData
{
    public:
        float elevation;
        int biom;
//      std::vector<Blocks*> vec;
    StoredMapData(float _value) {elevation = _value;};
};

class MapGeneration
{
    public:
        MapGeneration() {};
        ~MapGeneration() {};
        float a;
        std::unordered_map<glm::ivec2, StoredMapData*>* umap;

        int BiomeDefinition(float e, float m);

        float random (glm::vec2 st) 
        {
            return glm::fract(sin(glm::dot(st, glm::vec2(12.9898, 78.233))) * 43758.5453123);
        }

        template <typename T> inline T _Lerp(const T &v0, const T &v1, float t)
        {
            return v0 + (v1 - v0) * std::max(0.f, std::min(1.f, t));
        };
        
        float _Hash(const float n);
        float Noise(const glm::vec3 &x);
        void Generation(float height, float width, std::unordered_map<glm::ivec2, StoredMapData*>* umap);
        void SpawnObject(Game *game, std::unordered_map<glm::ivec2, StoredMapData*>* umap);
};

