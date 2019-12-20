#pragma once

#include <string>
#include <glm/gtc/quaternion.hpp>


#include "Types.h"

class Skybox {
public:
	Skybox(Shader*, CubeMap* day, CubeMap* night);
	~Skybox();
	Shader* Use(Camera*, float lerpVal, float runtime);
private:
	CubeMap* _cubemap_day;
	CubeMap* _cubemap_night;
	Shader* _shader;
	Geometry* _geometry;
};