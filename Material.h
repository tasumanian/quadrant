#pragma once

#include "Shader.h"
#include "Texture.h"
#include <string>

class Material
{
public:
	Material(Shader* shader,Texture* texture);

private:
	Shader* shader;
	Texture* texture;

public:
	Shader* GetShader();
	Texture* GetTexture();

	std::string name;

};