#include "Editor/HierarchyWindow.h"
#include "Component/Components.h"
#include "ResourceManager/GameObject.h"
#include "Editor/Editor.h"

void HierarchyWindow::Draw(Scene& scene, Editor& editor)
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

    for (auto& obj : scene.GetObjects())
    {
        bool selected =
            (&obj == editor.GetSelectedObject());

        if (ImGui::Selectable(
            obj->name.c_str(),
            selected))
        {
            editor.Select(&obj);
        }
    }
    ImGui::End();
}