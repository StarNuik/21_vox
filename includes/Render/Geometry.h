#pragma once

#include <string>
#include <vector>

#include "Types.h"

#define VERTICE_SIZE (8)

class Geometry {
public:
	Geometry(std::string);
	Geometry(std::vector<float>);
	~Geometry();
	uint GetPolygonCount();
	uint GetId();
	void Use();
	static std::vector<float> ReadGeometry(std::string);
private:
	void Init(std::vector<float>);
	uint _polygonCount, _vbo, _vao;
};
