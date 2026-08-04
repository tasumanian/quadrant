#include "ResourceManager/CameraSystem.h"
#include "ResourceManager/GameObject.h"
#include "Component/CameraComponent.h"
#include "ResourceManager/Ray.h"

glm::mat4 CameraSystem::GetViewMatrix(
    const CameraComponent* camera
) const
{
    const Transform& tr =
        camera->gameObject->transform;

    return glm::lookAt(
        tr.position,
        tr.position + tr.Forward(),
        tr.Up()
    );
}
glm::mat4 CameraSystem::GetProjectionMatrix(
    const CameraComponent* camera,
    float aspect
) const
{
    if (camera->orthographic)
    {
        float size =
            camera->orthographicSize;

        return glm::ortho(
            -size * aspect,
            size * aspect,
            -size,
            size,
            camera->nearClip,
            camera->farClip);
    }

    return glm::perspective(
        glm::radians(camera->fov),
        aspect,
        camera->nearClip,
        camera->farClip);
}
Ray CameraSystem::ScreenPointToRay(
    const CameraComponent* camera,
    float mouseX,
    float mouseY,
    float width,
    float height
) const
{
    // ① 画面座標→NDC(-1～1)
    float x =
        (2.0f * mouseX) / width - 1.0f;

    float y =
        1.0f - (2.0f * mouseY) / height;

    // OpenGLではNear面を-1とする
    glm::vec4 rayClip(
        x,
        y,
        -1.0f,
        1.0f
    );

    // ② Projection行列の逆
    glm::mat4 proj =
        glm::perspective(
            glm::radians(60.0f),
            width / height,
            0.1f,
            100.0f
        );

    glm::vec4 rayEye =
        glm::inverse(proj) * rayClip;

    // 方向ベクトルにする
    rayEye.z = -1.0f;
    rayEye.w = 0.0f;

    // ③ View行列の逆
    glm::vec4 rayWorld =
        glm::inverse(GetViewMatrix(camera))
        * rayEye;

    glm::vec3 direction =
        glm::normalize(glm::vec3(rayWorld));

    return Ray(
        camera->gameObject->transform.position,
        direction
    );
}