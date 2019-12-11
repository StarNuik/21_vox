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
#include "FastNoise.h"
#define FREQ 1
#define EXP 0.6
// #define EXP 2.8

class Game;

struct StoredMapData
{
    float elevation;
    int biom;
    
    StoredMapData(float _value) {elevation = _value;};
};

class MapGeneration
{
    public:
        MapGeneration();
        ~MapGeneration() {};    
    public:
        std::unordered_map<glm::ivec2, StoredMapData*> umap;

        int BiomeDefinition(float e, float m);
        
        std::unordered_map<glm::ivec2, StoredMapData*> Generation(float height, float width, glm::ivec2 pos);

        void SpawnObject(Game *game);
    private:
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

