#pragma once

#include <unordered_map>
#include <string>

#include "Types.h"
#include "World/Block.h"
// #include "Render/Objects.h"
// #include "World/World.h"

// class VertexBuffers;

class ResourceLoader {
public:
	ResourceLoader(Game*);
	~ResourceLoader();
	Shader* GetShader(std::string);
	Texture* GetTexture(std::string);
	Geometry* GetGeometry(std::string);
	CubeMap* GetCubeMap(std::string);
	Skybox* GetSkybox();
	Material* GetMaterial(Block);
	AnimationClip* GetAnimationClip(std::string);
	// Block* GetBlock(BlockType);
private:
	Game* _game;
	void LoadShaders();
	void LoadGeometries();
	void LoadCubeMaps();
	void LoadMaterials();
	void LoadAnimationClips();
	std::unordered_map<std::string, Shader*> _shaders;
	std::unordered_map<std::string, Texture*> _textures;
	std::unordered_map<Block, Material*> _materials;
	std::unordered_map<std::string, Geometry*> _geometries;
	std::unordered_map<std::string, CubeMap*> _cubemaps;
	std::unordered_map<std::string, AnimationClip*> _animationClips;
	Skybox* _skybox;
	VertexBuffers _vbs();
};