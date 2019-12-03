#include "Base.h"
#include "Engine/Engine.h"
#include "Engine/Locator.hpp"
#include "Generation/Map.h"

void testGeometry();

#include "Render/Objects.h"
int	main() {
	Game game = Game();
	// Locator::getLogger()->Log("Hi");
	game.GameLoop();
    // Geometry* g = new Geometry(std::string("resources/Models/Box.obj"));
//	std::unordered_map<glm::ivec2, StoredMapData*> umap;
//	MapGeneration* mp = new MapGeneration;
//	mp->Generation(16.f, 16.f, umap);
	std::cout << "Hello world!\n";
//	for (auto pair : umap)
//	{
		// if (pair.second->elevation <= 0.4f)
//			std::cout << "pos " << pair.first.x << " " << pair.first.y << ": " << pair.second->elevation << std::endl;
//			std::cout << pair.second->biom << std::endl;
//	}
	return (0);
}