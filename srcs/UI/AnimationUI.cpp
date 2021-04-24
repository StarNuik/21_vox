#include <algorithm>

#include "Mathf/Mathf.h"

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
		mathf::vec3 origScale = node->GetScale();

		mathf::vec3 newScale = origScale;
		ImGui::SliderFloat3("Scale", &(newScale.x), 0.01f, 5.0f);

		if (newScale != origScale) {
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
