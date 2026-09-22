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

    m_inspectorWindow.Draw(*scene, *this);
    m_hierarchyWindow.Draw(*scene, *this);

    ImGui::Render();

    ImGui_ImplOpenGL3_RenderDrawData(
        ImGui::GetDrawData());

}
void Editor::Select(std::unique_ptr<GameObject>* obj)
{
    m_selectedObject = obj;
}

std::unique_ptr<GameObject>* Editor::GetSelectedObject()
{
    return m_selectedObject;
}