#pragma once

#include <functional>
#include <glm/glm.hpp>

#include "Types.h"

#define __BLOCK_TYPE uint16

#define SHEET_WIDTH (384.0)
#define SHEET_HEIGHT (1312.0)
#define BLOCK_PIXELS (16)
#define BLOCK_SIZE_U ((double)(BLOCK_PIXELS) / SHEET_WIDTH)
#define BLOCK_SIZE_V ((double)(BLOCK_PIXELS) / SHEET_HEIGHT)

class Block
{
public:
	enum _Type : __BLOCK_TYPE
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
		GrassForSwamp,
		Leaves,
		Log,
		SnowGrass,
		Water,
		WaterForSwamp,
		Ice,
		RedFlower,
		YellowFlower,
		BlueFlower,
		RedMushroom,
		BrownMushroom,
		PineLog,
		BirchLog,
		PineLeaves,
		BirchLeaves,
		IronBlock,
		GoldBlock,
		DiamondBlock,
		RedstoneBlock,
		EmeraldBlock,
		Cactus,
		Glass,
		DeadShrub,
		HighGrass,
		Last = HighGrass,
		First = Bedrock,
		Size = Last + 1,
		NoChunk = (__BLOCK_TYPE)(-1),
		Sun = (__BLOCK_TYPE)(-2),
		Moon = (__BLOCK_TYPE)(-3),
	};
	enum Side : uint8 {
		Top = 0,
		Bottom,
		Right,
		Left,
		Front,
		Back
	};

	Block() = default;
	Block(_Type block) {_block = block;};
	Block(int num) {_block = _Type(num);};
	bool operator==(Block a) const {return _block == a._block;};
	bool operator!=(Block a) const {return _block != a._block;};
	bool operator==(_Type a) const {return _block == a;};
	bool operator!=(_Type a) const {return _block != a;};
	bool operator<(_Type a) const {return _block < a;};
	bool operator<=(_Type a) const {return _block <= a;};
	bool operator>(_Type a) const {return _block > a;};
	bool operator>=(_Type a) const {return _block >= a;};
	void operator++(int) {_block = _Type(_block + 1);};
	void operator--(int) {_block = _Type(_block - 1);};
	// Block operator+(int a) {return Block(_Type(_block + a));};
	explicit operator int() const {return _block;};
	explicit operator uint() const {return _block;};
	operator _Type() const {return _block;};
	explicit operator bool() = delete;
	bool IsTransparent() const;
	bool IsMultisided() const;
	bool IsFlower() const;
	bool IsWalkthrough() const;
	bool IsWater() const;
	glm::vec2 GetUV(Side) const;
	float GetU(Side) const;
	float GetV(Side) const;
private:
	_Type _block;
};

namespace std {
	template <>
	struct hash<Block> {
		std::size_t operator()(const Block& k) const {return (hash<__BLOCK_TYPE>()((uint)k));};
	};
}