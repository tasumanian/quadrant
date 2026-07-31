#include "Editor.h"
#include "Scene.h"

#include <imgui.h>

void Editor::Draw(Scene* scene)
{
    DrawHierarchy(scene);
    DrawInspector(scene);
}
void Editor::DrawHierarchy(Scene* scene)
{
    ImGui::Begin("Hierarchy");

    auto& objects = scene->GetObjects();

    for (int i = 0; i < objects.size(); i++)
    {
        if (ImGui::Selectable(objects[i].name.c_str(),
            i == m_selectedObject))
        {
            m_selectedObject = i;
        }
    }

    ImGui::End();
}
void Editor::DrawInspector(Scene* scene)
{
    ImGui::Begin("Inspector");

    auto& objects = scene->GetObjects();

    if (m_selectedObject >= 0 &&
        m_selectedObject < objects.size())
    {
        GameObject& obj =
            objects[m_selectedObject];

        ImGui::Text("%s", obj.name.c_str());
    }

    ImGui::End();
}