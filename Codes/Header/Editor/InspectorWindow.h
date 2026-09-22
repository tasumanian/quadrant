#pragma once
#include "Editor/EditorWindow.h"

class Editor;
class InspectorWindow :
    public EditorWindow
{
public:

    void Draw(Scene& scene,Editor& editor) override;

    void DrawComponent(Component* component);

    void DrawProperty(PropertyBase* property);
};