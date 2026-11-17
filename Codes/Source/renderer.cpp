#include "Renderer.h"
#include "Window.h"
#include "CameraComponent.h"
#include <SDL3/SDL_mouse.h>
#include "GameObject.h"

bool Renderer::Initialize(Window& window, Timer& timer , CameraSystem& cameraSystem)
{
    SDL_Log("Renderer Initialize");
    m_window = &window;
    m_cameraSystem = &cameraSystem;
    m_timer = &timer;
    glEnable(GL_DEPTH_TEST);

    // VSync: lets the display refresh pace the loop instead of SDL_Delay
    if (!SDL_GL_SetSwapInterval(1))
        SDL_Log("VSync not available: %s", SDL_GetError());
    m_lastTicks = SDL_GetTicks();

    return true;
}

void Renderer::Draw(Scene& scene)
{
    float deltaTime = m_timer->GetDeltaTime();
    (void)deltaTime;

    CameraComponent* camera = scene.GetMainCamera();

    if (camera == nullptr)
    {
        SDL_Log("No camera found in the scene.");
        return;
    }

    float aspect = (float)1280 / (float)720;

    glm::mat4 view = m_cameraSystem->GetViewMatrix(camera);
    glm::mat4 proj = m_cameraSystem->GetProjectionMatrix(camera, aspect);

    // Frustum for culling
    m_frustum.Update(proj * view);
    m_drawnCount = 0;
    m_culledCount = 0;

    for (auto& objPtr : scene.GetObjects())
    {
        if (!objPtr->HasComponent<MeshRenderer>())
            continue;

        auto& renderer = *objPtr->GetComponent<MeshRenderer>();
        if (!renderer.mesh || !renderer.material)
            continue;

        const glm::mat4 model = objPtr->GetTransform()->GetWorldMatrix();

        // Frustum culling with the mesh's bounding box moved to world space
        if (renderer.mesh->HasBounds())
        {
            const glm::vec3 bmin = renderer.mesh->GetBoundsMin();
            const glm::vec3 bmax = renderer.mesh->GetBoundsMax();
            const glm::vec3 localCenter = (bmin + bmax) * 0.5f;
            const glm::vec3 localExtents = (bmax - bmin) * 0.5f;

            const glm::vec3 worldCenter = glm::vec3(model * glm::vec4(localCenter, 1.0f));

            // World-space half extents of the transformed box
            glm::vec3 worldExtents(0.0f);
            for (int row = 0; row < 3; ++row)
            {
                worldExtents[row] = std::fabs(model[0][row]) * localExtents.x
                                  + std::fabs(model[1][row]) * localExtents.y
                                  + std::fabs(model[2][row]) * localExtents.z;
            }

            if (!m_frustum.IsBoxVisible(worldCenter, worldExtents))
            {
                ++m_culledCount;
                continue;
            }
        }

        auto* shader = renderer.material->GetShader();

        shader->Use(); // activate first: glUniform* applies to the current program
        shader->SetMat4("uView", view);
        shader->SetMat4("uProj", proj);
        shader->SetMat4("uModel", model);

        renderer.material->GetTexture()->Bind();
        renderer.mesh->Draw();

        ++m_drawnCount;
    }
}

void Renderer::BeginFrame()
{
    glClearColor(0.1f, 0.1f, 0.15f, 1.0f);

    // Clear color and depth
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
}

void Renderer::EndFrame(Window& window)
{
    window.Present();
}
