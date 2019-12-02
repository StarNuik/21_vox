#pragma once

#include "Base.h"

class Shader {
	private:
		uint id, vs, fs;
	public:
		Shader(std::string vertexPath, std::string fragmentPath);
		~Shader();
		void Use();
		void SetFloat(std::string varName, float x);
		void SetFloat2(std::string varName, float x, float y);
		void SetFloat3(std::string varName, float x, float y, float z);
		void SetFloat4(std::string varName, float x, float y, float z, float w);
		void SetFloat2(std::string varName, glm::vec2 vector);
		void SetFloat3(std::string varName, glm::vec3 vector);
		void SetFloat4(std::string varName, glm::vec4 vector);
		void SetMatrix4(std::string varName, glm::mat4 matrix);
};

class Texture {
	private:
		uint id;
	public:
		Texture();
		~Texture();
		void Load(std::string path);
		void Use();
};

class Geometry {
	private:
		uint polygonCount, vbo, vao;
	public:
		Geometry();
		~Geometry();
		uint GetPolygonCount();
		void Use();
};
