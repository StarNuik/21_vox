#include "Types.h"
#include "World/ResourceLoader.h"
#include "Render/VertexBuffers.h"
#include "Render/Shader.h"
#include "Render/Texture.h"
#include "Render/Geometry.h"
#include "Render/Cubemap.h"
// #include "World/World.h"

ResourceLoader::ResourceLoader() {
	VertexBuffers::Init();
	LoadShaders();
	LoadTextures();
	LoadGeometries();
	LoadCubeMaps();
};

ResourceLoader::~ResourceLoader() {
	for (auto pair : _shaders) {
		delete pair.second;
	}
	for (auto pair : _textures) {
		delete pair.second;
	}
	for (auto pair : _geometries) {
		delete pair.second;
	}
	for (auto pair : _cubemaps) {
		delete pair.second;
	}
	VertexBuffers::Destroy();
};

Shader* ResourceLoader::GetShader(std::string name) {return _shaders[name];}
Texture* ResourceLoader::GetTexture(BlockType type) {return _textures[(int)type];}
Geometry* ResourceLoader::GetGeometry(std::string name) {return _geometries[name];}
CubeMap* ResourceLoader::GetCubeMap(std::string name) {return _cubemaps[name];};

void ResourceLoader::LoadShaders() {
	_shaders["Base"] = new Shader("./resources/Shaders/base.vert", "./resources/Shaders/base.frag");
};

void ResourceLoader::LoadTextures() {
	_textures[(int)BlockType::Air] = new Texture("./resources/Textures/Anime.png");
	_textures[(int)BlockType::Bedrock] = new Texture("./resources/Textures/Bedrock.png");
	_textures[(int)BlockType::Cobblestone] = new Texture("./resources/Textures/Cobblestone.png");
	_textures[(int)BlockType::Dirt] = new Texture("./resources/Textures/Dirt.png");
	_textures[(int)BlockType::Planks] = new Texture("./resources/Textures/Planks.png");
	_textures[(int)BlockType::Sand] = new Texture("./resources/Textures/Sand.png");
	_textures[(int)BlockType::Stone] = new Texture("./resources/Textures/Stone.png");
	_textures[(int)BlockType::OreCoal] = new Texture("./resources/Textures/CoalOre.png");
	_textures[(int)BlockType::OreDiamond] = new Texture("./resources/Textures/DiamondOre.png");
	_textures[(int)BlockType::OreGold] = new Texture("./resources/Textures/GoldOre.png");
	_textures[(int)BlockType::OreIron] = new Texture("./resources/Textures/IronOre.png");
	_textures[(int)BlockType::OreRedstone] = new Texture("./resources/Textures/RedstoneOre.png");
	_textures[(int)BlockType::CraftingTable] = new Texture("./resources/Textures/CraftingTable.png");
	_textures[(int)BlockType::Grass] = new Texture("./resources/Textures/Grass.png");
	_textures[(int)BlockType::Leaves] = new Texture("./resources/Textures/Leaves.png");
	_textures[(int)BlockType::Log] = new Texture("./resources/Textures/Log.png");
	_textures[(int)BlockType::Dandelion] = new Texture("./resources/Textures/Dandelion.png");
};

void ResourceLoader::LoadGeometries() {
	// _geometries["Box"] = new Geometry("./resources/Models/Box.obj");
	// _geometries["Monkey"] = new Geometry("./resources/Models/Monkey.obj");
};

void ResourceLoader::LoadCubeMaps() {
	std::string paths[6] = {"", "", "", "", "", ""};
	_cubemaps["Skybox_Day"] = new CubeMap(paths);
};