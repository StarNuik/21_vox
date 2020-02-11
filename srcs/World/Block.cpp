#include "World/Block.h"

bool Block::IsTransparent() const {
	switch (_block) {
		case NoChunk:
		case Air:
		case Water:
			return true;
		default:
			return false;
	}
}

bool Block::IsMultisided() const {
	switch (_block) {
		case CraftingTable:
		case Grass:
		case Log:
		case SnowGrass:
			return true;
		default:
			return false;
	}
}

bool Block::IsFlower() const {
	switch(_block) {
		case RedFlower:
		case YellowFlower:
		case BlueFlower:
		case RedMushroom:
		case BrownMushroom:
			return true;
		default:
			return false;
	}
}

bool Block::IsWalkthrough() const {
	switch(_block) {
		case Air:
		case RedFlower:
		case YellowFlower:
		case BlueFlower:
		case RedMushroom:
		case BrownMushroom:
			return true;
		default:
			return false;
	}
}