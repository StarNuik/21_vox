#pragma once

#include <glm/glm.hpp>
#include "Types.h"
#include "World/Block.h"
#include "Engine/Entity.h"
#include <string>
#include <vector>
// #include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include "Animation/AnimationSkeletonNode.h"

#define ANIM_SHADER (std::string("Shadows"))
#define ANIM_MATERIAL (Block::Grass)
#define ANIM_GEOMETRY (std::string("BoxC"))
#define LIMB_WIDTH (0.5f)
// #define ANIM_GEOMETRY (std::string("BoxC"))

// #define MAX_BONE_COUNT (256)

class AnimationModel : public Entity
{
public:
	AnimationModel(Game* game, std::string path);
	void AddOverlayMatrix(std::string key, glm::mat4 matrix);
	void Update(float delta);
	void MuteBoneModel(std::string key);
private:
	void LogSceneTree(const aiScene* scene, aiNode* node, uint offset);
	void LogSceneInfo(const aiScene* scene, std::string path);
	// void PrepareBoneBuffer();

	// uint _ubo;

	Game* _game;
	AnimationSkeletonNode* _skeletonRoot;
	AnimationClip* _clip;

	bool _isPaused;
	float _animationTime;
};