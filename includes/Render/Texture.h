#pragma once

#include <string>

#include "Types.h"

class Texture {
public:
	Texture();
	~Texture();
	void Load(std::string);
	void Use();
	bool IsLoaded();
private:
	uint _id;
	bool _loaded;
};
