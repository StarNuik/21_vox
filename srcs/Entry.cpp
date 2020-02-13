// #include "Base.h"
#define TINYOBJLOADER_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define GLEW_STATIC

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Engine/Game.h"

#include "World/Block.h"

int	main() {
	Game* game = new Game();
	game->InitSystems();
	game->InitWorld();
	Block b = Block::Stone;
	std::cout << "Stone uv(0, 0) u:" << b.GetU(Block::Top) + BLOCK_SIZE_U * 0.f << ", v: " << b.GetV(Block::Top) + BLOCK_SIZE_V * 0.f << std::endl;
	std::cout << "Stone uv(1, 1) u:" << b.GetU(Block::Top) + BLOCK_SIZE_U * 1.f << ", v: " << b.GetV(Block::Top) + BLOCK_SIZE_V * 1.f << std::endl;
	game->GameLoop();
	return (0);
}
