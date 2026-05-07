#include "renderer.h"
#include "window.h"

#include <glad/glad.h>


//頂点シェーダの設定(よくわからん)
//多分meshの形、位置の設定
static const char* kVertexShader = R"(
#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 uModel; //ワールド座標化
uniform mat4 uView; //カメラ座標化
uniform mat4 uProj; //投影化(遠近法)

out vec3 vDebug;

void main()
{

    vec4 p = uProj * uView * uModel * vec4(aPos, 1.0);
    gl_Position = p;
    vDebug = p.xyz;
}
)";

//フラグメントシェーダの設定(よくわからん)
//多分色とかの設定
static const char* kFragmentShader = R"(
#version 330 core
in vec3 vDebug;

uniform vec3 uColor;

out vec4 FragColor;

void main()
{
    FragColor = vec4(uColor, 1.0);
}
)";

bool Renderer::Initialize()
{
    SDL_Log("Renderer Initialize");
    SDL_Log("%s", kVertexShader);
    //シェーダの生成
    if (!m_shader.Create(kVertexShader, kFragmentShader)) 
    {
        return false;
    }

    //メッシュの作成(三角形)
    if (!m_mesh.CreateTriangle())
    {
        return false;
    }
    m_transform.x = 0.0f;
    m_transform.y = 0.0f;
    m_transform.z = -2.0f;

    //カメラの座標を手前にしておく
    m_camera.x = 0.0f;
    m_camera.y = 0.0f;
    m_camera.z = 0.0f;

    glEnable(GL_DEPTH_TEST); //深度比較

    return true;
}
void Renderer::Draw()
{
    m_transform.rotationY += 0.01f;

    m_shader.Bind();

    Mat4 view = m_camera.GetViewMatrix();
    Mat4 proj = Mat4::Perspective(1.0f, 800.0f / 600.0f, 0.1f, 100.0f);

    m_shader.SetMat4("uView", view);
    m_shader.SetMat4("uProj", proj);

    // 奥（青）
    m_transform.x = -0.1f;
    m_transform.y = 0.0f;
    m_transform.z = -2.5f;

    Mat4 modelBack = m_transform.ToMatrix();
    m_shader.SetMat4("uModel", modelBack);
    m_shader.SetVec3("uColor", 0.2f, 0.3f, 1.0f);
    m_mesh.Draw();

    // 手前（赤）
    m_transform.x = 0.1f;
    m_transform.y = 0.0f;
    m_transform.z = -2.0f;

    Mat4 modelFront = m_transform.ToMatrix();
    m_shader.SetMat4("uModel", modelFront);
    m_shader.SetVec3("uColor", 1.0f, 0.2f, 0.2f);
    m_mesh.Draw();
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