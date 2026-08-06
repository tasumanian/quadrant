#include "Editor/HierarchyWindow.h"
#include "Component/Components.h"
#include "ResourceManager/GameObject.h"

void HierarchyWindow::Draw(Scene& scene)
{
    ImGui::SetNextWindowPos(
        ImVec2(0, 20),
        ImGuiCond_Once
    );

    ImGui::SetNextWindowSize(
        ImVec2(250, 500),
        ImGuiCond_Once
    );

    ImGui::Begin("Hierarchy");

    for (GameObject& obj : scene.GetObjects())
    {
        bool selected =
            (&obj == m_selection);

        if (ImGui::Selectable(
            obj.name.c_str(),
            selected))
        {
            m_selection = &obj;
        }
    }
    ImGui::End();
}