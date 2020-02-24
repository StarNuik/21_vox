#include "Generation/MapGeneration.h"

int     MapGeneration::BeachLength(glm::ivec2 pos)
{
    FastNoise& noise = _noises[BeachBordered];
    float e = (noise.GetNoise(pos.x, pos.y) * 0.5f + 0.5f);
    int beachLength;
    beachLength = (e > 0.5f ? (int)(e + e * 10.f) : 0);
    return beachLength;
}
