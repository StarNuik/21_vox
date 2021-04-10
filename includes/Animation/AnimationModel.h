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

#define ANIM_SHADER (std::string("DirLight"))
#define ANIM_MATERIAL (Block::GoldBlock)
#define ANIM_GEOMETRY (std::string("BoxC"))

class AnimationModel : public Entity
{
public:
	AnimationModel(Game* game, std::string path);
	void Update(float delta);
private:
	void LogSceneTree(const aiScene* scene, aiNode* node, uint offset);
	void LogSceneInfo(const aiScene* scene, std::string path);

	Game* _game;
	AnimationSkeletonNode* _skeletonRoot;
	AnimationClip* _clip;

	float _startTime;
};