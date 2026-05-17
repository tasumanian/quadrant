#include "transform.h"

Transform::Transform()
{
    position = glm::vec3(0.0f);
    rotation = glm::vec3(0.0f);
    scale = glm::vec3(1.0f);
    parent = nullptr;
}
glm::mat4 Transform::GetMatrix() const //位置、回転、スケールを行列に変換
{
    glm::mat4 mat = glm::mat4(1.0f);

    mat =
        glm::translate(
            mat,
            position
        );

    mat =
        glm::rotate(
            mat,
            rotation.x,
            glm::vec3(1, 0, 0)
        );

    mat =
        glm::rotate(
            mat,
            rotation.y,
            glm::vec3(0, 1, 0)
        );

    mat =
        glm::rotate(
            mat,
            rotation.z,
            glm::vec3(0, 0, 1)
        );

    mat =
        glm::scale(
            mat,
            scale
        );

    return mat;
}
glm::mat4 Transform::GetWorldMatrix() const
{
    glm::mat4 local =
        GetMatrix();

    if (parent)
    {
        return
            parent->GetWorldMatrix()
            *
            local;
    }

    return local;
}