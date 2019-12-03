#include "Render/Objects.h"
#include "Utilities/Utilities.h"
#include "Engine/Locator.hpp"
#include <glm/gtc/type_ptr.hpp>

void	CheckShaderError(uint index, std::string source)
{
	int		params = -1;
	glGetShaderiv(index, GL_COMPILE_STATUS, &params);
	if (params != GL_TRUE) {
		Locator::getLogger()->LogError("[CheckShaderError]\nShader didn't compile.");
		int		max_length = 4096;
		int		actual_length = 0;
		char	shader_log[max_length];
		glGetShaderInfoLog(index, max_length, &actual_length, shader_log);
		Locator::getLogger()->Log(std::string(shader_log));
		//! Something is wrong here!
		glfwTerminate();
		exit(1);
	}
}

Shader::Shader(std::string vertexPath, std::string fragmentPath) {
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
	Locator::getLogger()->LogSuccess("[Shader::Shader]\nLoaded: [" + vertexPath + ", " + fragmentPath);
};

Shader::~Shader() {
	glDeleteProgram(_id);
	glDeleteShader(_vs);
	glDeleteShader(_fs);
};

void Shader::Use() {
	glUseProgram(_id);
}

void	Shader::SetFloat(std::string variableName, float value)
{
	glUniform1f(glGetUniformLocation(_id, variableName.c_str()), value);
}

void	Shader::SetFloat2(std::string variableName, float x, float y)
{
	glUniform2f(glGetUniformLocation(_id, variableName.c_str()), x, y);
}

void	Shader::SetFloat3(std::string variableName, float x, float y, float z)
{
	glUniform3f(glGetUniformLocation(_id, variableName.c_str()), x, y, z);
}

void	Shader::SetFloat4(std::string variableName, float x, float y, float z, float w)
{
	glUniform4f(glGetUniformLocation(_id, variableName.c_str()), x, y, z, w);
}

void	Shader::SetFloat2(std::string variableName, glm::vec2 vec)
{
	glUniform2f(glGetUniformLocation(_id, variableName.c_str()), vec.x, vec.y);
}

void	Shader::SetFloat3(std::string variableName, glm::vec3 vec)
{
	glUniform3f(glGetUniformLocation(_id, variableName.c_str()), vec.x, vec.y, vec.z);
}

void	Shader::SetFloat4(std::string variableName, glm::vec4 vec)
{
	glUniform4f(glGetUniformLocation(_id, variableName.c_str()), vec.x, vec.y, vec.z, vec.w);
}

void	Shader::SetMatrix4(std::string variableName, glm::mat4 value)
{
	glUniformMatrix4fv(glGetUniformLocation(_id, variableName.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}
