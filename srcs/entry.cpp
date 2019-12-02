#include "Base.h"
#include "Engine/Engine.h"
#include "Engine/Locator.hpp"

int	main() {
	Game game = Game();
	// Locator::getLogger()->Log("Hi");
	game.GameLoop();
	std::cout << "Hello world!\n";
	return (0);
}