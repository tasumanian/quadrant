#include "Editor/Editor.h"
#include "ResourceManager/Scene.h"
#include "Core/Window.h"

#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_sdl3.h>

void Editor::Init(Window* window)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    ImGui_ImplSDL3_InitForOpenGL(window->GetSDLWindow(), window->GetGlContext());

    ImGui_ImplOpenGL3_Init("#version 330");
    m_hierarchyWindow.SetSelection(m_selectedObject);
    m_inspectorWindow.SetSelection(m_selectedObject);
}

void Editor::Draw(Scene* scene)
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL3_NewFrame();

    ImGui::NewFrame();

    if (ImGui::BeginMainMenuBar())
    {
        ImGui::Text("Quadrant Engine");

        ImGui::EndMainMenuBar();
    }

    m_inspectorWindow.Draw(*scene);
    m_hierarchyWindow.Draw(*scene);

    ImGui::Render();

    ImGui_ImplOpenGL3_RenderDrawData(
        ImGui::GetDrawData());

}
void Editor::Select(GameObject* obj)
{
    m_selectedObject = obj;
    m_hierarchyWindow.SetSelection(obj);
    m_inspectorWindow.SetSelection(obj);
}

GameObject* Editor::GetSelectedObject()
{
    return m_selectedObject;
}