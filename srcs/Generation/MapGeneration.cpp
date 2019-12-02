#include "./Generation/Map.h"
#define LERP MapGeneration::_Lerp

float MapGeneration::_Hash(const float n)
{
    float x = sin(n) * 43758.5453f;
    return x - floor(x);
}

float MapGeneration::Noise(const glm::vec3 &x)
{
    glm::vec3 p(floor(x.x), floor(x.y), floor(x.z));
    glm::vec3 f(x.x - p.x, x.y - p.y, x.z - p.z);
    f = f * (f * (glm::vec3(3.f, 3.f, 3.f) - f * 2.f));
    float n = glm::dot(p, glm::vec3(1.f, 57.f, 113.f));
    return LERP(LERP(
                    LERP(_Hash(n +  0.f), _Hash(n +  1.f), f.x),
                    LERP(_Hash(n + 57.f), _Hash(n + 58.f), f.x), f.y),
                LERP(
                    LERP(_Hash(n + 113.f), _Hash(n + 114.f), f.x),
                    LERP(_Hash(n + 170.f), _Hash(n + 171.f), f.x), f.y), f.z);
}

void MapGeneration::Generation(float height, float width, std::unordered_map<glm::ivec2, StoredMapData*> &umap)
{
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            float nx = x / width - 0.5, ny = y / height - 0.5;
            glm::ivec2 pos = glm::ivec2(x, y);
            umap[pos] = new StoredMapData(Noise(glm::vec3(nx, ny, 1.f)));
        }
    }
}
