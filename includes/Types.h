#pragma once

typedef unsigned int uint;
typedef unsigned long long uint64;
typedef unsigned int uint32;
typedef unsigned short uint16;
typedef unsigned char uint8;
typedef long long int64;
typedef int int32;
typedef short int16;
typedef char int8;

class RenderModel;
class Shader;
class Texture;
class Geometry;
class Camera;
class GLRenderer;
class Block;
class Shard;
class Chunk;
class World;
class Game;
class VertexBuffers;
class Player;
class ResourceLoader;

enum class BlockType {
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
	First = Air,
	Last = Dandelion,
	Size = Last + 1
};