#pragma once
#include <unordered_map>
#include <memory>
#include <string>
#include "Mesh.h"
#include "Texture.h"
#include "Shader.h"
#include "Material.h"

class ResourceManager
{
	private:
        std::unordered_map<std::string, std::unique_ptr<Mesh>>
			m_meshes;

        std::unordered_map<std::string, std::unique_ptr<Texture>>
            m_textures;

        std::unordered_map<std::string, std::unique_ptr<Shader>>
			m_shaders;
        std::unordered_map<std::string, std::unique_ptr<Material>>
            m_materials;

    public:

            Mesh* AddMesh(
                const std::string&,
                std::unique_ptr<Mesh> mesh
            );

            Mesh* GetMesh(
                const std::string& name
            );

            bool AddTexture(
                const std::string& name,
                std::unique_ptr<Texture> texture
            );

            Texture* GetTexture(
                const std::string& name
            );

            bool AddShader(
                const std::string& name,
                std::unique_ptr<Shader> shader
            );

            Shader* GetShader(
                const std::string& name
            );
            bool AddMaterial(
                const std::string& name,
                std::unique_ptr<Material> material
            );

            Material* GetMaterial(
                const std::string& name
            );

 };