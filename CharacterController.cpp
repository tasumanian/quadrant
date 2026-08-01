#include "CharacterController.h"
#include "StateInput.h"

#include <SDL3/SDL.h>
#include <glm/glm.hpp>

CharacterController::CharacterController()
{
    moveSpeed = 3.0f;
}
void CharacterController::CameraUpdate( Camera& camera,float deltaTime)
{

    float rotationSpeed = 1.0f;
    float mouseSensitivity = 0.05f;

    //マウスの移動量を取得s
	glm::vec2 mouseDelta =
		StateInput::GetMouseDelta();

    camera.yaw -= mouseDelta.x * mouseSensitivity;

    camera.pitch -=
        mouseDelta.y * mouseSensitivity;

    if (camera.pitch > 89.0f)
        camera.pitch = 89.0f;

    if (camera.pitch < -89.0f)
        camera.pitch = -89.0f;

    glm::quat yawRotation =
        glm::angleAxis(
            glm::radians(camera.yaw),
            glm::vec3(0, 1, 0)
        );

    glm::quat pitchRotation =
        glm::angleAxis(
            glm::radians(camera.pitch),
            glm::vec3(1, 0, 0)
        );

    camera.transform.rotation =
        yawRotation * pitchRotation;

    float moveSpeed = 0.8f;

    if (StateInput::GetKey(SDL_SCANCODE_W))
    {
        camera.transform.position +=
            camera.transform.Forward() * moveSpeed * deltaTime; //デルタタイムを使った時間計算
    }

    if (StateInput::GetKey(SDL_SCANCODE_S))
    {
        camera.transform.position +=
            camera.transform.Back() * moveSpeed * deltaTime;
    }

    if (StateInput::GetKey(SDL_SCANCODE_A))
    {
        camera.transform.position +=
            camera.transform.Left() * moveSpeed * deltaTime;
    }

    if (StateInput::GetKey(SDL_SCANCODE_D))
    {
        camera.transform.position +=
            camera.transform.Right() * moveSpeed * deltaTime;
    }
}