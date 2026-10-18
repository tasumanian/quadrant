#pragma once
#include "Components.h"
#include "GameObject.h"
#include "InspectorWindow.h"
#include "HierarchyWindow.h"

class Scene;
class Window;
class CameraSystem;
class PhysicsSystem;

class Editor
{

public:

    void Select(GameObject* obj);

    GameObject* GetSelectedObject();
    void Update(Window& window, CameraSystem& cameraSystem, PhysicsSystem& physicsSystem, Scene& scene);
    void Draw(Scene* scene);
    void Init(Window* window);

private:

    GameObject* m_selectedObject = nullptr;
	InspectorWindow m_inspectorWindow;
	HierarchyWindow m_hierarchyWindow;

    //void DrawTransform(GameObject& obj);
    //void DrawRigidbody(GameObject& obj);
    //void DrawCollider(GameObject& obj);
    //void DrawMaterial(GameObject& obj);
    //void DrawBoxCollider(GameObject& obj);
};