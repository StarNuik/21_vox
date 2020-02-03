#pragma once

#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include "Types.h"

#define SUN_DIFFUSE (.9f, .9f, .85f)
#define SUN_AMBIENT (.25f, .25f, .25f)
#define MOON_DIFFUSE (.08f, .08f, .12f)
#define MOON_AMBIENT (.05f, .05f, .05f)

class Skybox {
public:
	Skybox(Game*, Shader*, CubeMap* day, CubeMap* night);
	// ~Skybox();
	Shader* Use(Camera*, float lerpVal, float runtime);
	void ApplyDirLights(Shader*);
	// void SetDirLights(float lerpVal, float runtime);
	// ShadowRenderer* GetShadowRenderer();
	void SetActiveCamera(Camera*);

	Skybox();
	~Skybox();
	void Init(Game*);
	void PrepareData(float sunAngle, float moonAngle, float sunVal, float moonVal);
	void Render();
	void Use();
private:
	//! Delete these
	// CubeMap* _cubemap_day;
	// CubeMap* _cubemap_night;
	// Geometry* _geometry;
	// ShadowRenderer* _shadows;

	Game* _game;
	Shader* _shader;
	CubeMap* _dayCubemap;
	CubeMap* _nightCubemap;
	Geometry* _skyGeometry;
	RenderModel* _sunModel;
	RenderModel* _moonModel;
	DirLight* _sunLight;
	DirLight* _moonLight;
	Camera* _activeCamera;

	//* Render data
	glm::mat4 _mvpSky;
	glm::mat4 _mvpMoon;
	glm::mat4 _projection;
	float _sunVal;
	bool _easySunApply;
	float _moonVal;
	bool _easyMoonApply;
};