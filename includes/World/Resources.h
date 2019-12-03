#pragma once

#include <unordered_map>
#include <string>
#include "Render/Objects.h"


class ResourceLoader {
public:
	ResourceLoader();
	~ResourceLoader();
	Shader* GetShader(std::string);
	Texture* GetTexture(std::string);
	Geometry* GetGeometry(std::string);
private:
	void LoadShaders();
	void LoadTextures();
	void LoadGeometries();
	std::unordered_map<std::string, Shader*> _shaders;
	std::unordered_map<std::string, Texture*> _textures;
	std::unordered_map<std::string, Geometry*> _geometries;
};