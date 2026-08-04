#pragma once
#include "Component/Components.h"

class GameObject;
class Scene;
class Window;

class Editor
{
public:

    void Draw(Scene* scene);
    void Init(Window* window);

private:

    int m_selectedObject = -1;

    void DrawHierarchy(Scene* scene);
    void DrawInspector(Scene* scene);
    void DrawTransform(GameObject& obj);
    void DrawRigidbody(GameObject& obj);
    void DrawCollider(GameObject& obj);
    void DrawMaterial(GameObject& obj);
    void DrawBoxCollider(GameObject& obj);
};