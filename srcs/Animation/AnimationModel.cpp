#include "base.h"
#include "Animation/AnimationModel.h"
#include "Animation/AnimationClip.h"
#include "Animation/AnimationSkeletonNode.h"
#include "Utilities/Log.h"
#include "Utilities/Utilities.h"
#include "Render/Geometry.h"
#include "Render/RenderModel.h"
#include "Engine/Game.h"
#include "World/ResourceLoader.h"
#include "Input/Input.h"

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

AnimationSkeletonNode* AnimationModel::GetRoot() { return _skeletonRoot; }

void AnimationModel::SetAnimationClip(AnimationClip* clip) {
	_clip = clip;
	_animationTime = 0.f;
}

AnimationModel::~AnimationModel() {
	delete _skeletonRoot;
}

AnimationModel::AnimationModel(Game* game, std::string path) {
	_game = game;
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

	LogSceneInfo(scene, path);

	_animationTime = 0.f;

	_isPaused = false;

	_skeletonRoot = new AnimationSkeletonNode(game, scene->mRootNode, nullptr);
	Log::Success("Assimp succedeed");

	speed = 1.f;
}

void AnimationModel::Update(float delta) {
	if (_game->GetInput()->KeyJustPressed(GLFW_KEY_TAB)) {
		_isPaused = !_isPaused;
	}

	if (_isPaused) return;

	_animationTime += delta * speed;
	if (_animationTime > _clip->GetDuration()) {
		_animationTime = 0.f;
	}

	_skeletonRoot->ApplyAnimation(_clip, _animationTime);
}

void AnimationModel::AddOverlayMatrix(std::string key, mathf::mat4x4 matrix) {
	_skeletonRoot->ApplyOverlay(key, matrix);
}


void AnimationModel::LogSceneInfo(const aiScene* scene, std::string path) {
	Log::Basic("Mesh info of: ", path);
	Log::Basic("Materials: ", scene->mNumMaterials);
	Log::Basic("Textures: ", scene->mNumTextures);
	Log::Basic("Cameras: ", scene->mNumCameras);
	Log::Basic("Lights: ", scene->mNumLights);

	Log::Important("Meshes: ", scene->mNumMeshes);
	for (int i = 0; i < scene->mNumMeshes; i++) {
		aiMesh* mesh = scene->mMeshes[i];
		Log::Basic("");
		Log::Basic("\tName: ", mesh->mName.data);
		Log::Basic("\tVertices: ", mesh->mNumVertices);
		Log::Basic("\tAnim Meshes: ", mesh->mNumAnimMeshes);
		Log::Important("\tBones: ", mesh->mNumBones);
	}

	Log::Important("Animations: ", scene->mNumAnimations);
	for (int i = 0; i < scene->mNumAnimations; i++) {
		aiAnimation* animation = scene->mAnimations[i];
		Log::Basic("\tName: ", animation->mName.data);
		Log::Basic("\tDuration: ", animation->mDuration);
		Log::Basic("\tTicks per Second: ", animation->mTicksPerSecond);
		Log::Basic("\tMesh Channels: ", animation->mNumMeshChannels);
		Log::Basic("\tMorph Mesh Channels: ", animation->mNumMorphMeshChannels);
		Log::Basic("\tChannels: ", animation->mNumChannels);
	}

	Log::Important("Hierarchy");
	LogSceneTree(scene, scene->mRootNode, 0);
}

void AnimationModel::MuteBoneModel(std::string key) {
	_skeletonRoot->Mute(key);
}

void AnimationModel::LogSceneTree(const aiScene* scene, aiNode* node, uint offset) {
	std::string spaces = "";
	for (int i = 0; i < offset; i++) {
		spaces += "|  ";
	}
	spaces += "- ";

	Log::Basic(spaces, node->mName.data);

	for (int i = 0; i < node->mNumChildren; i++) {
		LogSceneTree(scene, node->mChildren[i], offset + 1);
	}
}
