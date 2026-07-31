#include "renderer.h"
#include "window.h"
#include <SDL3/SDL_mouse.h>


bool Renderer::Initialize(Window& window, Timer& timer)
{
    SDL_Log("Renderer Initialize");
    //SDL_Log("%s", kVertexShader);
    m_window = &window;
    //カメラの座標を手前にしておく
    m_timer = &timer;
    glEnable(GL_DEPTH_TEST); //深度比較
    SDL_SetWindowRelativeMouseMode(window.GetSDLWindow(), true);
    m_lastTicks = SDL_GetTicks();

    return true;
}
void Renderer::Draw(const Scene& scene ,const Camera& camera )
{
	float deltaTime = m_timer->GetDeltaTime(); //デルタタイムの取得

	glm::mat4 view = camera.GetViewMatrix(); //カメラ軸行列の取得
    //投影行列の生成
	glm::mat4 proj = glm::perspective(glm::radians(60.0f), 1280.0f / 720.0f, 0.1f, 100.0f);

    // 奥（青）


	for (GameObject obj : scene.GetObjects()) //オブジェクトの描写
    {

        obj.material->GetShader()->SetMat4("uView", view); //シェーダにカメラ軸行列を渡す
        obj.material->GetShader()->SetMat4("uProj", proj); //シェーダに投影行列を渡す
       
        glm::mat4 model = obj.transform.GetWorldMatrix(); //ワールド行列の取得

        obj.material->GetShader()->SetMat4("uModel",model); //シェーダにワールド行列を渡す


        obj.material->GetShader()->Use(); //シェーダの使用
        obj.material->GetTexture()->Bind(); //テクスチャのバインド

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