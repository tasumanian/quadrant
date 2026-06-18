#include "renderer.h"
#include "window.h"
#include <SDL3/SDL_mouse.h>



bool Renderer::Initialize(Window& window)
{
    SDL_Log("Renderer Initialize");
    //SDL_Log("%s", kVertexShader);
    m_window = &window;
    //カメラの座標を手前にしておく

    glEnable(GL_DEPTH_TEST); //深度比較
    SDL_SetWindowRelativeMouseMode(
        window.GetSDLWindow(),
        true
    );
    m_lastTicks = SDL_GetTicks();

    return true;
}
void Renderer::TimerSet(Timer* timer)
{
    m_timer = timer;
}
void Renderer::Draw(const Scene& scene)
{
	float deltaTime = m_timer->GetDeltaTime(); //デルタタイムの取得
    SDL_Log("Delta Time: %f", deltaTime);

    float rotationSpeed = 1.0f;

    float mouseSensitivity = 0.05f;

	//マウスの移動量を取得
    int mouseDeltaX = StateInput::GetMouseDeltaX();
    int mouseDeltaY = StateInput::GetMouseDeltaY();

    m_camera.yaw +=
        mouseDeltaX * mouseSensitivity;
    
    m_camera.pitch -=
        mouseDeltaY * mouseSensitivity;
    m_camera.UpdateVectors();
    if (m_camera.pitch > 89.0f)
        m_camera.pitch = 89.0f;

    if (m_camera.pitch < -89.0f)
        m_camera.pitch = -89.0f;

	//カメラの前方向と右方向の計算
    float forwardX =
        std::cos(m_camera.pitch) *
        std::sin(m_camera.yaw);

    float forwardY =
        std::sin(m_camera.pitch);

    float forwardZ =
        -std::cos(m_camera.pitch) *
        std::cos(m_camera.yaw);

    float rightX = std::cos(m_camera.yaw);
    float rightZ = std::sin(m_camera.yaw);


    float moveSpeed = 0.5f;

    if (StateInput::IsKeyDown(SDL_SCANCODE_W))
    {
        m_camera.position +=
            m_camera.forward * moveSpeed * deltaTime; //デルタタイムを使った時間計算
    }

    if (StateInput::IsKeyDown(SDL_SCANCODE_S))
    {
        m_camera.position -=
            m_camera.forward * moveSpeed * deltaTime;
    }

    if (StateInput::IsKeyDown(SDL_SCANCODE_A))
    {
        m_camera.position -=
            m_camera.right * moveSpeed * deltaTime;
    }

    if (StateInput::IsKeyDown(SDL_SCANCODE_D))
    {
        m_camera.position +=
            m_camera.right * moveSpeed * deltaTime;
    }
	glm::mat4 view = m_camera.GetViewMatrix(); //カメラ軸行列の取得
	glm::mat4 proj = //投影行列の生成
        glm::perspective(
            glm::radians(60.0f),
            1280.0f / 720.0f,
            0.1f,
            100.0f
        );

    // 奥（青）

	for (GameObject obj : scene.GetObjects()) //オブジェクトの描写
    {

        obj.material.shader.SetMat4("uView", view); //シェーダにカメラ軸行列を渡す
        obj.material.shader.SetMat4("uProj", proj); //シェーダに投影行列を渡す
       
        glm::mat4 model = obj.transform.GetWorldMatrix(); //ワールド行列の取得

        obj.material.shader.SetMat4("uModel",model); //シェーダにワールド行列を渡す


		obj.material.shader.Use(); //シェーダの使用
		obj.material.texture.Bind(); //テクスチャのバインド

		obj.mesh->Draw(); //メッシュの描写
    }
    //テスト用
   // m_objects[0].transform.rotation.y +=
   //     0.1f * deltaTime;


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