#include "Generation/Map.h"

// void MapGeneration(float height, float width, std::unordered_map<glm::ivec2, StoredMapData*> *umap)
// {
//     siv::PerlinNoise ns;
//     for (int y = 0; y < height; y++)
//     {
//         for (int x = 0; x < width; x++)
//         {
//             double nx = x / width - 0.5, ny = y / height - 0.5;
//             glm::ivec2 pos = glm::ivec2(x, y);
//             umap[pos] = new StoredMapData(ns.noise(nx, ny));
// //          value[y][x] = ns.noise(nx, ny);
//         }
//     }
// }

// int main()
// {
//     std::unordered_map<glm::ivec2, StoredMapData*> umap;

//     MapGeneration(4, 4, &umap);
//     for (auto umap)
//         std::cout << umap.first << " " << umap.second << endl;

// }
