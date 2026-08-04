#pragma once

class GameObject;

class Component
{
    friend class ComponentSystem;

protected:

    bool m_awaked = false;

    bool m_started = false;

public:

    GameObject* gameObject = nullptr;

    bool enabled = true;

    virtual ~Component() = default;

    virtual void Awake() {}
    virtual void Start() {}
    virtual void Update(float dt) {}
    virtual void FixedUpdate(float dt) {}
    virtual void LateUpdate(float dt) {}

    void SetGameObject(GameObject* obj)
    {
        gameObject = obj;
    }

    GameObject* GetGameObject() const
    {
        return gameObject;
    }
};