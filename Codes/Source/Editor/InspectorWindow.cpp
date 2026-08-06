#include "Editor/InspectorWindow.h"

void InspectorWindow::Draw(Scene& scene)
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

        auto& objects = scene.GetObjects();

        if (m_selection == nullptr)
        {
            ImGui::Text("No Selection");
            ImGui::End();
            return;
        }

        GameObject& obj =
            *m_selection;

        ImGui::Text("%s", obj.name.c_str());

        for (auto& component : obj.GetComponents())
        {
            DrawComponent(component.get());
        }
        ImGui::End();
}
void InspectorWindow::DrawComponent(Component* component)
{
    if (ImGui::CollapsingHeader(
        component->GetComponentName()))
    {
        for (auto& property :
            component->GetProperties())
        {
          //  DrawProperty(property.get());
        }
    }
}
void InspectorWindow::DrawProperty(
    PropertyBase* property)
{
    switch (property->GetType())
    {
        case PropertyType::Bool:
        {
            bool* value =
                static_cast<bool*>(
                    property->GetRawPointer());

            ImGui::Checkbox(
                property->GetMetadata().displayName.c_str(),
                value);

            break;
        }
        case PropertyType::Float:
        {
            float* value =
                static_cast<float*>(
                    property->GetRawPointer());

            auto& meta =
                property->GetMetadata();

            ImGui::DragFloat(
                meta.displayName.c_str(),
                value,
                meta.speed,
                meta.min,
                meta.max);

            break;
        }
        case PropertyType::Vec3:
        {
            glm::vec3* value =
                static_cast<glm::vec3*>(
                    property->GetRawPointer());

            ImGui::DragFloat3(
                property->GetMetadata().displayName.c_str(),
                &value->x);

            break;
        }
        default:
            break;
    }
}