#include "Animation/AnimationModel.h"
#include "Utilities/Log.h"
#include "Render/Geometry.h"
#include "Render/RenderModel.h"

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

AnimationModel::AnimationModel(Game* game_BADTIME, GLRenderer *renderer, std::string path, Shader* shader, Material* material) {
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

	Log::Success("Assimp succedeed");
	LogSceneInfo(scene, path);

	for (int meshId = 0; meshId < scene->mNumMeshes; meshId++) {
		aiMesh* mesh = scene->mMeshes[meshId];
		models.push_back(MeshToModel(mesh, renderer, shader, material));
	}
}

RenderModel* AnimationModel::MeshToModel(aiMesh* mesh, GLRenderer* renderer, Shader* shader, Material* material) {
	std::vector<float> vertexData;

	for (int faceId = 0; faceId < mesh->mNumFaces; faceId++) {
		aiFace face = mesh->mFaces[faceId];

		for (int i = 0; i < face.mNumIndices; i++) {
			uint index = face.mIndices[i];
			aiVector3D vertex = mesh->mVertices[index];
			aiVector3D normal = mesh->mNormals[index];
			aiVector3D uv = mesh->mTextureCoords[0][index];
			vertexData.push_back(vertex.x);
			vertexData.push_back(vertex.y);
			vertexData.push_back(vertex.z);
			vertexData.push_back(normal.x);
			vertexData.push_back(normal.y);
			vertexData.push_back(normal.z);
			vertexData.push_back(uv.x);
			vertexData.push_back(uv.y);
		}
	}

	Geometry* g = new Geometry(vertexData);
	return new RenderModel(renderer, shader, material, g);
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
		Log::Important("\tChannels: ", animation->mNumChannels);
		// for (int j = 0; j < animation->mNumChannels; j++) {
		// 	aiNodeAnim* animNode = animation->mChannels[j];
		// 	Log::Basic("");
		// 	Log::Basic("\t\tName: ", animNode->mNodeName.data);
		// 	Log::Basic("\t\tPosition keys: ", animNode->mNumPositionKeys);
		// 	Log::Basic("\t\tRotation keys: ", animNode->mNumRotationKeys);
		// 	Log::Basic("\t\tScaling keys: ", animNode->mNumScalingKeys);
		// 	Log::Basic("\t\tPre State: ", animNode->mPreState);
		// 	Log::Basic("\t\tPost State: ", animNode->mPostState);
		// }
	}

	Log::Important("Hierarchy");
	LogSceneTree(scene, scene->mRootNode, 0);
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

bool AnimationModel::SceneHasAnimationChannel(const aiScene* scene, aiString nodeName) {
	for (int i = 0; i < scene->mNumAnimations; i++) {
		aiAnimation* anim = scene->mAnimations[i];
		for (int j = 0; j < anim->mNumChannels; j++) {
			aiNodeAnim* animNode = anim->mChannels[j];
			if (strcmp(animNode->mNodeName.data, nodeName.data) == 0) return true;
		}
	}
	return false;
}

void AnimationModel::Update(float delta) {

}