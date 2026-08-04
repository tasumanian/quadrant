#include "ResourceManager/ResourceManager.h"

Mesh* ResourceManager::AddMesh(
    const std::string& name,
    std::unique_ptr<Mesh> mesh
)
{
    Mesh* ptr = mesh.get();

    m_meshes.emplace(
        name,
        std::move(mesh)
    );

    return ptr;
}
Mesh* ResourceManager::GetMesh(
    const std::string& name
)
{
    auto it =
        m_meshes.find(name);

    if (it == m_meshes.end())
    {
        return nullptr;
    }

    return it->second.get();
}
bool ResourceManager::AddTexture(
    const std::string& name,
    std::unique_ptr<Texture> texture
)
{
    return
        m_textures.emplace(
            name,
            std::move(texture)
        ).second;
}
Texture* ResourceManager::GetTexture(
    const std::string& name
)
{
    auto it =
        m_textures.find(name);

    if (it == m_textures.end())
    {
        return nullptr;
    }

    return it->second.get();
}
bool ResourceManager::AddShader(
    const std::string& name,
    std::unique_ptr<Shader> shader
)
{
    return
        m_shaders.emplace(
            name,
            std::move(shader)
        ).second;
}
Shader* ResourceManager::GetShader(
    const std::string& name
)
{
    auto it =
        m_shaders.find(name);

    if (it == m_shaders.end())
    {
        return nullptr;
    }

    return it->second.get();
}
bool ResourceManager::AddMaterial(
    const std::string& name,
    std::unique_ptr<Material> material
)
{
    return
        m_materials.emplace(
            name,
            std::move(material)
        ).second;
}
Material* ResourceManager::GetMaterial(
    const std::string& name
)
{
    auto it =
        m_materials.find(name);

    if (it == m_materials.end())
    {
        return nullptr;
    }

    return it->second.get();
}