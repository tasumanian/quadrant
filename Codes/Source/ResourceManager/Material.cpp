#include "ResourceManager/Material.h"



Material::Material(Shader* setShader, Texture* setTexture)
{
	shader = setShader;
	texture = setTexture;
}
Shader* Material::GetShader()
{
	return shader;
}
Texture* Material::GetTexture()
{
	return texture;
}