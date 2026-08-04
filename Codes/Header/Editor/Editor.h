#pragma once
#include "Component/Components.h"
#include "ResourceManager/GameObject.h"

class Scene;
class Window;

class Editor
{

public:

    void Select(GameObject* obj);

    GameObject* GetSelectedObject();
    void Draw(Scene* scene);
    void Init(Window* window);

private:

	GameObject* m_selectedObject = nullptr;

    void DrawHierarchy(Scene* scene);
    void DrawInspector(Scene* scene);
    void DrawTransform(GameObject& obj);
    void DrawRigidbody(GameObject& obj);
    void DrawCollider(GameObject& obj);
    void DrawMaterial(GameObject& obj);
    void DrawBoxCollider(GameObject& obj);
};