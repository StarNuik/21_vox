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
#include "World/Block.h"

ResourceLoader::ResourceLoader(Game* game) {
	_game = game;
	VertexBuffers::Init();
	LoadShaders();
	LoadGeometries();
	LoadCubeMaps();
	LoadMaterials();
	// _skybox = new Skybox(_game, _shaders["Skybox"], _cubemaps["Skybox Day"], _cubemaps["Skybox Night"]);
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
Material* ResourceLoader::GetMaterial(Block block) {return _materials[block];};

void ResourceLoader::LoadShaders() {
	//*Blocks
	_shaders["Base"] = new Shader("./resources/Shaders/base.vert", "./resources/Shaders/base.frag");
	_shaders["DirLight"] = new Shader("./resources/Shaders/base.vert", "./resources/Shaders/dirLight.frag");
	_shaders["Phong"] = new Shader("./resources/Shaders/base.vert", "./resources/Shaders/phong.frag");
	_shaders["Normals"] = new Shader("./resources/Shaders/base.vert", "./resources/Shaders/normals.frag");
	_shaders["Shadows"] = new Shader("./resources/Shaders/shadows.vert", "./resources/Shaders/shadows.frag");

	//* Post-Processing
	_shaders["Post Base"] = new Shader("./resources/Shaders/postBase.vert", "./resources/Shaders/postBase.frag");
	_shaders["Post HDR Basic"] = new Shader("./resources/Shaders/postBase.vert", "./resources/Shaders/postHdr0.frag");

	//* Utilities
	_shaders["Skybox"] = new Shader("./resources/Shaders/skybox.vert", "./resources/Shaders/skybox.frag");
	_shaders["Skybox Sun"] = new Shader("./resources/Shaders/sun.vert", "./resources/Shaders/sun.frag");
	_shaders["Shadow Renderer"] = new Shader("./resources/Shaders/shadowRenderer.vert", "./resources/Shaders/shadowRenderer.frag");
	// _shaders["Test"] = new Shader("./resources/Shaders/base.vert", "./resources/Shaders/test.frag");
	// _shaders["ShadowRenderer"] = new Shader("./resources/Shaders/shadows.vert", "./resources/Shaders/shadows.frag");
};

void ResourceLoader::LoadGeometries() {
	_geometries["Box"] = new Geometry("./resources/Models/Box.obj");
	_geometries["BoxC"] = new Geometry("./resources/Models/Cube_Centered.obj");
	_geometries["Monkey"] = new Geometry("./resources/Models/Monkey.obj");
	_geometries["Screen Quad"] = new Geometry("./resources/Models/ScreenQuad.obj");
	_geometries["Sun"] = new Geometry("./resources/Models/Sun.obj");
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
	_cubemaps["Day"] = new CubeMap(pathsDay);
	_cubemaps["Night"] = new CubeMap(pathsNight);
};

void ResourceLoader::LoadMaterials() {
	_materials[Block::Bedrock] = new Material("./resources/Textures/Bedrock/Bedrock", 1.f, 0.f);
	_materials[Block::Cobblestone] = new Material("./resources/Textures/Cobblestone/Cobblestone", 1.f, 0.f);
	_materials[Block::Dirt] = new Material("./resources/Textures/Dirt/Dirt", 1.f, 0.f);
	_materials[Block::Planks] = new Material("./resources/Textures/Planks/Planks", 1.f, 0.f);
	_materials[Block::Sand] = new Material("./resources/Textures/Sand/Sand", 1.f, 0.f);
	_materials[Block::Stone] = new Material("./resources/Textures/Stone/Stone", 1.f, 0.f);
	_materials[Block::OreCoal] = new Material("./resources/Textures/CoalOre/CoalOre", 1.f, 0.f);
	_materials[Block::OreDiamond] = new Material("./resources/Textures/DiamondOre/DiamondOre", 1.f, 0.f);
	_materials[Block::OreGold] = new Material("./resources/Textures/GoldOre/GoldOre", 32.f, 0.f);
	_materials[Block::OreIron] = new Material("./resources/Textures/IronOre/IronOre", 1.f, 0.f);
	_materials[Block::OreRedstone] = new Material("./resources/Textures/RedstoneOre/RedstoneOre", 1.f, 0.f);
	_materials[Block::CraftingTable] = new Material("./resources/Textures/CraftingTable/CraftingTable", 1.f, 0.f);
	_materials[Block::Grass] = new Material("./resources/Textures/Grass/Grass", 32.f, 0.f);
	_materials[Block::Leaves] = new Material("./resources/Textures/Leaves/Leaves", 1.f, 0.f);
	_materials[Block::Log] = new Material("./resources/Textures/Log/Log", 1.f, 0.f);
	_materials[Block::Dandelion] = new Material("./resources/Textures/Dandelion/Dandelion", 1.f, 0.f);
	_materials[Block::SnowGrass] = new Material("./resources/Textures/SnowGrass/SnowGrass", 4.f, 0.f);
	_materials[Block::Water] = new Material("./resources/Textures/Water/Water", 32.f, 0.f);
	_materials[Block::Ice] = new Material("./resources/Textures/Ice/Ice", 4.f, 0.f);
	_materials[Block::Sun] = new Material("./resources/Textures/Sun/Sun", 0.f, 10.f);
	_materials[Block::Moon] = new Material("./resources/Textures/Moon/Moon", 0.f, 0.3f);
};