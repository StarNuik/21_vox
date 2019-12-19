#pragma once

#include <string>

#include "Types.h"

class Skybox {
public:
	Skybox(Shader*, CubeMap* day, CubeMap* night);
	~Skybox();
	Shader* Use(Camera*, float lerpVal);
private:
	CubeMap* _cubemap_day;
	CubeMap* _cubemap_night;
	Shader* _shader;
	Geometry* _geometry;
};