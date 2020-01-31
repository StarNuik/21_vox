// #include "Base.h"
#define TINYOBJLOADER_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define GLEW_STATIC

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Engine/Game.h"

int	main() {
	Game* game = new Game();
	game->InitSystems();
	game->InitWorld();
	game->GameLoop();
	return (0);
}
