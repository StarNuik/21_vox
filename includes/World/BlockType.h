#pragma once

#include "Types.h"

class BlockType
{
public:
	enum _Type : uint8
	{
		Air = 0,
		Bedrock,
		Cobblestone,
		Dirt,
		Planks,
		Sand,
		Stone,
		OreCoal,
		OreDiamond,
		OreGold,
		OreIron,
		OreRedstone,
		CraftingTable,
		Grass,
		Leaves,
		Log,
		Dandelion,
		SnowGrass,
		Water,
		Ice,
		First = Air,
		Last = Ice,
		Size = Last + 1
	};

	BlockType() = default;
	BlockType(_Type block) : _block(block) {};
	BlockType(uint num) : _block(_Type(num)) {};
	bool operator==(BlockType a) const {return _block == a._block;};
	bool operator!=(BlockType a) const {return _block != a._block;};
	bool operator==(_Type a) const {return _block == a;};
	bool operator!=(_Type a) const {return _block != a;};
	explicit operator int() const {return _block;};
	explicit operator uint() const {return _block;};
	operator _Type() const {return _block;};
	explicit operator bool() = delete; 
	bool IsYellow() const {return _block == OreGold;};
private:
	_Type _block;
};
