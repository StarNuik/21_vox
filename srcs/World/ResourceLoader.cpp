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
#include "Animation/AnimationClip.h"

ResourceLoader::ResourceLoader(Game* game) {
	_game = game;
	VertexBuffers::Init();
	LoadShaders();
	LoadGeometries();
	LoadCubeMaps();
	LoadMaterials();
	LoadAnimationClips();
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
	for (auto pair : _animationClips) {
		delete pair.second;
	}
	delete _skybox;
	VertexBuffers::Destroy();
};

Shader* ResourceLoader::GetShader(std::string name) {return _shaders[name];};
Texture* ResourceLoader::GetTexture(std::string name) {return _textures[name];};
Geometry* ResourceLoader::GetGeometry(std::string name) {return _geometries[name];};
CubeMap* ResourceLoader::GetCubeMap(std::string name) {return _cubemaps[name];};
AnimationClip* ResourceLoader::GetAnimationClip(std::string name) { return _animationClips[name];};
Skybox* ResourceLoader::GetSkybox() {return _skybox;};
Material* ResourceLoader::GetMaterial(Block block) {return _materials[block];};

void ResourceLoader::LoadShaders() {
	//*Blocks
	_shaders["Base"] = new Shader("./resources/Shaders/Vertex/base.vert", "./resources/Shaders/Fragment/base.frag");
	_shaders["DirLight"] = new Shader("./resources/Shaders/Vertex/base.vert", "./resources/Shaders/Fragment/dirLight.frag");
	_shaders["Phong"] = new Shader("./resources/Shaders/Vertex/base.vert", "./resources/Shaders/Fragment/phong.frag");
	_shaders["Normals"] = new Shader("./resources/Shaders/Vertex/base.vert", "./resources/Shaders/Fragment/normals.frag");
	_shaders["Shadows"] = new Shader("./resources/Shaders/Vertex/shadows.vert", "./resources/Shaders/Fragment/shadows.frag");
	_shaders["Bloom"] = new Shader("./resources/Shaders/Vertex/shadows.vert", "./resources/Shaders/Fragment/bloom.frag");
	_shaders["Flower"] = new Shader("./resources/Shaders/Vertex/shadows.vert", "./resources/Shaders/Fragment/flower.frag");
	_shaders["SkinnedMesh"] = new Shader("./resources/Shaders/Vertex/skinned.vert", "./resources/Shaders/Fragment/shadows.frag");

	//* Post-Processing
	_shaders["Post Main: Bloom & HDR Aces approximate & Gamma"] = new Shader("./resources/Shaders/Vertex/post.vert", "./resources/Shaders/Post/acesaBloom.frag");
	_shaders["Post None"] = new Shader("./resources/Shaders/Vertex/post.vert", "./resources/Shaders/Post/none.frag");
	_shaders["Post HDR Reinhard only"] = new Shader("./resources/Shaders/Vertex/post.vert", "./resources/Shaders/Post/hdrReinhard.frag");
	_shaders["Post Gamma correction"] = new Shader("./resources/Shaders/Vertex/post.vert", "./resources/Shaders/Post/gamma.frag");
	_shaders["Post HDR Reinhard & Gamma"] = new Shader("./resources/Shaders/Vertex/post.vert", "./resources/Shaders/Post/hdrReinhardGamma.frag");
	_shaders["Post HDR Filmic & Gamma"] = new Shader("./resources/Shaders/Vertex/post.vert", "./resources/Shaders/Post/hdrFilmicGamma.frag");
	_shaders["Post HDR Uncharted & Gamma"] = new Shader("./resources/Shaders/Vertex/post.vert", "./resources/Shaders/Post/hdrUnchartedGamma.frag");
	_shaders["Post HDR Aces approximate & Gamma"] = new Shader("./resources/Shaders/Vertex/post.vert", "./resources/Shaders/Post/acesa.frag");
	_shaders["Post Acesa & Gamma & Purple"] = new Shader("./resources/Shaders/Vertex/post.vert", "./resources/Shaders/Post/acesaPurple.frag");
	_shaders["Post Acesa & Gamma & Inverse"] = new Shader("./resources/Shaders/Vertex/post.vert", "./resources/Shaders/Post/acesaInverse.frag");
	_shaders["Post Acesa & Gamma & Grayscale"] = new Shader("./resources/Shaders/Vertex/post.vert", "./resources/Shaders/Post/acesaGrayscale.frag");
	_shaders["Post Acesa & Gamma & Gaussian Blur 3x3"] = new Shader("./resources/Shaders/Vertex/post.vert", "./resources/Shaders/Post/acesaBlur3x3.frag");
	_shaders["Post Acesa & Gamma & Gaussian Blur 5x5"] = new Shader("./resources/Shaders/Vertex/post.vert", "./resources/Shaders/Post/acesaBlur5x5.frag");
	_shaders["Post Acesa & Gamma & Sharpen"] = new Shader("./resources/Shaders/Vertex/post.vert", "./resources/Shaders/Post/acesaSharpen.frag");
	_shaders["Post Acesa & Gamma & Outline"] = new Shader("./resources/Shaders/Vertex/post.vert", "./resources/Shaders/Post/acesaOutline.frag");
	_shaders["Post Acesa & Gamma & Emboss"] = new Shader("./resources/Shaders/Vertex/post.vert", "./resources/Shaders/Post/acesaEmboss.frag");
	_shaders["Post Pre Bloom"] = new Shader("./resources/Shaders/Vertex/post.vert", "./resources/Shaders/Post/preBloom.frag");
	_shaders["Post Gaussian Blur 5x5"] = new Shader("./resources/Shaders/Vertex/post.vert", "./resources/Shaders/Post/blur5x5.frag");
	_shaders["Post Main & Outline"] = new Shader("./resources/Shaders/Vertex/post.vert", "./resources/Shaders/Post/mainOutline.frag");
	_shaders["Post Main & Hueshift"] = new Shader("./resources/Shaders/Vertex/post.vert", "./resources/Shaders/Post/mainHueshift.frag");
	_shaders["Post Main & Outline Moving"] = new Shader("./resources/Shaders/Vertex/post.vert", "./resources/Shaders/Post/mainMoving.frag");

	//* Utilities
	_shaders["Skybox"] = new Shader("./resources/Shaders/Vertex/skybox.vert", "./resources/Shaders/Fragment/skybox.frag");
	_shaders["Skybox Sun"] = new Shader("./resources/Shaders/Vertex/sun.vert", "./resources/Shaders/Fragment/sun.frag");
	_shaders["Skybox Bloom"] = new Shader("./resources/Shaders/Vertex/skybox.vert", "./resources/Shaders/Fragment/skyboxBloom.frag");
	_shaders["Skybox Sun Bloom"] = new Shader("./resources/Shaders/Vertex/sun.vert", "./resources/Shaders/Fragment/sunBloom.frag");
	_shaders["Shadow Renderer"] = new Shader("./resources/Shaders/Vertex/shadowRenderer.vert", "./resources/Shaders/Fragment/shadowRenderer.frag");
	// _shaders["Test"] = new Shader("./resources/Shaders/base.vert", "./resources/Shaders/test.frag");
	// _shaders["ShadowRenderer"] = new Shader("./resources/Shaders/shadows.vert", "./resources/Shaders/shadows.frag");
};

void ResourceLoader::LoadGeometries() {
	// _geometries["Box"] = new Geometry("./resources/Models/Box.obj");
	_geometries["BoxC"] = new Geometry("./resources/Models/Cube_Centered.obj");
	_geometries["BoxOffset"] = new Geometry("./resources/Models/BoxOffset.obj");
	_geometries["-BoxOffset"] = new Geometry("./resources/Models/-BoxOffset.obj");
	// _geometries["Monkey"] = new Geometry("./resources/Models/Monkey.obj");
	_geometries["Screen Quad"] = new Geometry("./resources/Models/ScreenQuad.obj");
	_geometries["Sun"] = new Geometry("./resources/Models/Sun.obj");
	_geometries["Cactus"] = new Geometry("./resources/Models/Cactus.obj");
};

void ResourceLoader::LoadCubeMaps() {
	std::string pathsDay[6] = {
		"./resources/CubeMaps/day_top.png",
		"./resources/CubeMaps/day_top.png",
		"./resources/CubeMaps/day_top.png",
		"./resources/CubeMaps/day_top.png",
		"./resources/CubeMaps/day_top.png",
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
	_materials[Block::GrassForSwamp] = new Material("./resources/Textures/GrassForSwamp/GrassForSwamp", 32.f, 0.f);
	_materials[Block::Leaves] = new Material("./resources/Textures/Leaves/Leaves", 1.f, 0.f);
	_materials[Block::Log] = new Material("./resources/Textures/Log/Log", 1.f, 0.f);
	_materials[Block::SnowGrass] = new Material("./resources/Textures/SnowGrass/SnowGrass", 4.f, 0.f);
	_materials[Block::Water] = new Material("./resources/Textures/Water/Water", 32.f, 0.f);
	_materials[Block::WaterForSwamp] = new Material("./resources/Textures/WaterForSwamp/WaterForSwamp", 32.f, 0.f);
	_materials[Block::Ice] = new Material("./resources/Textures/Ice/Ice", 4.f, 0.f);
	_materials[Block::RedFlower] = new Material("./resources/Textures/RedFlower/RedFlower", 1.f, 0.f);
	_materials[Block::YellowFlower] = new Material("./resources/Textures/YellowFlower/YellowFlower", 1.f, 0.f);
	_materials[Block::BlueFlower] = new Material("./resources/Textures/BlueFlower/BlueFlower", 1.f, 0.f);
	_materials[Block::RedMushroom] = new Material("./resources/Textures/RedMushroom/RedMushroom", 1.f, 0.f);
	_materials[Block::BrownMushroom] = new Material("./resources/Textures/BrownMushroom/BrownMushroom", 1.f, 0.f);
	_materials[Block::PineLog] = new Material("./resources/Textures/PineLog/PineLog", 1.f, 0.f);
	_materials[Block::BirchLog] = new Material("./resources/Textures/BirchLog/BirchLog", 1.f, 0.f);
	_materials[Block::PineLeaves] = new Material("./resources/Textures/PineLeaves/PineLeaves", 1.f, 0.f);
	_materials[Block::BirchLeaves] = new Material("./resources/Textures/BirchLeaves/BirchLeaves", 1.f, 0.f);
	_materials[Block::IronBlock] = new Material("./resources/Textures/IronBlock/IronBlock", 1.f, 0.f);
	_materials[Block::GoldBlock] = new Material("./resources/Textures/GoldBlock/GoldBlock", 1.f, 0.f);
	_materials[Block::DiamondBlock] = new Material("./resources/Textures/DiamondBlock/DiamondBlock", 1.f, 0.f);
	_materials[Block::RedstoneBlock] = new Material("./resources/Textures/RedstoneBlock/RedstoneBlock", 1.f, 0.f);
	_materials[Block::EmeraldBlock] = new Material("./resources/Textures/EmeraldBlock/EmeraldBlock", 1.f, 0.f);
	_materials[Block::Cactus] = new Material("./resources/Textures/Cactus/Cactus", 1.f, 0.f);
	_materials[Block::Glass] = new Material("./resources/Textures/Glass/Glass", 1.f, 0.f);
	_materials[Block::DeadShrub] = new Material("./resources/Textures/DeadShrub/DeadShrub", 1.f, 0.f);
	_materials[Block::HighGrass] = new Material("./resources/Textures/HighGrass/HighGrass", 1.f, 0.f);
	_materials[Block::Sun] = new Material("./resources/Textures/Sun/Sun", 0.f, 1000.f);
	_materials[Block::Moon] = new Material("./resources/Textures/Moon/Moon", 0.f, .5f);
};

void ResourceLoader::LoadAnimationClips() {
	_animationClips["Dance1"] = new AnimationClip("./resources/Animations/Hip Hop Dancing");
	_animationClips["Dance2"] = new AnimationClip("./resources/Animations/Hip Hop Dancing 2");
	_animationClips["Dance3"] = new AnimationClip("./resources/Animations/Breakdance Freezes");
};