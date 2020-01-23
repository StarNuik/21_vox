#pragma once

#include "Types.h"
#include "Render/Texture.h"

class Material {
public:
	Material(std::string dirPath, float);
	~Material();
	void Use(Shader*);
	uint GetId();
private:
	Texture _diffuseMap;
	Texture _normalMap;
	Texture _specularMap;
	float _shininess;
	uint _id;
	static uint __nextId;
};
