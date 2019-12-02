#include "./Generation/Map.h"

class MapGeneration
{
    template <typename T> inline T _Lerp(const T &v0, const T &v1, float t)
    {
        return v0 + (v1 - v0) * std::max(0.f, std::min(1.f, t));
    }

    float _Hash(const float n)
    {
        float x = sin(n) * 43758.5453f;
        return x - floor(x);
    }

    float Noise(const glm::vec3 &x)
    {
        glm::vec3 p(floor(x.x), floor(x.y), floor(x.z));
        glm::vec3 f(x.x - p.x, x.y - p.y, x.z - p.z);
        f = f * (f * (glm::vec3(3.f, 3.f, 3.f) - f * 2.f));
        float n = glm::dot(p, glm::vec3(1.f, 57.f, 113.f));
        return _Lerp(_Lerp(
                        _Lerp(_Hash(n +  0.f), _Hash(n +  1.f), f.x),
                        _Lerp(_Hash(n + 57.f), _Hash(n + 58.f), f.x), f.y),
                    _Lerp(
                        _Lerp(_Hash(n + 113.f), _Hash(n + 114.f), f.x),
                        _Lerp(_Hash(n + 170.f), _Hash(n + 171.f), f.x), f.y), f.z);
    }

    void Generation(float height, float width, std::unordered_map<glm::ivec2, StoredMapData*> umap)
    {
        for (int y = 0; y < height; y++)
        {
            for (int x = 0; x < width; x++)
            {
                float nx = x / width - 0.5, ny = y / height - 0.5;
                glm::ivec2 pos = glm::ivec2(x, y);
                umap[pos] = new StoredMapData(Noise(glm::vec3(nx, ny, 0.f)));
            }
        }
    }
};
