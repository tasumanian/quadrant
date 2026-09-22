#pragma once
#include "ResourceManager/Scene.h"
#include "Core/Window.h"
#include "SelectionContext.h"

#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_sdl3.h>

class Editor;

class EditorWindow
{
public:

    virtual ~EditorWindow() = default;

    virtual void Draw(Scene& scene, Editor& editor) = 0;
};