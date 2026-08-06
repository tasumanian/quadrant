#pragma once
#include "Component/Components.h"
#include "ResourceManager/GameObject.h"
#include "Editor/InspectorWindow.h"
#include "Editor/HierarchyWindow.h"

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
	InspectorWindow m_inspectorWindow;
	HierarchyWindow m_hierarchyWindow;

    void DrawTransform(GameObject& obj);
    void DrawRigidbody(GameObject& obj);
    void DrawCollider(GameObject& obj);
    void DrawMaterial(GameObject& obj);
    void DrawBoxCollider(GameObject& obj);
};