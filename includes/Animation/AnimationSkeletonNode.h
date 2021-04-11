#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <vector>
#include <assimp/scene.h>

#include "Types.h"

class AnimationSkeletonNode {
public:
	AnimationSkeletonNode(Game* game, aiNode* node, AnimationSkeletonNode* parent);
	void ApplyAnimation(AnimationClip* clip, float time);
	void ApplyOverlay(std::string key, glm::mat4 matrix);
	void OverrideModelScale(std::string key, glm::vec3 scale);
	void Mute(std::string key);
private:
	glm::mat4 CalculateModelOverride();
	AnimationSkeletonNode* _parent;
	std::vector<AnimationSkeletonNode*> _children;
	RenderModel* _model;
	std::string _animKey;
	glm::mat4 _worldTransform;
	glm::mat4 _overlayTransform;
	bool _muted;
	// glm::mat4 _modelOverride;
};