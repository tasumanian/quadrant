#include "renderer.h"
#include "window.h"
#include <SDL3/SDL_mouse.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

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

    int width;
    int height;
    int channels;

    unsigned char* data =
        stbi_load(
            "assets/test.png", //RGBA8で読み込む
            &width,
            &height,
            &channels,
            4
        );
    if (!data)
    {
        SDL_Log("Failed to load texture");
        return false;
    }
    glGenTextures(1, &m_texture);

    glBindTexture(GL_TEXTURE_2D, m_texture);

    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGBA8,
        width,
        height,
        0,
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        data
    );
    glTexParameteri(
        GL_TEXTURE_2D,
        GL_TEXTURE_MIN_FILTER,
        GL_LINEAR
    );

    glTexParameteri(
        GL_TEXTURE_2D,
        GL_TEXTURE_MAG_FILTER,
        GL_LINEAR
    );
    glUseProgram(m_shader.GetProgram());

    GLint texLocation =
        glGetUniformLocation(
            m_shader.GetProgram(),
            "uTexture"
        );

    glUniform1i(texLocation, 0);
    stbi_image_free(data);
    //メッシュの作成()
    if (!m_mesh.CreateObject())
    {
        return false;
    }
    m_object.mesh = &m_mesh;
    //カメラの座標を手前にしておく

    glEnable(GL_DEPTH_TEST); //深度比較
    SDL_SetWindowRelativeMouseMode(
        window.GetSDLWindow(),
        true
    ); //マウス操作の有効か

    m_lastTicks = SDL_GetTicks();
    return true;
}
void Renderer::Draw()
{
    Uint64 currentTicks = SDL_GetTicks();

    float deltaTime =
        (currentTicks - m_lastTicks) / 1000.0f;

    m_lastTicks = currentTicks;
    float rotationSpeed = 1.0f;

    float mouseSensitivity = 0.05f;

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


    float moveSpeed = 0.2f;

    if (StateInput::IsKeyDown(SDL_SCANCODE_W))
    {
        m_camera.position +=
            m_camera.forward * moveSpeed;
    }

    if (StateInput::IsKeyDown(SDL_SCANCODE_S))
    {
        m_camera.position -=
            m_camera.forward * moveSpeed;
    }

    if (StateInput::IsKeyDown(SDL_SCANCODE_A))
    {
        m_camera.position -=
            m_camera.right * moveSpeed;
    }

    if (StateInput::IsKeyDown(SDL_SCANCODE_D))
    {
        m_camera.position +=
            m_camera.right * moveSpeed;
    }


    m_shader.Bind();

    glm::mat4 view = m_camera.GetViewMatrix();
    glm::mat4 proj =
        glm::perspective(
            glm::radians(60.0f),
            1280.0f / 720.0f,
            0.1f,
            100.0f
        );

    m_shader.SetMat4("uView", view);
    m_shader.SetMat4("uProj", proj);

    // 奥（青）
    m_object.transform.position =
        glm::vec3(-1, 0, -2);

    glm::mat4 modelBack =
        m_object.transform.GetMatrix();
    m_shader.SetMat4("uModel", modelBack);
    m_shader.SetVec3("uColor", 0.2f, 0.3f, 1.0f);
    m_mesh.Draw();

    // 手前（赤）
    m_object.transform.position =
        glm::vec3(1, 0, -2);
    glm::mat4 modelFront =
        m_object.transform.GetMatrix();
    m_shader.SetMat4("uModel", modelFront);
    m_shader.SetVec3("uColor", 1.0f, 0.2f, 0.2f);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_texture);

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