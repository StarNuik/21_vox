#include "Types.h"
#include "World/ResourceLoader.h"
#include "Render/VertexBuffers.h"
#include "Render/Shader.h"
#include "Render/Texture.h"
#include "Render/Geometry.h"
#include "Render/Cubemap.h"
#include "Render/Skybox.h"
#include "Render/Material.h"
#include "Utilities/Log.h"
// #include "World/World.h"

ResourceLoader::ResourceLoader() {
	VertexBuffers::Init();
	LoadShaders();
	LoadTextures();
	LoadGeometries();
	LoadCubeMaps();
	LoadMaterials();
	_skybox = new Skybox(_shaders["Skybox"], _cubemaps["Skybox Day"], _cubemaps["Skybox Night"]);
	Log::Success("[ResourceLoader::ResourceLoader]\nResources loaded.");
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
	delete _skybox;
	VertexBuffers::Destroy();
};

Shader* ResourceLoader::GetShader(std::string name) {return _shaders[name];};
Texture* ResourceLoader::GetTexture(std::string name) {return _textures[name];};
Geometry* ResourceLoader::GetGeometry(std::string name) {return _geometries[name];};
CubeMap* ResourceLoader::GetCubeMap(std::string name) {return _cubemaps[name];};
Skybox* ResourceLoader::GetSkybox() {return _skybox;};
Material* ResourceLoader::GetMaterial(BlockType type) {return _materials[(int)type];};

void ResourceLoader::LoadShaders() {
	_shaders["Base"] = new Shader("./resources/Shaders/base.vert", "./resources/Shaders/base.frag");
	_shaders["DirLight"] = new Shader("./resources/Shaders/base.vert", "./resources/Shaders/dirLight.frag");
	_shaders["Phong"] = new Shader("./resources/Shaders/base.vert", "./resources/Shaders/phong.frag");
	_shaders["Skybox"] = new Shader("./resources/Shaders/skybox.vert", "./resources/Shaders/skybox.frag");
};

void ResourceLoader::LoadTextures() {
	// _textures["Diffuse Bedrock"] = new Texture("./resources/Textures/Bedrock.png");
	// _textures["Diffuse Cobblestone"] = new Texture("./resources/Textures/Cobblestone.png");
	// _textures["Diffuse Dirt"] = new Texture("./resources/Textures/Dirt.png");
	// _textures["Diffuse Planks"] = new Texture("./resources/Textures/Planks.png");
	// _textures["Diffuse Sand"] = new Texture("./resources/Textures/Sand.png");
	// _textures["Diffuse Stone"] = new Texture("./resources/Textures/Stone.png");
	// _textures["Diffuse CoalOre"] = new Texture("./resources/Textures/CoalOre.png");
	// _textures["Diffuse DiamondOre"] = new Texture("./resources/Textures/DiamondOre.png");
	// _textures["Diffuse GoldOre"] = new Texture("./resources/Textures/GoldOre.png");
	// _textures["Diffuse IronOre"] = new Texture("./resources/Textures/IronOre.png");
	// _textures["Diffuse RedstoneOre"] = new Texture("./resources/Textures/RedstoneOre.png");
	// _textures["Diffuse CraftingTable"] = new Texture("./resources/Textures/CraftingTable.png");
	// _textures["Diffuse Grass"] = new Texture("./resources/Textures/Grass.png");
	// _textures["Diffuse Leaves"] = new Texture("./resources/Textures/Leaves.png");
	// _textures["Diffuse Log"] = new Texture("./resources/Textures/Log.png");
	// _textures["Diffuse Dandelion"] = new Texture("./resources/Textures/Dandelion.png");

	// _textures["Normal Bedrock"] = new Texture("./resources/Textures/Bedrock.png");
	// _textures["Normal Cobblestone"] = new Texture("./resources/Textures/Cobblestone.png");
	// _textures["Normal Dirt"] = new Texture("./resources/Textures/Dirt.png");
	// _textures["Normal Planks"] = new Texture("./resources/Textures/Planks.png");
	// _textures["Normal Sand"] = new Texture("./resources/Textures/Sand.png");
	// _textures["Normal Stone"] = new Texture("./resources/Textures/Stone.png");
	// _textures["Normal CoalOre"] = new Texture("./resources/Textures/CoalOre.png");
	// _textures["Normal DiamondOre"] = new Texture("./resources/Textures/DiamondOre.png");
	// _textures["Normal GoldOre"] = new Texture("./resources/Textures/GoldOre.png");
	// _textures["Normal IronOre"] = new Texture("./resources/Textures/IronOre.png");
	// _textures["Normal RedstoneOre"] = new Texture("./resources/Textures/RedstoneOre.png");
	// _textures["Normal CraftingTable"] = new Texture("./resources/Textures/CraftingTable.png");
	// _textures["Normal Grass"] = new Texture("./resources/Textures/Grass.png");
	// _textures["Normal Leaves"] = new Texture("./resources/Textures/Leaves.png");
	// _textures["Normal Log"] = new Texture("./resources/Textures/Log.png");
	// _textures["Normal Dandelion"] = new Texture("./resources/Textures/Dandelion.png");

	// _textures["Specular Bedrock"] = new Texture("./resources/Textures/Bedrock.png");
	// _textures["Specular Cobblestone"] = new Texture("./resources/Textures/Cobblestone.png");
	// _textures["Specular Dirt"] = new Texture("./resources/Textures/Dirt.png");
	// _textures["Specular Planks"] = new Texture("./resources/Textures/Planks.png");
	// _textures["Specular Sand"] = new Texture("./resources/Textures/Sand.png");
	// _textures["Specular Stone"] = new Texture("./resources/Textures/Stone.png");
	// _textures["Specular CoalOre"] = new Texture("./resources/Textures/CoalOre.png");
	// _textures["Specular DiamondOre"] = new Texture("./resources/Textures/DiamondOre.png");
	// _textures["Specular GoldOre"] = new Texture("./resources/Textures/GoldOre.png");
	// _textures["Specular IronOre"] = new Texture("./resources/Textures/IronOre.png");
	// _textures["Specular RedstoneOre"] = new Texture("./resources/Textures/RedstoneOre.png");
	// _textures["Specular CraftingTable"] = new Texture("./resources/Textures/CraftingTable.png");
	// _textures["Specular Grass"] = new Texture("./resources/Textures/Grass.png");
	// _textures["Specular Leaves"] = new Texture("./resources/Textures/Leaves.png");
	// _textures["Specular Log"] = new Texture("./resources/Textures/Log.png");
	// _textures["Specular Dandelion"] = new Texture("./resources/Textures/Dandelion.png");
};

void ResourceLoader::LoadGeometries() {
	// _geometries["Box"] = new Geometry("./resources/Models/Box.obj");
	// _geometries["Monkey"] = new Geometry("./resources/Models/Monkey.obj");
};

void ResourceLoader::LoadCubeMaps() {
	std::string pathsDay[6] = {
		"./resources/CubeMaps/day_top.png",
		"./resources/CubeMaps/day_top.png",
		"./resources/CubeMaps/day_top.png",
		"./resources/CubeMaps/day_top.png",
		"./resources/CubeMaps/day_sun.png",
		"./resources/CubeMaps/day_top.png"
	};
	std::string pathsNight[6] = {
		"./resources/CubeMaps/purplenebula_ft.tga",
		"./resources/CubeMaps/purplenebula_bk.tga",
		"./resources/CubeMaps/purplenebula_up.tga",
		"./resources/CubeMaps/purplenebula_dn.tga",
		"./resources/CubeMaps/purplenebula_rt.tga",
		"./resources/CubeMaps/purplenebula_lf.tga"
	};
	_cubemaps["Skybox Day"] = new CubeMap(pathsDay);
	_cubemaps["Skybox Night"] = new CubeMap(pathsNight);
};

void ResourceLoader::LoadMaterials() {
	_materials[(int)BlockType::Bedrock] = new Material("./resources/Textures/Bedrock/Bedrock", 1.f);
	_materials[(int)BlockType::Cobblestone] = new Material("./resources/Textures/Cobblestone/Cobblestone", 1.f);
	_materials[(int)BlockType::Dirt] = new Material("./resources/Textures/Dirt/Dirt", 1.f);
	_materials[(int)BlockType::Planks] = new Material("./resources/Textures/Planks/Planks", 1.f);
	_materials[(int)BlockType::Sand] = new Material("./resources/Textures/Sand/Sand", 1.f);
	_materials[(int)BlockType::Stone] = new Material("./resources/Textures/Stone/Stone", 1.f);
	_materials[(int)BlockType::OreCoal] = new Material("./resources/Textures/CoalOre/CoalOre", 1.f);
	_materials[(int)BlockType::OreDiamond] = new Material("./resources/Textures/DiamondOre/DiamondOre", 1.f);
	_materials[(int)BlockType::OreGold] = new Material("./resources/Textures/GoldOre/GoldOre", 32.f);
	_materials[(int)BlockType::OreIron] = new Material("./resources/Textures/IronOre/IronOre", 1.f);
	_materials[(int)BlockType::OreRedstone] = new Material("./resources/Textures/RedstoneOre/RedstoneOre", 1.f);
	_materials[(int)BlockType::CraftingTable] = new Material("./resources/Textures/CraftingTable/CraftingTable", 1.f);
	_materials[(int)BlockType::Grass] = new Material("./resources/Textures/Grass/Grass", 32.f);
	_materials[(int)BlockType::Leaves] = new Material("./resources/Textures/Leaves/Leaves", 1.f);
	_materials[(int)BlockType::Log] = new Material("./resources/Textures/Log/Log", 1.f);
	_materials[(int)BlockType::Dandelion] = new Material("./resources/Textures/Dandelion/Dandelion", 1.f);
	_materials[(int)BlockType::SnowGrass] = new Material("./resources/Textures/SnowGrass/SnowGrass", 4.f);
	_materials[(int)BlockType::Water] = new Material("./resources/Textures/Water/Water", 32.f);
	_materials[(int)BlockType::Ice] = new Material("./resources/Textures/Ice/Ice", 4.f);
};