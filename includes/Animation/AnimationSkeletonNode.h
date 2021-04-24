#pragma once
#include <vector>
#include <assimp/scene.h>

#include "Mathf/Mathf.h"

#include "Types.h"

class AnimationSkeletonNode {
	friend class AnimationModel;
public:
	~AnimationSkeletonNode();
	std::vector<AnimationSkeletonNode*> GetChildren();
	std::string GetKey();
	mathf::vec3 GetScale();
	void SetScale(mathf::vec3);
private:
	AnimationSkeletonNode(Game* game, aiNode* node, AnimationSkeletonNode* parent);
	void ApplyAnimation(AnimationClip* clip, float time);
	void ApplyOverlay(std::string key, mathf::mat4x4 matrix);
	void OverrideModelScale(std::string key, mathf::vec3 scale);
	void Mute(std::string key);
	mathf::mat4x4 CalculateModelOverride();
	AnimationSkeletonNode* _parent;
	std::vector<AnimationSkeletonNode*> _children;
	RenderModel* _model;
	std::string _animKey;
	mathf::mat4x4 _worldTransform;
	mathf::mat4x4 _overlayTransform;
	bool _muted;
};