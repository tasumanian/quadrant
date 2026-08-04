#pragma once

#include <glm/glm.hpp>

class CameraComponent;
class Ray;

class CameraSystem
{
public:

    glm::mat4 GetViewMatrix(
        const CameraComponent* camera
    ) const;

    glm::mat4 GetProjectionMatrix(
        const CameraComponent* camera,
        float aspect
    ) const;

    Ray ScreenPointToRay(
        const CameraComponent* camera,
        float mouseX,
        float mouseY,
        float width,
        float height
    ) const;
};