#include "CharacterController.h"
#include "StateInput.h"

#include <SDL3/SDL.h>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

CharacterController::CharacterController()
{
    moveSpeed = 3.0f;
}
void CharacterController::Update(
    GameObject& player,
    float yaw,
    float deltaTime
)
{
    glm::vec3 moveDir(0.0f);

    glm::vec3 forward =
    {
        std::sin(yaw),
        0.0f,
        -std::cos(yaw)
    };

    glm::vec3 right =
    {
        std::cos(yaw),
        0.0f,
        std::sin(yaw)
    };

    if (StateInput::IsKeyDown(SDL_SCANCODE_W))
    {
        moveDir += forward;
    }

    if (StateInput::IsKeyDown(SDL_SCANCODE_S))
    {
        moveDir -= forward;
    }

    if (StateInput::IsKeyDown(SDL_SCANCODE_D))
    {
        moveDir += right;
    }

    if (StateInput::IsKeyDown(SDL_SCANCODE_A))
    {
        moveDir -= right;
    }
    if (
        StateInput::IsKeyDown(
            SDL_SCANCODE_SPACE
        )
        &&
        player.rigidbody.isGrounded
        )
    {
        player.rigidbody.velocity.y =
            5.0f;
    }
    if (glm::length(moveDir) > 0.0f)
    {
        moveDir =
            glm::normalize(moveDir);
    }

    player.transform.position +=
        moveDir *
        moveSpeed *
        deltaTime;
}