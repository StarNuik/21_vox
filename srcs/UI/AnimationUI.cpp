#include <algorithm>
#include <glm/glm.hpp>
#include "Mathf/Mathf.h"
#include <glm/gtc/type_ptr.hpp>

#include "UI/UIController.h"
#include "Engine/Game.h"
#include "Utilities/Profiler.h"
#include "Player/Player.h"
#include "Animation/AnimationModel.h"
#include "Animation/AnimationSkeletonNode.h"

void UIController::UpdateAnimation() {
	UIData::DataAnimation& data = _dataAnimation;
}

void DrawSkeletonTree(AnimationSkeletonNode* node) {
	if (ImGui::TreeNode(node->GetKey().c_str()))
	{
		glm::vec3 origScale = node->GetScale().to_glm();

		float newScale = origScale.x;
		ImGui::SliderFloat("Scale", &newScale, 0.01f, 5.0f);

		if (newScale != origScale.x) {
			node->SetScale(mathf::vec3(newScale));
		}

		for (AnimationSkeletonNode* child : node->GetChildren()) {
			DrawSkeletonTree(child);
		}
		ImGui::TreePop();
	}
}

void UIController::AnimationUI() {
	UIData::DataAnimation& data = _dataAnimation;

	AnimationModel* animka = _game->GetHuman();
	AnimationSkeletonNode* node = animka->GetRoot();
	ImGui::SliderFloat("Animation Speed", &animka->speed, 0.f, 2.f);
	DrawSkeletonTree(node);
	// for ()

}
