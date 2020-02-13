#pragma once

#include <vector>
#include <array>
#include <Render/Geometry.h>

class VertexBuffers {
public:
	enum BufferType {
		Top = 0,
		Bottom,
		Right,
		Left,
		Front,
		Back,
		MultiTop,
		MultiBottom,
		MultiRight,
		MultiLeft,
		MultiFront,
		MultiBack,
		WaterRight,
		WaterLeft,
		WaterFront,
		WaterBack,
		WaterTop,
		Flower,
		Cactus,
		Skybox,
		First = Top,
		Last = Skybox,
		Size = Last + 1
	};
	static void Init();
	static void Destroy();
	static float* GetBuffer(BufferType);
	static std::vector<float> GetVectorBuffer(BufferType);
private:
	static float* _buffers[Size];
	static std::array<std::vector<float>, Size> _buffs;
};
