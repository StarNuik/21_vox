#include "World/Block.h"

bool Block::IsTransparent() const {
	switch (_block) {
		case Air:
		case Water:
		case Glass:
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
		case Dandelion:
			return true;
		default:
			return false;
	}
}