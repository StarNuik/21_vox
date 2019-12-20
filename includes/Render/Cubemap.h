#pragma once

#include <string>

#include "Types.h"

class CubeMap {
public:
	CubeMap(std::string[6]);
	~CubeMap();
	void Use();
private:
	uint _id;
};