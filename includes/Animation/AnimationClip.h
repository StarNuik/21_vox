#pragma once

#include "Types.h"
#include <assimp/scene.h>
//#include <glm/glm.hpp>
#include <vector>
#include <map>
#include <glm/gtc/quaternion.hpp>
#include <tuple>

#include "Mathf/Mathf.h"

using AnimTimeStamp = std::tuple<double, double>;

class AnimationChannel;

class AnimationClip {
public:
	// AnimationClip(aiAnimation* animation);
	~AnimationClip();
	AnimationClip(std::string path);
	mathf::mat4x4 GetModelMatrix(std::string key, double time);
	double GetDuration();
private:
	double _duration;
	std::map<std::string, AnimationChannel*> _channels;
};

class AnimationChannel {
public:
	AnimationChannel(aiNodeAnim* animNode);
	mathf::mat4x4 GetModelMatrixAtTime(double time);
	AnimTimeStamp FindKeys(double time, std::map<double, mathf::vec3>& map);
	AnimTimeStamp FindKeys(double time, std::map<double, mathf::quat>& map);
private:
	std::string _key;
	std::map<double, mathf::vec3> _positions;
	std::map<double, mathf::vec3> _scales;
	std::map<double, mathf::quat> _rotations;
};
