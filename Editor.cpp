#include "Editor.h"
#include "Scene.h"
#include "window.h"

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

    DrawHierarchy(scene);
    DrawInspector(scene);

    if (ImGui::BeginMainMenuBar())
    {
        ImGui::Text("Quadrant Engine");

        ImGui::EndMainMenuBar();
    }

    ImGui::Render();

    ImGui_ImplOpenGL3_RenderDrawData(
        ImGui::GetDrawData());
}
void Editor::DrawHierarchy(Scene* scene)
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
    ImGuiIO& io = ImGui::GetIO();

    ImGui::SetNextWindowPos(
        ImVec2(io.DisplaySize.x - 300, 20),
        ImGuiCond_Once
    );

    ImGui::SetNextWindowSize(
        ImVec2(300, 500),
        ImGuiCond_Once
    );


    ImGui::Begin("Inspector");

    auto& objects = scene->GetObjects();

    if (m_selectedObject >= 0 &&
        m_selectedObject < objects.size())
    {
        GameObject& obj =
            objects[m_selectedObject];

        ImGui::Text("%s", obj.name.c_str());

        DrawTransform(obj);
        
        DrawRigidbody(obj);

        DrawBoxCollider(obj);
    }

    ImGui::End();
}
void Editor::DrawTransform(GameObject& obj)
{
    ImGui::Separator();

    ImGui::Text("Transform");

    ImGui::DragFloat3(
        "Position",
        &obj.transform.position.x,
        0.1f
    );

    glm::vec3 euler =
        glm::degrees(
            glm::eulerAngles(
                obj.transform.rotation
            )
        );

    if (ImGui::DragFloat3(
        "Rotation",
        &euler.x,
        1.0f
    ))
    {
        obj.transform.rotation =
            glm::quat(
                glm::radians(euler)
            );
    }

    ImGui::DragFloat3(
        "Scale",
        &obj.transform.scale.x,
        0.1f
    );
}
void Editor::DrawRigidbody(GameObject& obj)
{
    if (ImGui::CollapsingHeader("Rigidbody"))
    {
        ImGui::Checkbox(
            "Use Gravity",
            &obj.rigidbody.useGravity
        );

        ImGui::DragFloat(
            "Mass",
            &obj.rigidbody.mass,
            0.1f,
            0.1f,
            100.0f
        );

        ImGui::Text(
            "Velocity %.2f %.2f %.2f",
            obj.rigidbody.velocity.x,
            obj.rigidbody.velocity.y,
            obj.rigidbody.velocity.z
        );
    }
}
void Editor::DrawBoxCollider(GameObject& obj)
{
    if (ImGui::CollapsingHeader("Box Collider"))
    {
        ImGui::DragFloat3(
            "Size",
            &obj.boxCollider.size.x,
            0.1f
        );
    }
}