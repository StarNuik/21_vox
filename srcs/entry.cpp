#include "Base.h"
#include "Engine/Engine.h"
#include "Engine/Locator.hpp"

void testGeometry();

#include "Render/Objects.h"
int	main() {
	Game game = Game();
	// Locator::getLogger()->Log("Hi");
	game.GameLoop();
	// std::cout << "Hello world!\n";
	// Geometry* g = new Geometry(std::string("resources/Models/Box.obj"));
	return (0);
}