#include "camera.h"

glm::mat4 Camera::GetViewMatrix() const
{
    return glm::lookAt(
        position,
        position + forward,
        up
    );
}
Camera::Camera()
{
    position = glm::vec3(0.0f, 0.0f, 3.0f);

    yaw = -90.0f;
    pitch = 0.0f;

    up = glm::vec3(0.0f, 1.0f, 0.0f);

    UpdateVectors();
}
void Camera::UpdateVectors()
{
    glm::vec3 dir;

    dir.x =
        cos(glm::radians(yaw))
        * cos(glm::radians(pitch));

    dir.y =
        sin(glm::radians(pitch));

    dir.z =
        sin(glm::radians(yaw))
        * cos(glm::radians(pitch));

    forward =
        glm::normalize(dir);

    right =
        glm::normalize(
            glm::cross(
                forward,
                glm::vec3(0.0f, 1.0f, 0.0f)
            )
        );

    up =
        glm::normalize(
            glm::cross(
                right,
                forward
            )
        );
}