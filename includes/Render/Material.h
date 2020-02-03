#pragma once

#include "Types.h"
#include "Render/Texture.h"

class Material {
public:
	Material(std::string dirPath, float shininess, float emission);
	~Material();
	void Use(Shader*);
	uint GetId();
private:
	Texture _diffuseMap;
	Texture _normalMap;
	Texture _specularMap;
	float _shininess;
	float _emission;
	uint _id;
	static uint __nextId;
};
