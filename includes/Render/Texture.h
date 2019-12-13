#pragma once

#include <string>

#include "Types.h"

class Texture {
public:
	Texture(std::string);
	~Texture();
	void Use();
private:
	uint _id;
};
