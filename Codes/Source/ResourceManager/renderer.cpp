#include "ResourceManager/Renderer.h"
#include "Core/Window.h"
#include "Component/CameraComponent.h"
#include <SDL3/SDL_mouse.h>


bool Renderer::Initialize(Window& window, Timer& timer , CameraSystem& cameraSystem)
{
    SDL_Log("Renderer Initialize");
    //SDL_Log("%s", kVertexShader);
    m_window = &window;
    m_cameraSystem = &cameraSystem;
    //カメラの座標を手前にしておく
    m_timer = &timer;
    glEnable(GL_DEPTH_TEST); //深度比較
    m_lastTicks = SDL_GetTicks();

    return true;
}
void Renderer::Draw(Scene& scene)
{
	float deltaTime = m_timer->GetDeltaTime(); //デルタタイムの取得

    CameraComponent*
        camera =
        scene.GetMainCamera();

	if (camera == nullptr)
	{
		SDL_Log("No camera found in the scene.");
		return;
	}

    float aspect =
        (float)1280 /
        (float)720;

    glm::mat4 view =
        m_cameraSystem->GetViewMatrix(camera);

    glm::mat4 proj =
        m_cameraSystem->GetProjectionMatrix(
            camera,
            aspect);

    // 奥（青）
	for (GameObject& obj : scene.GetObjects()) //オブジェクトの描写
    {
        auto* renderer =
            obj.GetComponent<MeshRenderer>();

        if (!renderer)
            continue;

        renderer->material->GetShader()->SetMat4("uView", view); //シェーダにカメラ軸行列を渡す
        renderer->material->GetShader()->SetMat4("uProj", proj); //シェーダに投影行列を渡す

        glm::mat4 model = obj.transform.GetWorldMatrix(); //ワールド行列の取得

        renderer->material->GetShader()->SetMat4("uModel",model); //シェーダにワールド行列を渡す


        renderer->material->GetShader()->Use(); //シェーダの使用
        renderer->material->GetTexture()->Bind(); //テクスチャのバインド

		renderer->mesh->Draw(); //メッシュの描写
    }
}

void Renderer::BeginFrame() //window描写
{
	glClearColor(0.1f, 0.1f, 0.15f, 1.0f);

    //色及び深度の初期化
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
}

void Renderer::EndFrame(Window& window)
{
	window.Present(); //window更新
}