#pragma once

class Scene;

class Editor
{
public:

    void Draw(Scene* scene);

private:

    int m_selectedObject = -1;

    void DrawHierarchy(Scene* scene);
    void DrawInspector(Scene* scene);
};