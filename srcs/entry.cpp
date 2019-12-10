#include "Base.h"
#include "Engine/Engine.h"
#include "Engine/Locator.hpp"
#include "Generation/Map.h"

void testGeometry();

#include "Render/Objects.h"
int	main() {
	Game* game = new Game();
	game->InitSystems();
	game->InitWorld();
	game->GameLoop();
	return (0);
}