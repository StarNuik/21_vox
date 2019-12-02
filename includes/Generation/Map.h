#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "glm/gtx/hash.hpp"

#include <vector>
#include <unordered_map>
#include <iostream>


class StoredMapData
{
    public:
        float value;
//      std::vector<Blocks*> vec;
    StoredMapData(float _value) {value = _value;};
};

class MapGeneration
{
    public:

    template <typename T> inline T _Lerp(const T &v0, const T &v1, float t)
    {
        return v0 + (v1 - v0) * std::max(0.f, std::min(1.f, t));
    };

    float _Hash(const float n);

    float Noise(const glm::vec3 &x);

    void Generation(float height, float width, std::unordered_map<glm::ivec2, StoredMapData*> &umap);
};

