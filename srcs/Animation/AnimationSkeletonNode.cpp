#include "Animation/AnimationSkeletonNode.h"
#include "Animation/AnimationModel.h"
#include "Animation/AnimationClip.h"
#include "World/ResourceLoader.h"
#include "Render/RenderModel.h"
#include "Engine/Game.h"
#include "Utilities/Utilities.h"
#include "Utilities/Log.h"
#include <glm/gtx/matrix_decompose.hpp>

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

	_overlayTransform = glm::identity<glm::mat4>();
	_muted = false;

	// _modelOverride = CalculateModelOverride();
	// _modelOverride = glm::mat4(1.f);
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

glm::mat4 AnimationSkeletonNode::CalculateModelOverride() {
	if (_children.size() == 0) return glm::mat4(_worldTransform);

	//? CURRENT
	//? Find global bone positions
	//? place the model inbetween bones
	//? Look rotation
	//? Size is distance between points
	//TODO DESIRED
	//TODO Find local bone positions
	//TODO Determine local size
	//TODO place inbetween global positions
	//TODO Look rotation
	//TODO size is local size * global scale
	glm::vec3 ps0;
	glm::vec3 worldScale;
	{
		glm::vec3 v3;
		glm::vec4 v4;
		glm::quat q;
		glm::decompose(_worldTransform, worldScale, worldRotation, ps0, v3, v4);
	}
	glm::vec3 ps1;
	{
		// mathf::vec3 sum(0.f);
		// for (AnimationSkeletonNode* child : _children) {
			glm::vec3 v3;
			glm::vec4 v4;
			glm::quat q;
			glm::vec3 pos;
			// glm::decompose(child->_worldTransform, v3, q, pos, v3, v4);
			glm::decompose(_children[0]->_worldTransform, v3, q, ps1, v3, v4);
			// sum += pos;
		// }
		// pos1 = sum / mathf::vec3(_children.size());
	}

	mathf::vec3 pos0(ps0);
	mathf::vec3 pos1(ps1);

	mathf::vec3 position = mathf::vec3(glm::mix(pos0.to_glm(), pos1.to_glm(), 0.5f));
	mathf::quat rotation = mathf::quat(glm::quatLookAt((pos1 - pos0).normalize().to_glm(), mathf::vec3(0.f, 1.f, 0.f).to_glm()));

	float distance = mathf::vec3::distance(pos0, pos1);
	glm::vec3 simpleScale = glm::vec3(distance);
	float scaleSide = mathf::vec3::distance(pos0, pos1);
	float limbWidth = mathf::min(scaleSide, LIMB_WIDTH);

	float limbWidthX = mathf::min(scaleSide, LIMB_WIDTH * worldScale.x);
	float limbWidthY = mathf::min(scaleSide, LIMB_WIDTH * worldScale.y);

	mathf::vec3 scale = mathf::vec3(limbWidthX, limbWidthY, scaleSide);

	glm::mat4 result(1.f);
	result = glm::translate(result, position.to_glm());
	result = result * glm::mat4_cast(rotation.to_glm());
	result = glm::scale(result, scale.to_glm());
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
		_model->SetModelMatrix(glm::mat4(0.f));
	} else {
		_model->SetModelMatrix(CalculateModelOverride());
		// _model->SetModelMatrix(_worldTransform * _modelOverride);
	}
	//! 
	// _model->SetScale(mathf::vec3(1.f));
	//! 
}

void AnimationSkeletonNode::ApplyOverlay(std::string key, glm::mat4 matrix) {
	if (_animKey == key) {
		// Log::Important("Overlay applied");
		_overlayTransform = matrix;
		return;
	}
	for (AnimationSkeletonNode* child : _children) {
		child->ApplyOverlay(key, matrix);
	}
}
