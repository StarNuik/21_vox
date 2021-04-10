#include "Animation/AnimationSkeletonNode.h"
#include "Animation/AnimationModel.h"
#include "Animation/AnimationClip.h"
#include "World/ResourceLoader.h"
#include "Render/RenderModel.h"
#include "Engine/Game.h"
#include "Utilities/Utilities.h"

AnimationSkeletonNode::AnimationSkeletonNode(Game* game, aiNode* node, AnimationSkeletonNode* parent) {
	_parent = parent;
	_animKey = std::string(node->mName.data);
	
	if (parent == nullptr) {
		_localTransform = Utilities::mat4_cast(node->mTransformation);
	} else {
		_localTransform = parent->_localTransform * Utilities::mat4_cast(node->mTransformation);
	}

	ResourceLoader* r = game->GetResources();
	_model = new RenderModel(
		game->GetRenderer(),
		r->GetShader(ANIM_SHADER),
		r->GetMaterial(ANIM_MATERIAL),
		r->GetGeometry(ANIM_GEOMETRY)
	);
	_model->SetModelMatrix(_localTransform);

	for (int i = 0; i < node->mNumChildren; i++) {
		_children.push_back(new AnimationSkeletonNode(game, node->mChildren[i], this));
	}
}

void AnimationSkeletonNode::ApplyAnimation(AnimationClip* clip, float time) {
	_localTransform = clip->GetModelMatrix(_animKey, time);
	if (_parent) {
		_localTransform = _parent->_localTransform * _localTransform;
	}

	// if (_parent == nullptr) {
		_model->SetModelMatrix(_localTransform);
		//! temp
		_model->SetScale(glm::vec3(5.f));
		//!
	// } else {
	// 	_model->SetModelMatrix(_parent->_localTransform * _localTransform);
	// }

	for (AnimationSkeletonNode* child : _children) {
		child->ApplyAnimation(clip, time);
	}
}
