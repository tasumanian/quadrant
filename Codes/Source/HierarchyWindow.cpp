#include "HierarchyWindow.h"
#include "Components.h"
#include "GameObject.h"
#include "Editor.h"

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

    for (auto& object : scene.GetObjects())
    {
        GameObject* obj = object.get();
        bool selected =
            (obj == editor.GetSelectedObject());

        if (ImGui::Selectable(
            obj->name.c_str(),
            selected))
        {
            editor.Select(obj);
        }
    }
    ImGui::End();
}