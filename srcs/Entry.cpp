// #include "Base.h"
#define TINYOBJLOADER_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
// #include "Engine/Engine.h"
// #include "Engine/Locator.hpp"
// #include "Generation/Map.h"

// #include "Render/Objects.h"

// #include "Types.h"
#include "Engine/Game.h"

int	main() {
	Game* game = new Game();
	game->InitSystems();
	game->InitWorld();
	game->GameLoop();
	return (0);
}
