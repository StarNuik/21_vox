#include "World/World.h"

Block::Block(Texture* texture) {_texture = texture; _isWalkable = true;};
Block::~Block() {};