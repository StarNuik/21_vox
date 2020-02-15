#include <GL/glew.h>

#include "Render/Geometry.h"
#include "Utilities/Log.h"
#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"
// #include "Types.h"
// #include <string>
#include <glm/glm.hpp>

#include <iostream>
namespace {
void ResolveTangents(std::vector<float>& vertices) {
	for (int i = 0; i < vertices.size(); i += VERTEX_SIZE * 3) {
		glm::vec3 pos1(vertices[i + VERTEX_SIZE * 0 + 0], vertices[i + VERTEX_SIZE * 0 + 1], vertices[i + VERTEX_SIZE * 0 + 2]);
		glm::vec3 pos2(vertices[i + VERTEX_SIZE * 1 + 0], vertices[i + VERTEX_SIZE * 1 + 1], vertices[i + VERTEX_SIZE * 1 + 2]);
		glm::vec3 pos3(vertices[i + VERTEX_SIZE * 2 + 0], vertices[i + VERTEX_SIZE * 2 + 1], vertices[i + VERTEX_SIZE * 2 + 2]);
		glm::vec2 uv1(vertices[i + VERTEX_SIZE * 0 + 6], vertices[i + VERTEX_SIZE * 0 + 7]);
		glm::vec2 uv2(vertices[i + VERTEX_SIZE * 1 + 6], vertices[i + VERTEX_SIZE * 1 + 7]);
		glm::vec2 uv3(vertices[i + VERTEX_SIZE * 2 + 6], vertices[i + VERTEX_SIZE * 2 + 7]);
		glm::vec3 edge1 = pos2 - pos1;
		glm::vec3 edge2 = pos3 - pos1;
		glm::vec2 deltaUV1 = uv2 - uv1;
		glm::vec2 deltaUV2 = uv3 - uv1;
		float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);
		glm::vec3 tangent;
		// tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
		// tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
		// tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
		tangent = (deltaUV1.y * edge2 - deltaUV2.y * edge1) * f;
		tangent = glm::normalize(tangent);
		vertices[i + VERTEX_SIZE * 0 + 8] = tangent.x;
		vertices[i + VERTEX_SIZE * 0 + 9] = tangent.y;
		vertices[i + VERTEX_SIZE * 0 + 10] = tangent.z;
		vertices[i + VERTEX_SIZE * 1 + 8] = tangent.x;
		vertices[i + VERTEX_SIZE * 1 + 9] = tangent.y;
		vertices[i + VERTEX_SIZE * 1 + 10] = tangent.z;
		vertices[i + VERTEX_SIZE * 2 + 8] = tangent.x;
		vertices[i + VERTEX_SIZE * 2 + 9] = tangent.y;
		vertices[i + VERTEX_SIZE * 2 + 10] = tangent.z;

		// std::cout << "Normal: " << vertices[i + 3] << " " << vertices[i + 4] << " " << vertices[i + 5] << std::endl;
		// std::cout << "Tangent: " << tangent.x << " " << tangent.y << " " << tangent.z << std::endl;
	}
}
}

// uva = tv[1].x-tv[0].x;
// uvb = tv[2].x-tv[0].x;

// uvc = tv[1].y-tv[0].y;
// uvd = tv[2].y-tv[0].y;

// uvk = uvb*uvc - uva*uvd;
// uvk = deltaUV2.x * deltaUV1.y - deltaUV1.x * deltaUV2.y

// v1 = v[1]-v[0];
// v2 = v[2]-v[0];

// if (uvk!=0) 
// {
// 	tvec[0] = (uvc*v2-uvd*v1)/uvk;
// 	tvec[1] = (uva*v2-uvb*v1)/uvk;
// 	tvec[0] = (deltaUV1.y * edge2 - deltaUV2.y * edge1) / delta;
// 	tvec[1] = (deltaUV1.x * edge2 - deltaUV2.x * edge1) / delta;
// }

std::vector<float> Geometry::ReadGeometry(std::string path) {
	//? Code copied from https://github.com/syoyo/tinyobjloader

	std::vector<float> res;

	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string warn;
	std::string err;

	bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, path.c_str());
	if (!warn.empty()) {
		Log::Warning("[ReadGeometry]\n" + warn);
	}
	if (!err.empty()) {
		Log::Error("[ReadGeometry]\n" + err);
	}
	if (!ret) {
		Log::Error("[ReadGeometry]\nError");
		exit(1);
	}
	size_t index_offset = 0;
	for (size_t f = 0; f < shapes[0].mesh.num_face_vertices.size(); f++) {
		int fv = shapes[0].mesh.num_face_vertices[f];

		// Loop over vertices in the face.
		for (size_t v = 0; v < fv; v++) {
			// access to vertex
			tinyobj::index_t idx = shapes[0].mesh.indices[index_offset + v];
			res.push_back(attrib.vertices [3 * idx.vertex_index   + 0]);
			res.push_back(attrib.vertices [3 * idx.vertex_index   + 1]);
			res.push_back(attrib.vertices [3 * idx.vertex_index   + 2]);
			res.push_back(attrib.normals  [3 * idx.normal_index   + 0]);
			res.push_back(attrib.normals  [3 * idx.normal_index   + 1]);
			res.push_back(attrib.normals  [3 * idx.normal_index   + 2]);
			res.push_back(attrib.texcoords[2 * idx.texcoord_index + 0]);
			res.push_back(attrib.texcoords[2 * idx.texcoord_index + 1]);
			res.push_back(0.f);
			res.push_back(0.f);
			res.push_back(0.f);
		}
		index_offset += fv;
	}
	res.shrink_to_fit();
	ResolveTangents(res);
	return res;
}

Geometry::Geometry(std::string path) {
	std::vector<float> buffer = ReadGeometry(path);
	Init(buffer);
}

Geometry::Geometry(std::vector<float> buffer) {
	Init(buffer);
}

void Geometry::Init(std::vector<float> buffer) {
	_polygonCount = buffer.size() / (VERTEX_SIZE * 3);
	glGenVertexArrays(1, &_vao);
	glGenBuffers(1, &_vbo);

	glBindVertexArray(_vao);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);

	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * buffer.size(), &buffer[0], GL_STATIC_DRAW);
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, VERTEX_SIZE * sizeof(float), nullptr);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, VERTEX_SIZE * sizeof(float), (void*)(3 * sizeof(float)));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, VERTEX_SIZE * sizeof(float), (void*)(6 * sizeof(float)));
	
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
};

Geometry::~Geometry() {
	glDeleteBuffers(1, &_vbo);
	glDeleteVertexArrays(1, &_vao);
};

uint Geometry::GetPolygonCount() {return _polygonCount;}
uint Geometry::GetId() {return _vao;};

void Geometry::Use() {
	glBindVertexArray(_vao);
}

