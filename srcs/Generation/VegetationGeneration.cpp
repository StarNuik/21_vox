#include "Generation/MapGeneration.h"
#include "World/Block.h"
#include "Utilities/Rand.h"

bool MapGeneration::IsThereAPlant(glm::ivec2 pos, int R, GenerationType noiseType)
{
  FastNoise& noise = _noises[noiseType];
  float max = 0;
  for (int yn = pos.y - R; yn <= pos.y + R; yn++)
  {
    for (int xn = pos.x - R; xn <= pos.x + R; xn++)
    {
        float e = noise.GetNoise(xn, yn);
        if (e > max)
            max = e;
    }
  }
  if (noise.GetNoise(pos.x, pos.y) == max)
      return true;
  return false;
}

float MapGeneration::TreeGeneration(glm::ivec2 pos)
{
  FastNoise& noise = _noises[Tree];
  int R = 3;
  float max = 0;

  for (int yn = pos.y - R; yn <= pos.y + R; yn++) {
    for (int xn = pos.x - R; xn <= pos.x + R; xn++) {
      float e = noise.GetNoise(xn, yn);
      if (e > max)
        max = e;
    }
  }
  if (noise.GetNoise(pos.x, pos.y) == max)
    return 1.f;
  return tree.Nothing;
}

__BLOCK_TYPE MapGeneration::ConvertPlantToBlock(VegetationType vegetation)
{
  switch (vegetation)
  {
    case VegetationType::RedFlower:
     return Block::RedFlower;
    break;
    case VegetationType::YellowFlower:
     return Block::YellowFlower;
    break;
    case VegetationType::BlueFlower:
     return Block::BlueFlower;
    break;
    case VegetationType::HighGrass:
      return Block::HighGrass;
    break;
    case VegetationType::RedMushroom:
      return Block::RedMushroom;
    break;
    case VegetationType::BrownMushroom:
      return Block::BrownMushroom;
    break;
    case VegetationType::DeadShrub:
      return Block::DeadShrub;
    break;
    default:
      return Block::Air;
    break;
  }
  return Block::Air;
}

MapGeneration::VegetationType MapGeneration::SwampVegetationGeneration(glm::ivec2 pos)
{
  int probabilityCalculation = IntRand(0, 100);

  if (probabilityCalculation < 65 && IsThereAPlant(pos, 1, GenerationType::Vegetation))
    return VegetationType::HighGrass;
  else if (probabilityCalculation >= 65 && probabilityCalculation < 90 && IsThereAPlant(pos, 3, GenerationType::Vegetation))
    return (VegetationType)IntRand(VegetationType::RedMushroom, VegetationType::BrownMushroom + 1);
  return VegetationType::NothingVegetation;
}

MapGeneration::VegetationType MapGeneration::DesertVegetationGeneration(glm::ivec2 pos)
{
  if (IsThereAPlant(pos, 5, GenerationType::Tree))
    return VegetationType::DeadShrub;
  return VegetationType::NothingVegetation;
}

MapGeneration::VegetationType MapGeneration::ChineseMountainsVegetationGeneration(glm::ivec2 pos)
{
  int probabilityCalculation = IntRand(0, 100);

  if (probabilityCalculation < 75 && IsThereAPlant(pos, 1, GenerationType::Vegetation))
    return VegetationType::HighGrass;
  else if (probabilityCalculation >= 75 && probabilityCalculation < 100 && IsThereAPlant(pos, 2, GenerationType::Vegetation))
    return (VegetationType)IntRand(VegetationType::RedFlower, VegetationType::BlueFlower + 1);
  return VegetationType::NothingVegetation;
}

MapGeneration::VegetationType MapGeneration::CavesVegetationGeneration(glm::ivec2 pos)
{
  if (IsThereAPlant(pos, 8, GenerationType::Tree))
    return VegetationType::BrownMushroom;
  return VegetationType::NothingVegetation;
}

MapGeneration::VegetationType MapGeneration::GrassLandVegetationGeneration(glm::ivec2 pos)
{
  int probabilityCalculation = IntRand(0, 100);

  if (probabilityCalculation < 65 && IsThereAPlant(pos, 1, GenerationType::Vegetation))
    return VegetationType::HighGrass;
  else if (probabilityCalculation >= 65 && probabilityCalculation < 90 && IsThereAPlant(pos, 2, GenerationType::Vegetation))
    return (VegetationType)IntRand(VegetationType::RedFlower, VegetationType::BlueFlower + 1);
  else if (IsThereAPlant(pos, 3, GenerationType::Vegetation))
      return (VegetationType)IntRand(VegetationType::RedMushroom, VegetationType::BrownMushroom + 1);
  return VegetationType::NothingVegetation;
}

__BLOCK_TYPE MapGeneration::VegetationGeneration(glm::ivec2 chunkPos, glm::ivec2 columnPos, GenerationType biome)
{
  glm::ivec2 globalChunkPos = chunkPos * 16;
	glm::ivec2 pos = globalChunkPos + columnPos;

  VegetationType vegetation;

  switch (biome)
  {
    case GenerationType::GrassLand:
      vegetation = GrassLandVegetationGeneration(pos);
    break;
    case GenerationType::Desert:
      vegetation = DesertVegetationGeneration(pos);
    break;
    case GenerationType::ChineseMountains:
      vegetation = ChineseMountainsVegetationGeneration(pos);
    break;
    case GenerationType::Caves:
      vegetation = CavesVegetationGeneration(pos);
    break;
    case GenerationType::Swamp:
      vegetation = SwampVegetationGeneration(pos);
    break;
    default:
      return Block::Air;
    break;
  }
  return ConvertPlantToBlock(vegetation);
}
