#pragma once

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
		Flower,
		First = Top,
		Last = Flower,
		Size = Last + 1
	};
	static void Init();
	static void Destroy();
	static float* GetBuffer(BufferType);
private:
	static float* _buffers[Size];
};
