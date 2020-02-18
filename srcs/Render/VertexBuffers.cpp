#include "Render/VertexBuffers.h"

float* VertexBuffers::_buffers[VertexBuffers::Size];
std::array<std::vector<float>, VertexBuffers::Size> VertexBuffers::_buffs;

void VertexBuffers::Init() {
	//?    Position
	//?                     Normal
	//?                                      UV
	_buffers[Top] = new float[48] {
		0.f, 1.f, 0.f,   0.f, 1.f, 0.f,   0.f, 1.f,
		0.f, 1.f, 1.f,   0.f, 1.f, 0.f,   0.f, 0.f,
		1.f, 1.f, 1.f,   0.f, 1.f, 0.f,   1.f, 0.f,

		1.f, 1.f, 1.f,   0.f, 1.f, 0.f,   1.f, 0.f,
		1.f, 1.f, 0.f,   0.f, 1.f, 0.f,   1.f, 1.f,
		0.f, 1.f, 0.f,   0.f, 1.f, 0.f,   0.f, 1.f,
	};
	_buffers[Bottom] = new float[48] {
		0.f, 0.f, 0.f,   0.f, -1.f, 0.f,  0.f, 0.f,
		1.f, 0.f, 1.f,   0.f, -1.f, 0.f,  1.f, 1.f,
		0.f, 0.f, 1.f,   0.f, -1.f, 0.f,  1.f, 0.f,

		1.f, 0.f, 1.f,   0.f, -1.f, 0.f,  1.f, 1.f,
		0.f, 0.f, 0.f,   0.f, -1.f, 0.f,  0.f, 0.f,
		1.f, 0.f, 0.f,   0.f, -1.f, 0.f,  0.f, 1.f,
	};
	_buffers[Right] = new float[48] {
		1.f, 1.f, 0.f,   1.f, 0.f, 0.f,   0.f, 1.f,
		1.f, 0.f, 1.f,   1.f, 0.f, 0.f,   1.f, 0.f,
		1.f, 0.f, 0.f,   1.f, 0.f, 0.f,   0.f, 0.f,

		1.f, 0.f, 1.f,   1.f, 0.f, 0.f,   1.f, 0.f,
		1.f, 1.f, 0.f,   1.f, 0.f, 0.f,   0.f, 1.f,
		1.f, 1.f, 1.f,   1.f, 0.f, 0.f,   1.f, 1.f,
	};
	_buffers[Left] = new float[48] {
		0.f, 0.f, 1.f,   -1.f, 0.f, 0.f,  1.f, 0.f,
		0.f, 1.f, 1.f,   -1.f, 0.f, 0.f,  1.f, 1.f,
		0.f, 1.f, 0.f,   -1.f, 0.f, 0.f,  0.f, 1.f,

		0.f, 1.f, 0.f,   -1.f, 0.f, 0.f,  0.f, 1.f,
		0.f, 0.f, 0.f,   -1.f, 0.f, 0.f,  0.f, 0.f,
		0.f, 0.f, 1.f,   -1.f, 0.f, 0.f,  1.f, 0.f,
	};
	_buffers[Back] = new float[48] {
		0.f, 0.f, 0.f,   0.f, 0.f, -1.f,  0.f, 0.f,
		0.f, 1.f, 0.f,   0.f, 0.f, -1.f,  0.f, 1.f,
		1.f, 1.f, 0.f,   0.f, 0.f, -1.f,  1.f, 1.f,

		1.f, 1.f, 0.f,   0.f, 0.f, -1.f,  1.f, 1.f,
		1.f, 0.f, 0.f,   0.f, 0.f, -1.f,  1.f, 0.f,
		0.f, 0.f, 0.f,   0.f, 0.f, -1.f,  0.f, 0.f,
	};
	_buffers[Front] = new float[48] {
		0.f, 1.f, 1.f,   0.f, 0.f, 1.f,   0.f, 1.f,
		0.f, 0.f, 1.f,   0.f, 0.f, 1.f,   0.f, 0.f,
		1.f, 0.f, 1.f,   0.f, 0.f, 1.f,   1.f, 0.f,

		1.f, 0.f, 1.f,   0.f, 0.f, 1.f,   1.f, 0.f,
		1.f, 1.f, 1.f,   0.f, 0.f, 1.f,   1.f, 1.f,
		0.f, 1.f, 1.f,   0.f, 0.f, 1.f,   0.f, 1.f,
	};
	_buffers[MultiTop] = new float[48] {
		0.f, 1.f, 0.f,   0.f, 1.f, 0.f,   0.25f, 1.f,
		0.f, 1.f, 1.f,   0.f, 1.f, 0.f,   0.25f, 0.5f,
		1.f, 1.f, 1.f,   0.f, 1.f, 0.f,   0.f, 0.5f,

		1.f, 1.f, 1.f,   0.f, 1.f, 0.f,   0.f, 0.5f,
		1.f, 1.f, 0.f,   0.f, 1.f, 0.f,   0.f, 1.f,
		0.f, 1.f, 0.f,   0.f, 1.f, 0.f,   0.25f, 1.f,
	};
	_buffers[MultiBottom] = new float[48] {
		0.f, 0.f, 0.f,   0.f, -1.f, 0.f,  0.25f, 1.f,
		1.f, 0.f, 1.f,   0.f, -1.f, 0.f,  0.25f, 0.5f,
		0.f, 0.f, 1.f,   0.f, -1.f, 0.f,  0.5f, 0.5f,

		1.f, 0.f, 1.f,   0.f, -1.f, 0.f,  0.5f, 0.5f,
		0.f, 0.f, 0.f,   0.f, -1.f, 0.f,  0.5f, 1.f,
		1.f, 0.f, 0.f,   0.f, -1.f, 0.f,  0.25f, 1.f,
	};
	_buffers[MultiRight] = new float[48] {
		1.f, 1.f, 0.f,   1.f, 0.f, 0.f,   0.5f, 0.5f,
		1.f, 0.f, 1.f,   1.f, 0.f, 0.f,   0.75f, 0.f,
		1.f, 0.f, 0.f,   1.f, 0.f, 0.f,   0.5f, 0.f,

		1.f, 0.f, 1.f,   1.f, 0.f, 0.f,   0.75f, 0.f,
		1.f, 1.f, 0.f,   1.f, 0.f, 0.f,   0.5f, 0.5f,
		1.f, 1.f, 1.f,   1.f, 0.f, 0.f,   0.75f, 0.5f,
	};
	_buffers[MultiLeft] = new float[48] {
		0.f, 0.f, 1.f,   -1.f, 0.f, 0.f,  0.f, 0.f,
		0.f, 1.f, 1.f,   -1.f, 0.f, 0.f,  0.f, 0.5f,
		0.f, 1.f, 0.f,   -1.f, 0.f, 0.f,  0.25f, 0.5f,

		0.f, 1.f, 0.f,   -1.f, 0.f, 0.f,  0.25f, 0.5f,
		0.f, 0.f, 0.f,   -1.f, 0.f, 0.f,  0.25f, 0.f,
		0.f, 0.f, 1.f,   -1.f, 0.f, 0.f,  0.f, 0.f,
	};
	_buffers[MultiBack] = new float[48] {
		0.f, 0.f, 0.f,   0.f, 0.f, -1.f,  0.5f, 0.f,
		0.f, 1.f, 0.f,   0.f, 0.f, -1.f,  0.5f, 0.5f,
		1.f, 1.f, 0.f,   0.f, 0.f, -1.f,  0.25f, 0.5f,

		1.f, 1.f, 0.f,   0.f, 0.f, -1.f,  0.25f, 0.5f,
		1.f, 0.f, 0.f,   0.f, 0.f, -1.f,  0.25f, 0.f,
		0.f, 0.f, 0.f,   0.f, 0.f, -1.f,  0.5f, 0.f,
	};
	_buffers[MultiFront] = new float[48] {
		0.f, 1.f, 1.f,   0.f, 0.f, 1.f,   0.5f, 1.f,
		0.f, 0.f, 1.f,   0.f, 0.f, 1.f,   0.5f, .5f,
		1.f, 0.f, 1.f,   0.f, 0.f, 1.f,   0.75f, .5f,

		1.f, 0.f, 1.f,   0.f, 0.f, 1.f,   0.75f, 0.5f,
		1.f, 1.f, 1.f,   0.f, 0.f, 1.f,   0.75f, 1.f,
		0.f, 1.f, 1.f,   0.f, 0.f, 1.f,   0.5f, 1.f,
	};
	//? \ First, / Second
	_buffers[Flower] = new float[96] {
		0.f, 0.f, 0.f,   0.f, 1.f, 0.f,   1.f,  0.f,
		1.f, 0.f, 1.f,   0.f, 1.f, 0.f,   0.f,  0.f,
		0.f, 1.f, 0.f,   0.f, 1.f, 0.f,   1.f,  1.f,

		1.f, 0.f, 1.f,   0.f, 1.f, 0.f,   0.f,  1.f,
		1.f, 1.f, 1.f,   0.f, 1.f, 0.f,   1.f,  1.f,
		0.f, 1.f, 0.f,   0.f, 1.f, 0.f,   1.f,  0.f,


		0.f, 0.f, 1.f,   0.f, 1.f, 0.f,   0.f,  0.f,
		1.f, 0.f, 0.f,   0.f, 1.f, 0.f,   1.f,  0.f,
		1.f, 1.f, 0.f,   0.f, 1.f, 0.f,   1.f,  1.f,

		0.f, 0.f, 1.f,   0.f, 1.f, 0.f,   0.f,  0.f,
		1.f, 1.f, 0.f,   0.f, 1.f, 0.f,   1.f,  1.f,
		0.f, 1.f, 1.f,   0.f, 1.f, 0.f,   0.f,  1.f,
	};
	_buffers[Skybox] = new float[11 * 6 * 6] {
		-1.0f,  1.0f, -1.0f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f,
		-1.0f, -1.0f, -1.0f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f,
		1.0f, -1.0f, -1.0f,  0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f,
		1.0f, -1.0f, -1.0f,  0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f,
		1.0f,  1.0f, -1.0f,  0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f,
		-1.0f,  1.0f, -1.0f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f,

		-1.0f, -1.0f,  1.0f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f,
		-1.0f, -1.0f, -1.0f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f,
		-1.0f,  1.0f, -1.0f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f,
		-1.0f,  1.0f, -1.0f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f,
		-1.0f,  1.0f,  1.0f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f,
		-1.0f, -1.0f,  1.0f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f,

		1.0f, -1.0f, -1.0f,  0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f,
		1.0f, -1.0f,  1.0f,  0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f,
		1.0f,  1.0f,  1.0f,  0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f,
		1.0f,  1.0f,  1.0f,  0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f,
		1.0f,  1.0f, -1.0f,  0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f,
		1.0f, -1.0f, -1.0f,  0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f,

		-1.0f, -1.0f,  1.0f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f,
		-1.0f,  1.0f,  1.0f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f,
		1.0f,  1.0f,  1.0f,  0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f,
		1.0f,  1.0f,  1.0f,  0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f,
		1.0f, -1.0f,  1.0f,  0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f,
		-1.0f, -1.0f,  1.0f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f,

		-1.0f,  1.0f, -1.0f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f,
		1.0f,  1.0f, -1.0f,  0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f,
		1.0f,  1.0f,  1.0f,  0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f,
		1.0f,  1.0f,  1.0f,  0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f,
		-1.0f,  1.0f,  1.0f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f,
		-1.0f,  1.0f, -1.0f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f,

		-1.0f, -1.0f, -1.0f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f,
		-1.0f, -1.0f,  1.0f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f,
		1.0f, -1.0f, -1.0f,  0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f,
		1.0f, -1.0f, -1.0f,  0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f,
		-1.0f, -1.0f,  1.0f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f,
		1.0f, -1.0f,  1.0f,  0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f,
	};
	_buffs[Cactus] = Geometry::ReadGeometry("./resources/Models/Cactus.obj");
	_buffs[Flower] = Geometry::ReadGeometry("./resources/Models/Flower.obj");
	_buffs[WaterBack] = Geometry::ReadGeometry("./resources/Models/WaterBack.obj");
	_buffs[WaterRight] = Geometry::ReadGeometry("./resources/Models/WaterRight.obj");
	_buffs[WaterFront] = Geometry::ReadGeometry("./resources/Models/WaterFront.obj");
	_buffs[WaterLeft] = Geometry::ReadGeometry("./resources/Models/WaterLeft.obj");
	_buffs[WaterTop] = Geometry::ReadGeometry("./resources/Models/WaterTop.obj");
	_buffs[Top] = Geometry::ReadGeometry("./resources/Models/Top.obj");
	_buffs[Bottom] = Geometry::ReadGeometry("./resources/Models/Bottom.obj");
	_buffs[Right] = Geometry::ReadGeometry("./resources/Models/Right.obj");
	_buffs[Left] = Geometry::ReadGeometry("./resources/Models/Left.obj");
	_buffs[Front] = Geometry::ReadGeometry("./resources/Models/Front.obj");
	_buffs[Back] = Geometry::ReadGeometry("./resources/Models/Back.obj");
	_buffs[Cactus].shrink_to_fit();
	_buffs[Flower].shrink_to_fit();
	_buffs[WaterBack].shrink_to_fit();
	_buffs[WaterRight].shrink_to_fit();
	_buffs[WaterFront].shrink_to_fit();
	_buffs[WaterLeft].shrink_to_fit();
	_buffs[WaterTop].shrink_to_fit();
}

void VertexBuffers::Destroy() {
	for (int i = First; i <= Last; i++)
		delete _buffers[i];
}

std::vector<float> VertexBuffers::GetVectorBuffer(BufferType t) {
	return _buffs[t];
}

float* VertexBuffers::GetBuffer(BufferType t) {
	if (_buffers[t])
		return _buffers[t];
	else
		return _buffs[t].data();
}