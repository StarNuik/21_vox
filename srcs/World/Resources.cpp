#include "World/Resources.h"

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

Texture* ResourceLoader::GetTexture(std::string name) {
	return _textures[name];
}

Geometry* ResourceLoader::GetGeometry(std::string name) {
	return _geometries[name];
}

Block* ResourceLoader::GetBlock(uint type) {
	return _blocks[type];
}

void ResourceLoader::LoadShaders() {
	_shaders["Base"] = new Shader("./resources/Shaders/base.vert", "./resources/Shaders/base.frag");
};

void ResourceLoader::LoadTextures() {
	_textures["Anime"] = new Texture("./resources/Textures/Anime.png");
	_textures["Bedrock"] = new Texture("./resources/Textures/Bedrock.png");
	_textures["Cobblestone"] = new Texture("./resources/Textures/Cobblestone.png");
	_textures["Dirt"] = new Texture("./resources/Textures/Dirt.png");
	_textures["Planks"] = new Texture("./resources/Textures/Planks.png");
	_textures["Sand"] = new Texture("./resources/Textures/Sand.png");
	_textures["Stone"] = new Texture("./resources/Textures/Stone.png");
};

void ResourceLoader::LoadGeometries() {
	_geometries["Box"] = new Geometry("./resources/Models/Box.obj");
};

void ResourceLoader::LoadBlocks() {
	_blocks[BLOCK_TYPE::AIR] = nullptr;
	_blocks[BLOCK_TYPE::BEDROCK] = new Block(_textures["Bedrock"]);
	_blocks[BLOCK_TYPE::COBBLESTONE] = new Block(_textures["Cobblestone"]);
	_blocks[BLOCK_TYPE::DIRT] = new Block(_textures["Dirt"]);
	_blocks[BLOCK_TYPE::PLANKS] = new Block(_textures["Planks"]);
	_blocks[BLOCK_TYPE::SAND] = new Block(_textures["Sand"]);
	_blocks[BLOCK_TYPE::STONE] = new Block(_textures["Stone"]);
};