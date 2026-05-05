#include "renderer.h"
#include "window.h"

#include <glad/glad.h>


//頂点シェーダの設定(よくわからん)
static const char* kVertexShader = R"(
#version 330 core
layout (location = 0) in vec3 aPos;

void main()
{
    gl_Position = vec4(aPos, 1.0);
}
)";

//フラグメントシェーダの設定(よくわからん)
static const char* kFragmentShader = R"(
#version 330 core
out vec4 FragColor;

void main()
{
    FragColor = vec4(0.9, 0.4, 0.2, 1.0);
}
)";

bool Renderer::Initialize()
{
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

    return true;
}
void Renderer::Draw()
{
    m_shader.Bind();
    m_mesh.Draw();
}
void Renderer::BeginFrame() //window描写
{
	glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::EndFrame(Window& window)
{
	window.Present(); //window更新
}