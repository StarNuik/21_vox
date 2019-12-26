#pragma once

#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include "Types.h"

#define SUN_DIFFUSE (.5f, .5f, .55f)
#define SUN_AMBIENT (.5f, .5f, .4f)
#define MOON_DIFFUSE (.15f, .15f, .2f)
#define MOON_AMBIENT (.05f, .05f, .1f)

class Skybox {
public:
	Skybox(Game*, Shader*, CubeMap* day, CubeMap* night);
	~Skybox();
	Shader* Use(Camera*, float lerpVal, float runtime);
	void ApplyDirLights(Shader*);
	void SetDirLights(float lerpVal, float runtime);
	ShadowRenderer* GetShadowRenderer();
private:
	CubeMap* _cubemap_day;
	CubeMap* _cubemap_night;
	Shader* _shader;
	Geometry* _geometry;
	// glm::vec3 _sunColor;
	// glm::vec3 _moonColor;
	DirLight* _sunLight;
	DirLight* _moonLight;
	ShadowRenderer* _shadows;
};