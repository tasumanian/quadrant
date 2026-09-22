#pragma once

#include "Editor/EditorWindow.h"

class Editor;
class HierarchyWindow :
    public EditorWindow
{
public:

    void Draw(Scene& scene,Editor& editor) override;

};