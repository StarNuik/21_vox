#include "Types.h"
#include "World/Resources.h"
#include "World/World.h"

ResourceLoader::ResourceLoader() {
	LoadShaders();
	LoadTextures();
	LoadGeometries();
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
};

Shader* ResourceLoader::GetShader(std::string name) {
	return _shaders[name];
}

Texture* ResourceLoader::GetTexture(BlockType type) {
	return _textures[(int)type];
}

Geometry* ResourceLoader::GetGeometry(std::string name) {
	return _geometries[name];
}

Block* ResourceLoader::GetBlock(BlockType type) {
	return _blocks[(int)type];
}

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
};

void ResourceLoader::LoadGeometries() {
	_geometries["Box"] = new Geometry("./resources/Models/Box.obj");
	_geometries["Monkey"] = new Geometry("./resources/Models/Monkey.obj");
};

void ResourceLoader::LoadBlocks() {
	// _blocks[(int)BlockType::Air] = nullptr;
	// _blocks[(int)BlockType::Bedrock] = new Block(_textures["Bedrock"]);
	// _blocks[(int)BlockType::Cobblestone] = new Block(_textures["Cobblestone"]);
	// _blocks[(int)BlockType::Dirt] = new Block(_textures["Dirt"]);
	// _blocks[(int)BlockType::Planks] = new Block(_textures["Planks"]);
	// _blocks[(int)BlockType::Sand] = new Block(_textures["Sand"]);
	// _blocks[(int)BlockType::Stone] = new Block(_textures["Stone"]);
};