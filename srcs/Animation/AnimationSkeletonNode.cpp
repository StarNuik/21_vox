#include "Animation/AnimationSkeletonNode.h"
#include "Animation/AnimationModel.h"
#include "Animation/AnimationClip.h"
#include "World/ResourceLoader.h"
#include "Render/RenderModel.h"
#include "Engine/Game.h"
#include "Utilities/Utilities.h"
#include "Utilities/Log.h"
#include <glm/gtx/matrix_decompose.hpp>

AnimationSkeletonNode::AnimationSkeletonNode(Game* game, aiNode* node, AnimationSkeletonNode* parent) {
	_parent = parent;
	_animKey = std::string(node->mName.data);
	
	if (parent == nullptr) {
		_worldTransform = Utilities::mat4_cast(node->mTransformation);
	} else {
		_worldTransform = parent->_worldTransform * Utilities::mat4_cast(node->mTransformation);
	}

	ResourceLoader* r = game->GetResources();
	_model = new RenderModel(
		game->GetRenderer(),
		r->GetShader(ANIM_SHADER),
		r->GetMaterial(ANIM_MATERIAL),
		r->GetGeometry(ANIM_GEOMETRY)
	);
	_model->SetModelMatrix(_worldTransform);

	for (int i = 0; i < node->mNumChildren; i++) {
		_children.push_back(new AnimationSkeletonNode(game, node->mChildren[i], this));
	}

	_overlayTransform = glm::identity<glm::mat4>();
}

void AnimationSkeletonNode::ApplyAnimation(AnimationClip* clip, float time) {
	_worldTransform = clip->GetModelMatrix(_animKey, time) * _overlayTransform;
	if (_parent) {
		_worldTransform = _parent->_worldTransform * _worldTransform;
	}

	_model->SetModelMatrix(_worldTransform);
	//! 
	_model->SetScale(glm::vec3(1.f));
	//! 

	for (AnimationSkeletonNode* child : _children) {
		child->ApplyAnimation(clip, time);
	}
}

void AnimationSkeletonNode::ApplyOverlay(std::string key, glm::mat4 matrix) {
	if (_animKey == key) {
		Log::Important("Overlay applied");
		_overlayTransform = matrix;
		return;
	}
	for (AnimationSkeletonNode* child : _children) {
		child->ApplyOverlay(key, matrix);
	}
}
