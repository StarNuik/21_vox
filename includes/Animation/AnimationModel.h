#pragma once

#include <glm/glm.hpp>
#include "Types.h"
#include "Engine/Entity.h"
#include <string>
#include <vector>
// #include <assimp/Importer.hpp>
#include <assimp/scene.h>

class AnimationModel : public Entity
{
public:
	AnimationModel(Game* game_BADTIME, GLRenderer *renderer, std::string path, Shader* shader, Material* material);
	void Update(float delta);
	// ~AnimationModel() {};
private:
	bool SceneHasAnimationChannel(const aiScene* scene, aiString nodeName);
	void LogSceneTree(const aiScene* scene, aiNode* node, uint offset);
	void LogSceneInfo(const aiScene* scene, std::string path);
	RenderModel* MeshToModel(aiMesh* mesh, GLRenderer* renderer, Shader* shader, Material* material);
	std::vector<RenderModel*> models;
};