#pragma once

#include <string>
#include <vector>

#include "Types.h"

class Geometry {
public:
	Geometry(std::string);
	Geometry(std::vector<float>);
	~Geometry();
	uint GetPolygonCount();
	void Use();
	static std::vector<float> ReadGeometry(std::string);
private:
	void Init(std::vector<float>);
	uint _polygonCount, _vbo, _vao;
};