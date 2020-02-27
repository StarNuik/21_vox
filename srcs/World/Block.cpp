#include "World/Block.h"

bool Block::IsTransparent() const {
	switch (_block) {
		// case NoChunk:
		case Air:
		case Water:
		case WaterForSwamp:
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
		case GrassForSwamp:
		case Log:
		case PineLog:
		case BirchLog:
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
		case DeadShrub:
		case HighGrass:
		case Cactus:
			return true;
		default:
			return false;
	}
}

bool Block::IsWater() const {
	switch (_block)
	{
		case Water:
		case WaterForSwamp:
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
		case DeadShrub:
		case HighGrass:
			return true;
		default:
			return false;
	}
}

namespace {
	inline float IntToU(int u) {return BLOCK_SIZE_U * (double)u;};
	inline float IntToV(int v) {return BLOCK_SIZE_V * (double)v;};
	inline float SideCase(Block::Side side, float top, float bottom, float right, float left, float front, float back) {
		switch (side) {
			case Block::Top:
				return top;
			case Block::Bottom:
				return bottom;
			case Block::Right:
				return right;
			case Block::Left:
				return left;
			case Block::Front:
				return front;
			case Block::Back:
				return back;
			default:
				return BLOCK_SIZE_U * 0.5;
		}
	}
}

float Block::GetU(Side side) const {
	if (IsMultisided()) {
		switch(_block) {
			case CraftingTable:
				return SideCase(side, IntToU(11), IntToU(4), IntToU(11), IntToU(11), IntToU(12), IntToU(12));
			case Grass:
				return SideCase(side, IntToU(0), IntToU(2), IntToU(3), IntToU(3), IntToU(3), IntToU(3));
			case Log:
				return SideCase(side, IntToU(5), IntToU(5), IntToU(4), IntToU(4), IntToU(4), IntToU(4));
			case SnowGrass:
				return SideCase(side, IntToU(2), IntToU(2), IntToU(4), IntToU(4), IntToU(4), IntToU(4));
			case PineLog:
				return SideCase(side, IntToU(1), IntToU(1), IntToU(1), IntToU(1), IntToU(1), IntToU(1));
			case BirchLog:
				return SideCase(side, IntToU(1), IntToU(1), IntToU(1), IntToU(1), IntToU(1), IntToU(1));
			case Cactus:
				return SideCase(side, IntToU(1), IntToU(1), IntToU(1), IntToU(1), IntToU(1), IntToU(1));
			default:
				return BLOCK_SIZE_U * 0.5;
		}
	} else {
		switch(_block) {
			case Bedrock:
				return IntToU(1);
			case Cobblestone:
				return IntToU(0);
			case Dirt:
				return IntToU(2);
			case Planks:
				return IntToU(4);
			case Sand:
				return IntToU(2);
			case Stone:
				return IntToU(1);
			case OreCoal:
				return IntToU(2);
			case OreDiamond:
				return IntToU(2);
			case OreGold:
				return IntToU(0);
			case OreIron:
				return IntToU(1);
			case OreRedstone:
				return IntToU(3);
			case Leaves:
				return IntToU(4);
			case Water:
				return IntToU(0);
			case Ice:
				return IntToU(3);
			case RedFlower:
				return IntToU(12);
			case YellowFlower:
				return IntToU(13);
			case BlueFlower:
				return IntToU(1);
			case RedMushroom:
				return IntToU(12);
			case BrownMushroom:
				return IntToU(13);
			case PineLeaves:
				return IntToU(5);
			case BirchLeaves:
				return IntToU(20);
			case IronBlock:
				return IntToU(6);
			case GoldBlock:
				return IntToU(7);
			case DiamondBlock:
				return IntToU(8);
			case RedstoneBlock:
				return IntToU(10);
			case EmeraldBlock:
				return IntToU(9);
			case Glass:
				return IntToU(1);
			case DeadShrub:
				return IntToU(7);
			case HighGrass:
				return IntToU(8);
			default:
				return BLOCK_SIZE_U * 0.5;
		};
	}
}

float Block::GetV(Side side) const {
	if (IsMultisided()) {
		switch(_block) {
			case CraftingTable:
				return SideCase(side, IntToV(11), IntToV(4), IntToV(11), IntToV(11), IntToV(12), IntToV(12));
			case Grass:
				return SideCase(side, IntToV(0), IntToV(2), IntToV(3), IntToV(3), IntToV(3), IntToV(3));
			case Log:
				return SideCase(side, IntToV(5), IntToV(5), IntToV(4), IntToV(4), IntToV(4), IntToV(4));
			case SnowGrass:
				return SideCase(side, IntToV(2), IntToV(2), IntToV(4), IntToV(4), IntToV(4), IntToV(4));
			case PineLog:
				return SideCase(side, IntToV(1), IntToV(1), IntToV(1), IntToV(1), IntToV(1), IntToV(1));
			case BirchLog:
				return SideCase(side, IntToV(1), IntToV(1), IntToV(1), IntToV(1), IntToV(1), IntToV(1));
			case Cactus:
				return SideCase(side, IntToV(1), IntToV(1), IntToV(1), IntToV(1), IntToV(1), IntToV(1));
			default:
				return BLOCK_SIZE_V * 0.5;
		}
	} else {
		switch(_block) {
			case Bedrock:
				return IntToV(1);
			case Cobblestone:
				return IntToV(0);
			case Dirt:
				return IntToV(2);
			case Planks:
				return IntToV(4);
			case Sand:
				return IntToV(2);
			case Stone:
				return IntToV(1);
			case OreCoal:
				return IntToV(2);
			case OreDiamond:
				return IntToV(2);
			case OreGold:
				return IntToV(0);
			case OreIron:
				return IntToV(1);
			case OreRedstone:
				return IntToV(3);
			case Leaves:
				return IntToV(4);
			case Water:
				return IntToV(0);
			case Ice:
				return IntToV(3);
			case RedFlower:
				return IntToV(12);
			case YellowFlower:
				return IntToV(13);
			case BlueFlower:
				return IntToV(1);
			case RedMushroom:
				return IntToV(12);
			case BrownMushroom:
				return IntToV(13);
			case PineLeaves:
				return IntToV(5);
			case BirchLeaves:
				return IntToV(20);
			case IronBlock:
				return IntToV(6);
			case GoldBlock:
				return IntToV(7);
			case DiamondBlock:
				return IntToV(8);
			case RedstoneBlock:
				return IntToV(10);
			case EmeraldBlock:
				return IntToV(9);
			case Glass:
				return IntToV(1);
			case DeadShrub:
				return IntToV(7);
			case HighGrass:
				return IntToV(8);
			default:
				return BLOCK_SIZE_V * 0.5;
		};
	}
}