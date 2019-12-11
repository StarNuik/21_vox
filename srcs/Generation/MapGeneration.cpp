#include "./Generation/Map.h"
#define LERP MapGeneration::_Lerp
#define VEC3 glm::vec3

int MapGeneration::BiomeDefinition(float e, float m)
{
  if (e < 0.1)
    return OCEAN;
  if (e < 0.12) return BEACH;
  
  if (e > 0.8) {
    if (m < 0.1) return SCORCHED;
    if (m < 0.2) return BARE;
    if (m < 0.5) return TUNDRA;
    return SNOW;
  }

  if (e > 0.6) {
    if (m < 0.33) return TEMPERATE_DESERT;
    if (m < 0.66) return SHRUBLAND;
    return TAIGA;
  }

  if (e > 0.3) {
    if (m < 0.16) return TEMPERATE_DESERT;
    if (m < 0.50) return GRASSLAND;
    if (m < 0.83) return TEMPERATE_DECIDUOUS_FOREST;
    return TEMPERATE_RAIN_FOREST;
  }

  if (m < 0.16) return SUBTROPICAL_DESERT;
  if (m < 0.33) return GRASSLAND;
  if (m < 0.66) return TROPICAL_SEASONAL_FOREST;
  return TROPICAL_RAIN_FOREST;
}

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

void MapGeneration::Generation(float height, float width, glm::ivec2 pos)
{
  int start_w = (pos.x != 0 ? width * pos.x - width : 0);
  int start_h = (pos.y != 0 ? height * pos.y - height : 0);
  int end_w = (start_w == 0 ? width : width * pos.x);
  int end_h = (start_h == 0 ? height : height * pos.y);

  // std::cout << "Generation: " << std::endl;
  // std::cout << pos.y << " Y|X " << pos.x << std::endl;
  // std::cout << start_h << "  |Y|  " << height * pos.y << std::endl;
  // std::cout << start_w << "  |X|  " << width * pos.x << std::endl;
  // std::cout << "Origin: " << std::endl;
    for (int y = start_h; y < end_h; y++)
    {
      for (int x = start_w; x < end_w; x++)
      {
        float nx = x / width - 0.5f, ny = y / height - 0.5f;
        glm::ivec2 position = glm::ivec2(x, y);
        float e = (1.0f * SimplexNoise::noise(1.f * nx, 1.f * ny)
          + 0.50f * SimplexNoise::noise(2.f * nx, 2.f * ny)
          + 0.25f * SimplexNoise::noise(4.f * nx, 4.f * ny));
          // + 0.13f * SimplexNoise::noise(8.f * nx, 8.f * ny)
          // + 0.06f * SimplexNoise::noise(16.f * nx, 16.f * ny)
          // + 0.03f * SimplexNoise::noise(32.f * nx, 32.f * ny));
        float m = (1.0f * SimplexNoise::noise(1.f * nx, 1.f * ny)
          + 0.75f * SimplexNoise::noise(2.f * nx, 2.f * ny)
          + 0.33f * SimplexNoise::noise(4.f * nx, 4.f * ny)
          + 0.33f * SimplexNoise::noise(8 * nx, 8 * ny)
          + 0.33f * SimplexNoise::noise(16.f * nx, 16.f * ny)
          + 0.50f * SimplexNoise::noise(32.f * nx, 32.f * ny, 1.f));
        m /= (1.0f + 0.75f + 0.33f + 0.33f + 0.33f + 0.50f);
        // (*umap)[position] = new StoredMapData(pow(e, EXP) - 0.42); // резкие горные пики
        (*umap)[position] = new StoredMapData((round((e * 10 * 32) / 32))); // терассы

        (*umap)[position]->biom = BiomeDefinition(e, m);
      }
    }
}


void MapGeneration::SpawnObject(Game *game)
{
    
    Game* _game = game;
    ResourceLoader* r = _game->GetResources();
    for (auto pair : *umap)
    {
      RenderModel* m;
      // if (pair.second->biom == OCEAN)
        m = new RenderModel(r->GetShader("Base"), r->GetTexture(BlockType::Sand), r->GetGeometry("Box"));
      // else if (pair.second->biom == BEACH)
        // m = new RenderModel(r->GetShader("Base"), r->GetTexture("Sand"), r->GetGeometry("Box"));
      // else
        // m = new RenderModel(r->GetShader("Base"), r->GetTexture("Stone"), r->GetGeometry("Box"));
        _game->GetRenderer()->AddModel(m);
        m->SetPosition(glm::vec3(pair.first.x, floorf(pair.second->elevation * 7.f) - 15, pair.first.y));
        if (pair.second->elevation < 0.5 || pair.second->elevation < -0.0000001)
          std::cout << "| x: " << pair.first.x << "| y: " << pair.first.y << "| z: " << pair.second->elevation << std::endl;
    }
}

MapGeneration::MapGeneration()
{
  umap = new std::unordered_map<glm::ivec2, StoredMapData*>();
}

// // // globalPos = glm::ivec3(1, 1, 1);
// 	for (int x = 0; x < 16; x++)
// 		for (int y = 0; y < 16; y++)
// 			for (int z = 0; z < 16; z++) {
// 				int elevation = (int)floorf((*mp->umap)[glm::ivec2(x + globalPos.x * 16, y + globalPos.y * 16)]->elevation);
// 				// if (y < 10)
// 					// SetBlock(glm::ivec3(x, y, z), BlockType::Stone);
// 				// else -- для биомов
// 				// {

// 				// }
// 				SetBlock((glm::ivec3(x + globalPos.x * 16, elevation + globalPos.y * 16,
// 										z + globalPos.z * 16)), BlockType::Stone);
// 			}