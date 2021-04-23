#include "Animation/AnimationClip.h"
#include <glm/glm.hpp>
#include "Utilities/Log.h"

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

// AnimationClip::AnimationClip(aiAnimation* animation) {
// 	for (int i = 0; i < animation->mNumChannels; i++) {
// 		aiNodeAnim* animNode = animation->mChannels[i];
// 		std::string key(animNode->mNodeName.data);
// 		_channels[key] = new AnimationChannel(animNode);
// 	}
// 	_duration = animation->mDuration;
// }

AnimationClip::~AnimationClip() {
	for (auto pair : _channels) {
		delete pair.second;
	}
}

AnimationClip::AnimationClip(std::string path) {
	path += ".dae";

	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(
		path,
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices
	);

	if (!scene) {
		Log::Error("Assimp failed to load the scene: ", path);
		return;
	}
	if (scene->mNumAnimations <= 0) {
		Log::Error("File doesn't have any animations: ", path);
		return;
	}

	aiAnimation* animation = scene->mAnimations[0];

	for (int i = 0; i < animation->mNumChannels; i++) {
		aiNodeAnim* animNode = animation->mChannels[i];
		std::string key(animNode->mNodeName.data);
		_channels[key] = new AnimationChannel(animNode);
	}
	_duration = animation->mDuration;
}

glm::mat4 AnimationClip::GetModelMatrix(std::string key, double time) {
	if (_channels[key] == nullptr) {
		return glm::mat4(1.f);
	}
	return _channels[key]->GetModelMatrixAtTime(time);
}

double AnimationClip::GetDuration() { return _duration; }

AnimationChannel::AnimationChannel(aiNodeAnim* animNode) {
	_key = std::string(animNode->mNodeName.data);
	
	for (int i = 0; i < animNode->mNumPositionKeys; i++) {
		aiVectorKey positionKey = animNode->mPositionKeys[i];
		aiVector3D vec = positionKey.mValue;
		_positions[positionKey.mTime] = mathf::vec3(vec.x, vec.y, vec.z);
	}

	for (int i = 0; i < animNode->mNumScalingKeys; i++) {
		aiVectorKey scaleKey = animNode->mScalingKeys[i];
		aiVector3D vec = scaleKey.mValue;
		_scales[scaleKey.mTime] = mathf::vec3(vec.x, vec.y, vec.z);
	}

	for (int i = 0; i < animNode->mNumScalingKeys; i++) {
		aiQuatKey rotationKey = animNode->mRotationKeys[i];
		aiQuaternion quat = rotationKey.mValue;
		mathf::quat q = mathf::quat(quat.x, quat.y, quat.z, quat.w);
		// q = glm::normalize(q);
		// if (glm::length(q) == 0) {
		// 	q = glm::identity<mathf::quat>();
		// }
		_rotations[rotationKey.mTime] = q;
		// _rotations[rotationKey.mTime]

	}
}


glm::mat4 AnimationChannel::GetModelMatrixAtTime(double time) {
	AnimTimeStamp positionTime = FindKeys(time, _positions);
	AnimTimeStamp scaleTime = FindKeys(time, _scales);
	AnimTimeStamp rotationTime = FindKeys(time, _rotations);

	mathf::vec3 position = mathf::vec3(glm::mix(
		_positions[std::get<0>(positionTime)].to_glm(),
		_positions[std::get<1>(positionTime)].to_glm(),
		(float)((time - std::get<0>(positionTime)) / (std::get<1>(positionTime)  - std::get<0>(positionTime)))
	));

	mathf::vec3 scale = mathf::vec3(glm::mix(
		_scales[std::get<0>(scaleTime)].to_glm(),
		_scales[std::get<1>(scaleTime)].to_glm(),
		(float)((time - std::get<0>(scaleTime)) / (std::get<1>(scaleTime)  - std::get<0>(scaleTime)))
	));

	mathf::quat rotation = mathf::quat(glm::slerp(
		_rotations[std::get<0>(rotationTime)].to_glm(),
		_rotations[std::get<1>(rotationTime)].to_glm(),
		(float)((time - std::get<0>(rotationTime)) / (std::get<1>(rotationTime)  - std::get<0>(rotationTime))))
	);
	// mathf::vec3 position = _positions[std::get<0>(positionTime)];
	// mathf::vec3 scale = _scales[std::get<0>(scaleTime)];
	// mathf::quat rotation = _rotations[std::get<0>(rotationTime)];

	glm::mat4 result = glm::identity<glm::mat4>();
	result = glm::translate(result, position.to_glm());
	result = result * glm::mat4_cast(rotation.to_glm());
	result = glm::scale(result, scale.to_glm());
	// result = glm::scale(result, mathf::vec3(5.f));
	return result;
}

AnimTimeStamp AnimationChannel::FindKeys(double time, std::map<double, mathf::vec3>& map) {
	double previous = 0.f;
	for (auto pair : map) {
		if (pair.first >= time) {
			return std::make_tuple(previous, pair.first);
		}
		previous = pair.first;
	}
	return std::make_tuple(previous, previous);
}

AnimTimeStamp AnimationChannel::FindKeys(double time, std::map<double, mathf::quat>& map) {
	double previous = 0.f;
	for (auto pair : map) {
		if (pair.first >= time) {
			return std::make_tuple(previous, pair.first);
		}
		previous = pair.first;
	}
	return std::make_tuple(previous, previous);
}