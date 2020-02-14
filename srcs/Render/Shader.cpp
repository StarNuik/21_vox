// #include "Render/Objects.h"
// #include "Utilities/Utilities.h"
// #include "Engine/Log.h"
#include <string>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Render/Shader.h"
#include "Utilities/Utilities.h"
#include "Utilities/Log.h"
// #include "Types.h"

void CheckShaderError(uint index, std::string source) {
	int		params = -1;
	glGetShaderiv(index, GL_COMPILE_STATUS, &params);
	if (params != GL_TRUE) {
		Log::Error("[CheckShaderError]\nShader (" + source + ") didn't compile.");
		int		max_length = 4096;
		int		actual_length = 0;
		char	shader_log[max_length];
		glGetShaderInfoLog(index, max_length, &actual_length, shader_log);
		Log::Basic(std::string(shader_log));
		//! Something is wrong here!
		glfwTerminate();
		exit(1);
	}
}

Shader::Shader(std::string vertexPath, std::string fragmentPath) {
	_locationMap = std::unordered_map<std::string, Location>();
	char* vsSource = Utilities::ReadFile(vertexPath);
	char* fsSource = Utilities::ReadFile(fragmentPath);

	_vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(_vs, 1, &vsSource, nullptr);
	glCompileShader(_vs);
	CheckShaderError(_vs, vsSource);
	delete vsSource;

	_fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(_fs, 1, &fsSource, nullptr);
	glCompileShader(_fs);
	CheckShaderError(_fs, fsSource);
	delete fsSource;

	_id = glCreateProgram();
	glAttachShader(_id, _vs);
	glAttachShader(_id, _fs);
	glLinkProgram(_id);
};

Shader::~Shader() {
	glDeleteProgram(_id);
	glDeleteShader(_vs);
	glDeleteShader(_fs);
};

void Shader::Use() {
	glUseProgram(_id);
}

#include <iostream>
int Shader::GetUniformLocation(std::string name, std::string mod) {
	Location& element = _locationMap[name + mod];
	if (element.initialized) {
		return element.location;
	} else {
		element.location = glGetUniformLocation(_id, name.c_str());
		element.initialized = true;
		if (element.location < 0) {
			Log::Warning("[Shader::GetUniformLocation]\n" + name + " has location " + std::to_string(element.location));
		}
		// _locationMap[name + mod] = location;
		return element.location;
	}
}

uint Shader::GetId() {return _id;};

void Shader::SetInt(std::string variableName, int value) {
	// glUniform1i(glGetUniformLocation(_id, variableName.c_str()), value);
	int location = GetUniformLocation(variableName, "|i");
	if (location < 0)
		return;
	glUniform1i(location, value);
}

void Shader::SetFloat(std::string variableName, float value) {
	// glUniform1f(glGetUniformLocation(_id, variableName.c_str()), value);
	int location = GetUniformLocation(variableName, "|f");
	if (location < 0)
		return;
	glUniform1f(location, value);
}

void Shader::SetFloat2(std::string variableName, float x, float y) {
	// glUniform2f(glGetUniformLocation(_id, variableName.c_str()), x, y);
	int location = GetUniformLocation(variableName, "|f2");
	if (location < 0)
		return;
	glUniform2f(location, x, y);
}

void Shader::SetFloat3(std::string variableName, float x, float y, float z) {
	// glUniform3f(glGetUniformLocation(_id, variableName.c_str()), x, y, z);
	int location = GetUniformLocation(variableName, "|f3");
	if (location < 0)
		return;
	glUniform3f(location, x, y, z);
}

void Shader::SetFloat4(std::string variableName, float x, float y, float z, float w) {
	// glUniform4f(glGetUniformLocation(_id, variableName.c_str()), x, y, z, w);
	int location = GetUniformLocation(variableName, "|f4");
	if (location < 0)
		return;
	glUniform4f(location, x, y, z, w);
}

void Shader::SetFloat2(std::string variableName, glm::vec2 vec) {
	// glUniform2f(glGetUniformLocation(_id, variableName.c_str()), vec.x, vec.y);
	int location = GetUniformLocation(variableName, "|f2");
	if (location < 0)
		return;
	glUniform2f(location, vec.x, vec.y);
}

void Shader::SetFloat3(std::string variableName, glm::vec3 vec) {
	// glUniform3f(glGetUniformLocation(_id, variableName.c_str()), vec.x, vec.y, vec.z);
	int location = GetUniformLocation(variableName, "|f3");
	if (location < 0)
		return;
	glUniform3f(location, vec.x, vec.y, vec.z);
}

void Shader::SetFloat4(std::string variableName, glm::vec4 vec) {
	// glUniform4f(glGetUniformLocation(_id, variableName.c_str()), vec.x, vec.y, vec.z, vec.w);
	int location = GetUniformLocation(variableName, "|f4");
	if (location < 0)
		return;
	glUniform4f(location, vec.x, vec.y, vec.z, vec.w);
}

void Shader::SetMatrix4(std::string variableName, glm::mat4 value) {
	// glUniformMatrix4fv(glGetUniformLocation(_id, variableName.c_str()), 1, GL_FALSE, glm::value_ptr(value));
	int location = GetUniformLocation(variableName, "|m4");
	if (location < 0)
		return;
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
}
