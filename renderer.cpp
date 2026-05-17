#include "renderer.h"
#include "window.h"
#include <SDL3/SDL_mouse.h>

//頂点シェーダの設定(よくわからん)
//多分meshの形、位置の設定
static const char* kVertexShader = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aUV;
layout(location = 2) in vec3 aNormal;

uniform mat4 uModel; //ワールド座標化
uniform mat4 uView; //カメラ座標化
uniform mat4 uProj; //投影化(遠近法)
out vec3 vNormal; //法線ベクトル

out vec2 vUV; //UV座標

void main()
{

    gl_Position =
        uProj *
        uView *
        uModel *
        vec4(aPos, 1.0);

    vUV = aUV;
    vNormal = aNormal;  
}
)";

//フラグメントシェーダの設定(よくわからん)
//多分色とかの設定
static const char* kFragmentShader = R"(
#version 330 core

in vec2 vUV;
in vec3 vNormal;

uniform sampler2D uTexture;

out vec4 FragColor;

void main()
{
    //uv座標を元にテクスチャから色を取得
     vec3 lightDir =
        normalize(vec3(1.0, 1.0, 1.0));

    float diffuse = //ライティング
        max(dot(vNormal, lightDir), 0.0);

    vec3 texColor = //UV+テクスチャー
        texture(uTexture, vUV).rgb;

    float ambient = 0.2; //最低保証の光

    vec3 finalColor = //合成
        texColor * (diffuse + ambient);

    FragColor =
        vec4(finalColor, 1.0);
}
)";

bool Renderer::Initialize(Window& window)
{
    SDL_Log("Renderer Initialize");
    SDL_Log("%s", kVertexShader);
    m_window = &window;
    //シェーダの生成
    if (!m_shader.Create(kVertexShader, kFragmentShader)) 
    {
        return false;
    }

    m_material.shader = &m_shader;
    m_material.texture = &m_texture;

	m_texture.Load("assets/test.png");

    GLint texLocation =
        glGetUniformLocation(
            m_shader.GetProgram(),
            "uTexture"
        );

    glUniform1i(texLocation, 0);
    //メッシュの作成()
    if (!m_mesh.CreateObject())
    {
        return false;
    }
    //カメラの座標を手前にしておく

    glEnable(GL_DEPTH_TEST); //深度比較
    SDL_SetWindowRelativeMouseMode(
        window.GetSDLWindow(),
        true
    ); //マウス操作の有効か

    m_lastTicks = SDL_GetTicks();


    GameObject obj1; //オブジェクトの生成
    obj1.mesh = &m_mesh;
    obj1.transform.position =
        glm::vec3(-2.0f, 0.0f, -5.0f);
    obj1.material = &m_material;
    obj1.rigidbody =
        &m_rigidbody;

    m_objects.push_back(obj1); //オブジェクトの追加

    // 手前（赤）
    GameObject obj2;
    obj2.mesh = &m_mesh;
    obj2.transform.position =
        glm::vec3(0.0f, 0.0f, -5.0f);
    obj2.material = &m_material;
    obj2.rigidbody =
        &m_rigidbody;

    m_objects.push_back(obj2);

    GameObject obj3;
    obj3.mesh = &m_mesh;
    obj3.transform.position =
        glm::vec3(2.0f, 0.0f, -5.0f);
    obj3.material = &m_material;
    obj3.rigidbody =
        &m_rigidbody;

    m_objects.push_back(obj3);

    m_objects[1].transform.parent =
        &m_objects[0].transform;

    return true;
}
void Renderer::TimerSet(
    Timer* timer
)
{
    m_timer = timer;
}
void Renderer::Draw()
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

	m_shader.SetMat4("uView", view); //シェーダにカメラ軸行列を渡す
	m_shader.SetMat4("uProj", proj); //シェーダに投影行列を渡す

    // 奥（青）

	for (GameObject& obj : m_objects) //オブジェクトの描写
    {
       glm::mat4 model =
		   obj.transform.GetWorldMatrix(); //ワールド行列の取得

	   m_shader.SetMat4( //シェーダにワールド行列を渡す
            "uModel",
            model
        );

		obj.material->shader->Use(); //シェーダの使用
		obj.material->texture->Bind(); //テクスチャのバインド

		obj.mesh->Draw(); //メッシュの描写
    }
    //テスト用
    m_objects[0].transform.rotation.y +=
        0.1f * deltaTime;


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
std::vector<GameObject>&
Renderer::GetObjects()
{
    return m_objects;
}