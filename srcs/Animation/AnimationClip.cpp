#include "Animation/AnimationClip.h"
#include <glm/glm.hpp>
#include "Utilities/Log.h"
// #include "assim"

AnimationClip::AnimationClip(aiAnimation* animation) {
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
		_positions[positionKey.mTime] = glm::vec3(vec.x, vec.y, vec.z);
	}

	for (int i = 0; i < animNode->mNumScalingKeys; i++) {
		aiVectorKey scaleKey = animNode->mScalingKeys[i];
		aiVector3D vec = scaleKey.mValue;
		_scales[scaleKey.mTime] = glm::vec3(vec.x, vec.y, vec.z);
	}

	for (int i = 0; i < animNode->mNumScalingKeys; i++) {
		aiQuatKey rotationKey = animNode->mRotationKeys[i];
		aiQuaternion quat = rotationKey.mValue;
		_rotations[rotationKey.mTime] = glm::quat(quat.w, quat.x, quat.y, quat.z);
	}
}


glm::mat4 AnimationChannel::GetModelMatrixAtTime(double time) {
	AnimTimeStamp positionTime = FindKeys(time, _positions);
	AnimTimeStamp scaleTime = FindKeys(time, _scales);
	AnimTimeStamp rotationTime = FindKeys(time, _rotations);

	// glm::vec3 position = glm::mix(
	// 	_positions[std::get<0>(positionTime)],
	// 	_positions[std::get<1>(positionTime)],
	// 	(float)((time - std::get<0>(positionTime)) / std::get<1>(positionTime))
	// );

	// glm::vec3 scale = glm::mix(
	// 	_scales[std::get<0>(scaleTime)],
	// 	_scales[std::get<1>(scaleTime)],
	// 	(float)((time - std::get<0>(scaleTime)) / std::get<1>(scaleTime))
	// );

	// glm::quat rotation = glm::mix(
	// 	_rotations[std::get<0>(rotationTime)],
	// 	_rotations[std::get<1>(rotationTime)],
	// 	(float)((time - std::get<0>(rotationTime)) / std::get<1>(rotationTime))
	// );
	glm::vec3 position = _positions[std::get<0>(positionTime)];
	glm::vec3 scale = _scales[std::get<0>(scaleTime)];
	glm::quat rotation = _rotations[std::get<0>(rotationTime)];

	glm::mat4 result = glm::identity<glm::mat4>();
	result = glm::translate(result, position);
	result = result * glm::mat4_cast(rotation);
	result = glm::scale(result, scale);
	// result = glm::scale(result, glm::vec3(5.f));
	return result;
}

AnimTimeStamp AnimationChannel::FindKeys(double time, std::map<double, glm::vec3>& map) {
	double previous = 0.f;
	for (auto pair : map) {
		if (pair.first >= time) {
			return std::make_tuple(previous, pair.first);
		}
		previous = pair.first;
	}
	return std::make_tuple(previous, previous);
}

AnimTimeStamp AnimationChannel::FindKeys(double time, std::map<double, glm::quat>& map) {
	double previous = 0.f;
	for (auto pair : map) {
		if (pair.first >= time) {
			return std::make_tuple(previous, pair.first);
		}
		previous = pair.first;
	}
	return std::make_tuple(previous, previous);
}