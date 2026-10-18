#include "Editor.h"
#include "Scene.h"
#include "Window.h"
#include "StateInput.h"
#include "Ray.h"
#include "CameraSystem.h"
#include "PhysicsSystem.h"

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
void Editor::Select(GameObject* obj)
{
    m_selectedObject = obj;
}
void Editor::Update(Window& window, CameraSystem& cameraSystem, PhysicsSystem& physicsSystem, Scene& scene)
{
    ImGuiIO& io = ImGui::GetIO();

    CameraComponent* camera = scene.GetMainCamera();

    if (!io.WantCaptureMouse && StateInput::GetMouseButtonDown(SDL_BUTTON_LEFT))
    {
        int w, h;
        SDL_GetWindowSize(window.GetSDLWindow(), &w, &h);
        glm::vec2 mouse =
            StateInput::GetMousePosition();

        Ray ray =
            cameraSystem.ScreenPointToRay(
                camera,
                mouse.x,
                mouse.y,
                w,
                h);

        GameObject* obj =
            physicsSystem.Raycast(
                scene,
                ray);

        Select(obj);
    }
}

GameObject* Editor::GetSelectedObject()
{
    return m_selectedObject;
}