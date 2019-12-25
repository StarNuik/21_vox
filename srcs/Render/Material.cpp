#include <GL/glew.h>
#include <string>

#include "Types.h"
#include "Render/Material.h"
#include "Render/Texture.h"
#include "Render/Shader.h"
#include "Utilities/Log.h"

Material::Material(std::string dirPath, float shininess) {
	_diffuseMap.Load(dirPath + "_diffuse.png");
	_normalMap.Load(dirPath + "_normal.png");
	_specularMap.Load(dirPath + "_specular.png");
	_shininess = shininess;
	if (!_diffuseMap.IsLoaded()) {
		Log::Error("[Material::Material]\nCouldn't load diffuse map: " + dirPath + "_diffuse.png");
		exit(1);
	}
}

Material::~Material() {};

void Material::Use(Shader* shader) {
	shader->SetInt("material.diffuse", 0);
	glActiveTexture(GL_TEXTURE0);
	_diffuseMap.Use();

	shader->SetInt("material.normal", 1);
	glActiveTexture(GL_TEXTURE1);
	if (_normalMap.IsLoaded()) {
		_normalMap.Use();
	} else {
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	shader->SetInt("material.specular", 2);
	glActiveTexture(GL_TEXTURE2);
	if (_specularMap.IsLoaded()) {
		_specularMap.Use();
	} else {
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	shader->SetFloat("material.shininess", _shininess);
};