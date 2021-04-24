#include "Animation/AnimationSkeletonNode.h"
#include "Animation/AnimationModel.h"
#include "Animation/AnimationClip.h"
#include "World/ResourceLoader.h"
#include "Render/RenderModel.h"
#include "Engine/Game.h"
#include "Utilities/Utilities.h"
#include "Utilities/Log.h"

AnimationSkeletonNode::~AnimationSkeletonNode() {
	delete _model;
	for (AnimationSkeletonNode* child : _children) {
		delete child;
	}
}

std::string AnimationSkeletonNode::GetKey() { return _animKey; }
std::vector<AnimationSkeletonNode*> AnimationSkeletonNode::GetChildren() { return std::vector<AnimationSkeletonNode*>(_children); }

mathf::vec3 AnimationSkeletonNode::GetScale() { return mathf::vec3(_overlayTransform[0][0], _overlayTransform[1][1], _overlayTransform[2][2]); }
void AnimationSkeletonNode::SetScale(mathf::vec3 scale) {
	_overlayTransform[0][0] = scale.x;
	_overlayTransform[1][1] = scale.y;
	_overlayTransform[2][2] = scale.z;
}

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

	_overlayTransform = mathf::mat4x4::identity();
	_muted = false;

}

void AnimationSkeletonNode::Mute(std::string key) {
	if (_animKey == key) {
		_muted = true;
		return;
	}
	for (AnimationSkeletonNode* child : _children) {
		child->Mute(key);
	}
}

mathf::mat4x4 AnimationSkeletonNode::CalculateModelOverride() {
	if (_children.size() == 0) return mathf::mat4x4(_worldTransform);

	mathf::vec3 ps0;
	mathf::vec3 worldScale;
	mathf::vec3 v3;
	mathf::vec4 v4;
	mathf::quat q;
	mathf::mat4x4::decompose(_worldTransform, worldScale, q, ps0);

	mathf::vec3 ps1;
	{
			mathf::vec3 v3;
			mathf::vec4 v4;
			mathf::quat q;
			mathf::vec3 pos;
			mathf::mat4x4::decompose(_children[0]->_worldTransform, v3, q, ps1);
	}

	mathf::vec3 pos0(ps0);
	mathf::vec3 pos1(ps1);

	mathf::vec3 position = mathf::vec3(mathf::vec3::lerp(pos0, pos1, 0.5f));
	mathf::quat rotation = mathf::quat(mathf::quat::look_at((pos1 - pos0).normalize(), mathf::vec3(0.f, 1.f, 0.f)));

	float distance = mathf::vec3::distance(pos0, pos1);
	glm::vec3 simpleScale = glm::vec3(distance);
	float scaleSide = mathf::vec3::distance(pos0, pos1);
	float limbWidth = mathf::min(scaleSide, LIMB_WIDTH);

	float limbWidthX = mathf::min(scaleSide, LIMB_WIDTH * worldScale.x);
	float limbWidthY = mathf::min(scaleSide, LIMB_WIDTH * worldScale.y);

	mathf::vec3 scale = mathf::vec3(limbWidthX, limbWidthY, scaleSide);

	mathf::mat4x4 result = mathf::mat4x4::identity();
	result = mathf::mat4x4::translate(result, position);
	result = result * mathf::mat4x4::cast(rotation);
	result = mathf::mat4x4::scale(result, scale);
	return result;
}

void AnimationSkeletonNode::ApplyAnimation(AnimationClip* clip, float time) {
	_worldTransform = clip->GetModelMatrix(_animKey, time) * _overlayTransform;
	if (_parent) {
		_worldTransform = _parent->_worldTransform * _worldTransform;
	}

	for (AnimationSkeletonNode* child : _children) {
		child->ApplyAnimation(clip, time);
	}

	if (_muted) {
		mathf::mat4x4 m;
		_model->SetModelMatrix(m);
	} else {
		_model->SetModelMatrix(CalculateModelOverride());
	}
	//! 
	// _model->SetScale(mathf::vec3(1.f));
	//! 
}

void AnimationSkeletonNode::ApplyOverlay(std::string key, mathf::mat4x4 matrix) {
	if (_animKey == key) {
		// Log::Important("Overlay applied");
		_overlayTransform = matrix;
		return;
	}
	for (AnimationSkeletonNode* child : _children) {
		child->ApplyOverlay(key, matrix);
	}
}
