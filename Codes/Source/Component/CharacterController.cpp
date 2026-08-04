#include "Component/CharacterController.h"
#include "ResourceManager/GameObject.h"
#include "Core/StateInput.h"

#include <SDL3/SDL.h>
#include <glm/glm.hpp>
#include <algorithm>

CharacterController::CharacterController()
{
    moveSpeed = 3.0f;
}
void CharacterController::Update(CameraComponent* camera,float deltaTime)
{

    float rotationSpeed = 1.0f;
    float mouseSensitivity = 0.05f;
	Transform& cameraTr = camera->gameObject->transform;
    //マウスの移動量を取得s
	glm::vec2 mouseDelta =
		StateInput::GetMouseDelta();

    m_yaw -= mouseDelta.x * mouseSensitivity;

    m_pitch -= mouseDelta.y * mouseSensitivity;

    m_pitch = std::clamp(m_pitch, -89.0f, 89.0f);

    cameraTr.rotation =
        glm::quat(glm::vec3(
            glm::radians(m_pitch),
            glm::radians(m_yaw),
            0.0f));

    float moveSpeed = 0.8f;

    if (StateInput::GetKey(SDL_SCANCODE_W))
    {
        cameraTr.position +=
            cameraTr.Forward() * moveSpeed * deltaTime; //デルタタイムを使った時間計算
    }

    if (StateInput::GetKey(SDL_SCANCODE_S))
    {
        cameraTr.position +=
            cameraTr.Back() * moveSpeed * deltaTime;
    }

    if (StateInput::GetKey(SDL_SCANCODE_A))
    {
        cameraTr.position +=
            cameraTr.Left() * moveSpeed * deltaTime;
    }

    if (StateInput::GetKey(SDL_SCANCODE_D))
    {
        cameraTr.position +=
            cameraTr.Right() * moveSpeed * deltaTime;
    }
}