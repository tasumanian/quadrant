#pragma once

class GameObject;

class Component
{
public:

    GameObject* gameObject = nullptr;

    virtual ~Component() = default;

    virtual void Awake() {}
    virtual void Start() {}
    virtual void Update(float dt) {}
    virtual void FixedUpdate(float dt) {}

    void SetGameObject(GameObject* obj)
    {
        gameObject = obj;
    }

    GameObject* GetGameObject() const
    {
        return gameObject;
    }
};